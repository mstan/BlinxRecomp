#include "title_mod.h"
#include "visual_mod_plan.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BLINX_SHA "e6b7cd8a5527dd001e05bb3e881d9b24ffc58824ff67def394215ddbc8009249"
static const struct PilotTitleModHost *host;
static unsigned numerator,denominator,adaptive,phase,phase_dirty=1,projection_seen,frame_aspect[2];
static unsigned frame_mode;
static FILE *trace;
static unsigned trace_count;
static unsigned trace_ui_only,trace_ready;
static const char *trace_trigger;
static unsigned trace_armed;
static void fail(const char *reason)
{fprintf(stderr,"[TITLE MOD STOP] %s\n",reason);fflush(stderr);exit(2);}
static void read_data(unsigned address,void *data,unsigned bytes)
{if(!host->read(address,data,bytes))fail("Blinx mod data is unavailable");}
static unsigned mode(void)
{if(phase_dirty){read_data(0x50a57c,&phase,4);phase&=0x7f;phase_dirty=0;}return phase;}
/* Screen 10 replays the stage behind the Results overlay. */
static int scene_mode(void){unsigned value=mode();return value==9 || value==10;}
static void open_trace(void)
{
    const char *path=getenv("BLINX_MOD_DRAW_TRACE");
    if(path && *path){trace=fopen(path,"wb");if(!trace)fail("mod draw trace cannot be opened");}
    const char *filter=getenv("BLINX_MOD_DRAW_TRACE_UI_ONLY");
    trace_ui_only=filter && !strcmp(filter,"raster") ? 4 :
        filter && !strcmp(filter,"effects") ? 3 :
        filter && !strcmp(filter,"populated") ? 2 : filter && !strcmp(filter,"1");
    trace_trigger=getenv("BLINX_MOD_DRAW_TRACE_TRIGGER");
    trace_armed=!trace_trigger || !*trace_trigger;
}
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
    if(!scene_mode())return;
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
    if(!scene_mode() || !projection_seen || !draw->positions || draw->count>4096 || draw->stride<8 ||
       (draw->position_type!=0x22 && draw->position_type!=0x32 && draw->position_type!=0x42 && draw->position_type!=0x72))return;
    int marker=0;
    if(draw->caller==0x33811 && draw->depth_test) {
        unsigned programs[2];read_data(0x6678a8,programs,sizeof(programs));
        marker=draw->shader==programs[0] || draw->shader==programs[1];
    }
    int sprite=draw->caller==0x30c0a || (draw->caller==0x33811 && (!draw->depth_test || marker));
    int camera_sprite=draw->caller==0x30c0a && draw->depth_test;
    int text=draw->caller==0xef2ae || draw->caller==0xef8a4 || draw->caller==0xf183a;
    /* The ordinary gameplay scene is rendered into a texture and copied to
     * the backbuffer by 9D9DB. Its camera proxies and world markers need the
     * same placement there. Keep offscreen UI/effect producers unchanged. */
    int excluded_target=!draw->default_target && !camera_sprite && !marker;
    int tracing=trace && trace_armed && trace_count<1024;
    if(excluded_target && !tracing)return;
    if(!sprite && !text && !tracing)return;
    float min_x=1e20f,min_y=1e20f,max_x=-1e20f,max_y=-1e20f;
    for(unsigned i=0;i<draw->count;++i) {
        float xy[2];memcpy(xy,draw->positions+i*draw->stride,8);
        if(!isfinite(xy[0]) || !isfinite(xy[1]))return;
        if(xy[0]<min_x)min_x=xy[0];
        if(xy[0]>max_x)max_x=xy[0];
        if(xy[1]<min_y)min_y=xy[1];
        if(xy[1]>max_y)max_y=xy[1];
    }
    if(tracing && (trace_ui_only!=2 || trace_ready) &&
       (trace_ui_only!=4 || (draw->caller==0x30c0a && draw->default_target && draw->count==4 &&
                            fabsf(min_x+320)<.001f && fabsf(max_x-320)<.001f &&
                            fabsf(min_y+240)<.001f && fabsf(max_y-240)<.001f)) &&
       (trace_ui_only!=3 || (!draw->depth_test && draw->count<=24 &&
                            max_x-min_x>=500 && max_y-min_y<=64)) &&
       (!trace_ui_only || sprite || text || draw->caller==0x33811 || !draw->depth_test)) {
        float z=0;if(draw->position_type!=0x22)memcpy(&z,draw->positions+8,4);
        unsigned textures[4],frame,time_state;read_data(0x142e58,textures,sizeof(textures));read_data(0x50a464,&frame,4);read_data(0x50a428,&time_state,4);
        fprintf(trace,"%u %08x %08x %u %u %.6g %.6g %.6g %.6g z=%.9g depth=%u/%u target=%u frame=%u time=%u textures=%08x,%08x,%08x,%08x\n",trace_count++,draw->caller,
            draw->shader,draw->position_type,draw->count,min_x,min_y,max_x,max_y,z,draw->depth_test,draw->depth_write,
            draw->default_target,frame,time_state,textures[0],textures[1],textures[2],textures[3]);fflush(trace);
    }
    if(excluded_target)return;
    /* 30B16 submits ordinary screen-space sprites through the title's 2D
     * shader. World geometry and full-screen history/effect passes retain
     * their projection and raster mapping. Other UI helpers are added only
     * after identifying their actual position contract. */
    if(!sprite && !text)return;
    /* The ordinary sprite queue stores centered XY; its original shader adds
     * c121=(320,240,0,0). Font FVF submits absolute screen pixels instead. */
    if(sprite){min_x+=320;max_x+=320;min_y+=240;max_y+=240;}
    if(!camera_sprite && min_x<=1 && max_x>=639 && min_y<=1 && max_y>=479)return;
    /* The full-screen tint/fade uses 30C0A with depth testing enabled, both
     * during Retry and its surrounding lifecycle. That helper also submits
     * world visibility proxies; preserve only this exact raster-covering
     * quad, including its constant near-raster depth, without a time-state
     * condition that would leave a narrow prior tint in frame history. */
    if(camera_sprite && draw->default_target && draw->count==4 &&
       draw->position_type==0x32 && draw->stride>=12 && !draw->depth_write &&
       fabsf(min_x)<.001f && fabsf(max_x-640)<.001f &&
       fabsf(min_y)<.001f && fabsf(max_y-480)<.001f) {
        int raster_quad=1;
        for(unsigned i=0;i<4;++i) {
            float z;memcpy(&z,draw->positions+i*draw->stride+8,4);
            if(!isfinite(z) || fabsf(z-1)>.001f)raster_quad=0;
        }
        if(raster_quad)return;
    }
    /* Automatic Retry draws six full-width 12-pixel tape-noise strips through
     * the ordinary sprite queue. They cover the scene, so retaining their
     * original raster mapping fills the wide frame. Resolve o_noise through
     * the title's live catalogue/owner table; resource handles change on load.
     * Do not exempt similarly sized HUD/dialog sprites or offscreen effects. */
    if(draw->caller==0x33811 && !draw->depth_test && draw->count==4 &&
       draw->position_type==0x32 && min_x>=-1 && min_x<=1 &&
       max_x>=639 && max_x<=641 && max_y-min_y>0 && max_y-min_y<=64) {
        unsigned slot,bound,noise=0;
        read_data(0x1e220c,&slot,4);read_data(0x142e58,&bound,4);
        if(slot<1024)read_data(0x8d2860+slot*4,&noise,4);
        if(noise && bound==noise)return;
    }
    unsigned n=frame_aspect[0],d=frame_aspect[1];
    draw->x_scale=4.0f*d/(3.0f*n);draw->x_anchor=320;
    /* 36180 builds depth-tested type-4 billboards with an already projected
     * center, but a radius from the original pixel focal length. Preserve
     * that center and correct only the width (enemy health/target circles).
     * The type-4 program pair lives at 6678A8; depth alone also matches world
     * geometry in this queue and cannot identify its position contract. */
    if(marker){draw->x_anchor=(min_x+max_x)*.5f;return;}
    /* The same immediate sprite helper draws the depth-tested visibility
     * proxies. Those follow the camera, including beyond the old 4:3 edges;
     * a proxy is never an edge-anchored HUD item or a full-screen effect. */
    if(mode()==9 && !camera_sprite && min_y>=-32 && max_y<=160) {
        if(max_x<=270)draw->x_anchor=0;
        else if(min_x>=400)draw->x_anchor=640;
    }
    /* The lower strip has three independent groups: crystal queue through
     * X=216, centered time controls, and trash inventory from X=448 onward.
     * Apply these bounds to absolute screen pixels, including sprite bias. */
    if(mode()==9 && !camera_sprite && min_y>=320 && max_y<=480) {
        /* The slot-frame artwork occupies a padded 0..256 quad. Its visible
         * circles belong to the crystal queue; the adjacent rewind button
         * starts at X=218 and must retain the centered time-control anchor. */
        if(max_x<=216 || (min_x<=1 && max_x<=256))draw->x_anchor=0;
        else if(min_x>=448)draw->x_anchor=640;
    }
}
static void present(void)
{
    phase_dirty=1;
    if(trace && !trace_armed) {
        FILE *trigger=fopen(trace_trigger,"rb");
        if(trigger){fclose(trigger);trace_armed=1;}
    }
    if(frame_mode) {
        unsigned paused=0,stage=0;read_data(0x50a534,&paused,4);read_data(0x50312c,&stage,4);
        /* Movies, menus, pause and stage changes have independent history.
         * Synthetic frames never run this callback or any guest operation. */
        host->frame_scope(mode()==9 && !paused?stage+1:0);
    }
    if(trace && trace_ui_only==2 && !trace_ready && mode()==9) {
        unsigned player;unsigned char counts[2];read_data(0x4eb6c4,&player,4);
        if(player && player<0x4000000-0x177) {
            read_data(player+0x175,counts,2);trace_ready=counts[0] || counts[1];
        }
    }
    unsigned n=4,d=3;
    if(scene_mode() && projection_seen){n=frame_aspect[0];d=frame_aspect[1];}
    if(!host->frame_shape(n,d))fail("unsupported mod presentation shape");
    phase_dirty=1;
}
const struct PilotTitleMod *pilot_title_mod_open(const struct PilotTitleModHost *api)
{
    const char *path=getenv("BLINX_MOD_PLAN");if(!path || !*path)return NULL;
    if(!api || api->abi!=PILOT_TITLE_MOD_ABI || strcmp(api->xbe_sha256,BLINX_SHA))fail("widescreen title identity mismatch");
    host=api;
    FILE *file=fopen(path,"rb");if(!file)fail("resolved mod plan is unavailable");
    char header[64];if(!fgets(header,sizeof(header),file))fail("empty resolved mod plan");rewind(file);
    if(!strncmp(header,"xlle-mod-plan 2",15)) {
        struct PilotVisualPlan plan;
        if(!pilot_visual_plan_read(file,&plan)){fclose(file);fail("invalid resolved visual mod plan");}
        fclose(file);
        if(strcmp(plan.identity,BLINX_SHA) ||
           ((plan.operations&PILOT_MOD_ASPECT) && strcmp(plan.aspect_plugin,"blinx.widescreen")))
            fail("visual mod title/plugin identity mismatch");
        unsigned wide=plan.operations&PILOT_MOD_ASPECT;
        if(wide) {
            unsigned literal;read_data(0x2c86b,&literal,4);
            if(literal!=0xbfaaaaab)fail("Blinx projection call-site guard differs");
        }
        if(plan.options.frame_mode && !host->frame_scope)fail("frame-rate scope callback is unavailable");
        if(!host->visual_mods || !host->visual_mods(&plan.options))fail("visual mod options are unsupported by this build/profile");
        if(!plan.operations)return NULL;
        numerator=plan.options.aspect_numerator;denominator=plan.options.aspect_denominator;adaptive=plan.adaptive;
        frame_mode=plan.options.frame_mode;
        if(wide && !host->presentation(numerator,denominator,720))fail("unsupported visual mod presentation");
        fprintf(stderr,"[MOD] visual-plan=2 operations=%u aspect=%u:%u adaptive=%u height=%u ssaa=%u aa=%u aniso=%u frames=%u/%u\n",
            plan.operations,numerator,denominator,adaptive,plan.options.internal_height,plan.options.supersample,
            plan.options.antialiasing,plan.options.anisotropy,plan.options.frame_mode,plan.options.frame_rate);
        static struct PilotTitleMod combined;
        combined=(struct PilotTitleMod){PILOT_TITLE_MOD_ABI,wide?transform:NULL,wide?draw:NULL,(wide||frame_mode)?present:NULL};
        if(wide)open_trace();
        return &combined;
    }
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
    open_trace();
    fprintf(stderr,"[MOD] package=%s version=%s plugin=%s aspect=%u:%u adaptive=%u original-image=unchanged\n",
        package,version,plugin,numerator,denominator,adaptive);
    static const struct PilotTitleMod result={PILOT_TITLE_MOD_ABI,transform,draw,present};return &result;
}
