#include "title_mod.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BLINX_SHA "e6b7cd8a5527dd001e05bb3e881d9b24ffc58824ff67def394215ddbc8009249"
static const struct PilotTitleModHost *host;
static unsigned numerator,denominator,adaptive,phase,phase_dirty=1,projection_seen,frame_aspect[2];
static FILE *trace;
static unsigned trace_count;
static void fail(const char *reason)
{fprintf(stderr,"[TITLE MOD STOP] %s\n",reason);fflush(stderr);exit(2);}
static void read_data(unsigned address,void *data,unsigned bytes)
{if(!host->read(address,data,bytes))fail("Blinx mod data is unavailable");}
static unsigned mode(void)
{if(phase_dirty){read_data(0x50a57c,&phase,4);phase&=0x7f;phase_dirty=0;}return phase;}
static void aspect(unsigned *n,unsigned *d)
{
    *n=numerator;*d=denominator;
    if(adaptive) {
        unsigned size[2];
        if(host->client_size(size)) {
            if((unsigned long long)size[0]*3<(unsigned long long)size[1]*4){*n=4;*d=3;}
            else if((unsigned long long)size[0]*9>(unsigned long long)size[1]*32){*n=32;*d=9;}
            else {*n=size[0];*d=size[1];}
        }
    }
}
static void transform(unsigned state,unsigned address,unsigned caller)
{
    if(state!=1 || address!=0x39d910 || caller!=0x2c898)return;
    phase_dirty=1;
    if(mode()!=9)return;
    float matrix[16];read_data(address,matrix,sizeof(matrix));
    /* This is the original perspective builder's negative 4:3 aspect. Change
     * its data before SetTransform and the title's following matrix copy;
     * CPU screen-space/culling consumers consequently see the same frustum. */
    if(!isfinite(matrix[0]) || !isfinite(matrix[5]) || matrix[0]==0 ||
       fabsf(-matrix[5]/matrix[0]-4.0f/3.0f)>.001f)fail("unexpected original Blinx projection");
    unsigned n,d;aspect(&n,&d);
    float value=matrix[0]*(4.0f*d/(3.0f*n));
    if(!host->write_data(address,&value,4))fail("Blinx projection data is not writable");
    projection_seen=1;
    frame_aspect[0]=n;frame_aspect[1]=d;
}
static void draw(struct PilotTitleModDraw *draw)
{
    if(mode()!=9 || !projection_seen || !draw->default_target || !draw->positions || draw->count>4096 || draw->stride<8 ||
       (draw->position_type!=0x22 && draw->position_type!=0x32 && draw->position_type!=0x42 && draw->position_type!=0x72))return;
    int sprite=draw->caller==0x30c0a || (draw->caller==0x33811 && !draw->depth_test);
    int text=draw->caller==0xef2ae || draw->caller==0xef8a4 || draw->caller==0xf183a;
    if(!sprite && !text && (!trace || trace_count>=1024))return;
    float min_x=1e20f,min_y=1e20f,max_x=-1e20f,max_y=-1e20f;
    for(unsigned i=0;i<draw->count;++i) {
        float xy[2];memcpy(xy,draw->positions+i*draw->stride,8);
        if(!isfinite(xy[0]) || !isfinite(xy[1]))return;
        if(xy[0]<min_x)min_x=xy[0];
        if(xy[0]>max_x)max_x=xy[0];
        if(xy[1]<min_y)min_y=xy[1];
        if(xy[1]>max_y)max_y=xy[1];
    }
    if(trace && trace_count<1024) {
        float z=0;if(draw->position_type!=0x22)memcpy(&z,draw->positions+8,4);
        fprintf(trace,"%u %08x %08x %u %u %.6g %.6g %.6g %.6g z=%.9g depth=%u/%u\n",trace_count++,draw->caller,
            draw->shader,draw->position_type,draw->count,min_x,min_y,max_x,max_y,z,draw->depth_test,draw->depth_write);fflush(trace);
    }
    /* 30B16 submits ordinary screen-space sprites through the title's 2D
     * shader. World geometry and full-screen history/effect passes retain
     * their projection and raster mapping. Other UI helpers are added only
     * after identifying their actual position contract. */
    if(!sprite && !text)return;
    /* The ordinary sprite queue stores centered XY; its original shader adds
     * c121=(320,240,0,0). Font FVF submits absolute screen pixels instead. */
    if(sprite){min_x+=320;max_x+=320;min_y+=240;max_y+=240;}
    if(min_x<=1 && max_x>=639 && min_y<=1 && max_y>=479)return;
    unsigned n=frame_aspect[0],d=frame_aspect[1];
    draw->x_scale=4.0f*d/(3.0f*n);draw->x_anchor=320;
    if(min_y>=-32 && max_y<=160) {
        if(max_x<=270)draw->x_anchor=0;
        else if(min_x>=400)draw->x_anchor=640;
    }
    /* The lower strip has three independent groups: crystal queue through
     * X=216, centered time controls, and trash inventory from X=448 onward.
     * Apply these bounds to absolute screen pixels, including sprite bias. */
    if(min_y>=320 && max_y<=480) {
        if(max_x<=216)draw->x_anchor=0;
        else if(min_x>=448)draw->x_anchor=640;
    }
}
static void present(void)
{
    phase_dirty=1;
    unsigned n=4,d=3;
    if(mode()==9 && projection_seen){n=frame_aspect[0];d=frame_aspect[1];}
    if(!host->frame_shape(n,d))fail("unsupported mod presentation shape");
    phase_dirty=1;
}
const struct PilotTitleMod *pilot_title_mod_open(const struct PilotTitleModHost *api)
{
    const char *path=getenv("BLINX_MOD_PLAN");if(!path || !*path)return NULL;
    if(!api || api->abi!=PILOT_TITLE_MOD_ABI || strcmp(api->xbe_sha256,BLINX_SHA))fail("widescreen title identity mismatch");
    host=api;
    FILE *file=fopen(path,"rb");if(!file)fail("resolved mod plan is unavailable");
    char package[96],version[32],plugin[96],identity[65],extra;
    int fields=fscanf(file,"xlle-mod-plan 1\npackage %95s %31s\nplugin %95s\nidentity %64s\naspect %u %u\nadaptive %u\n %c",
        package,version,plugin,identity,&numerator,&denominator,&adaptive,&extra);
    fclose(file);
    if(fields!=7 || strcmp(package,"blinx.enhancement.widescreen") || strcmp(version,"1.0.0") ||
       strcmp(plugin,"blinx.widescreen") || strcmp(identity,BLINX_SHA) || adaptive>1)
        fail("invalid resolved widescreen mod plan");
    unsigned literal;read_data(0x2c86b,&literal,4);
    if(literal!=0xbfaaaaab)fail("Blinx projection call-site guard differs");
    if(!host->presentation(numerator,denominator,720))fail("aspect is outside 4:3 through 32:9 or backend is unsupported");
    const char *trace_path=getenv("BLINX_MOD_DRAW_TRACE");
    if(trace_path && *trace_path){trace=fopen(trace_path,"wb");if(!trace)fail("mod draw trace cannot be opened");}
    fprintf(stderr,"[MOD] package=%s version=%s plugin=%s aspect=%u:%u adaptive=%u original-image=unchanged\n",
        package,version,plugin,numerator,denominator,adaptive);
    static const struct PilotTitleMod result={PILOT_TITLE_MOD_ABI,transform,draw,present};return &result;
}
