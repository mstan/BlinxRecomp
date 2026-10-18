#include "title_mod.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"failed line %d: %s\n",__LINE__,#c);return 1;}}while(0)
static float matrix[16]={.75f,0,0,0,0,-1,0,0,0,0,1,1,0,0,-1,0};
static unsigned phase=0x89,numerator,denominator,shape[2],writes;
static unsigned paused,stage,scope,frames;
static int read_data(unsigned address,void *data,unsigned bytes)
{
    unsigned guard=0xbfaaaaab;
    if(address==0x2c86b && bytes==4)memcpy(data,&guard,4);
    else if(address==0x50a57c && bytes==4)memcpy(data,&phase,4);
    else if(address==0x50a534 && bytes==4)memcpy(data,&paused,4);
    else if(address==0x50312c && bytes==4)memcpy(data,&stage,4);
    else if(address==0x39d910 && bytes==64)memcpy(data,matrix,64);
    else return 0;
    return 1;
}
static int write_data(unsigned address,const void *data,unsigned bytes)
{if(address!=0x39d910 || bytes!=4)return 0;memcpy(matrix,data,4);++writes;return 1;}
static int presentation(unsigned n,unsigned d,unsigned height)
{
    if(!n || !d || height!=720 || n>65536 || d>65536 || (unsigned long long)n*3<(unsigned long long)d*4 ||
       (unsigned long long)n*9>(unsigned long long)d*32)return 0;
    numerator=n;denominator=d;return 1;
}
static int client_size(unsigned size[2]){size[0]=1680;size[1]=720;return 1;}
static int frame_shape(unsigned n,unsigned d){shape[0]=n;shape[1]=d;return 1;}
static unsigned quality_calls;
static int visual_mods(const struct PilotVisualMods *options)
{++quality_calls;frames=options->frame_mode;return pilot_visual_mods_validate(options);}
static void frame_scope(unsigned key){scope=key;}
static int close_to(float a,float b){return fabsf(a-b)<.001f;}
int main(int argc,char **argv)
{
    struct PilotTitleModHost host={PILOT_TITLE_MOD_ABI,
        "e6b7cd8a5527dd001e05bb3e881d9b24ffc58824ff67def394215ddbc8009249",
        read_data,write_data,presentation,client_size,frame_shape,visual_mods,frame_scope};
#ifdef _WIN32
    _putenv_s("BLINX_MOD_PLAN","");
    if(argc>=2)_putenv_s("BLINX_MOD_PLAN",argv[1]);
#else
    unsetenv("BLINX_MOD_PLAN");
    if(argc>=2)setenv("BLINX_MOD_PLAN",argv[1],1);
#endif
    const struct PilotTitleMod *mod=pilot_title_mod_open(&host);
    if(argc==3 && !strcmp(argv[2],"--quality-only")) {
        CHECK(mod && !mod->transform && !mod->draw && !mod->present && quality_calls==1 && !writes && !numerator);return 0;
    }
    if(argc==3 && !strcmp(argv[2],"--frame-only")) {
        CHECK(mod && !mod->transform && !mod->draw && mod->present && quality_calls==1 && !writes && !numerator);
        mod->present();CHECK(scope==1);paused=1;mod->present();CHECK(!scope);
        paused=0;stage=7;mod->present();CHECK(scope==8);return 0;
    }
    if(argc==1){CHECK(!mod && !writes && !numerator);return 0;}
    CHECK(mod && mod->abi==PILOT_TITLE_MOD_ABI);
    mod->transform(1,0x39d910,0x2c898);
    CHECK(writes==1);
    /* The same world-space X/Y distance has equal displayed pixel lengths;
     * projection widens the world rather than stretching its geometry. */
    float aspect=(float)numerator/denominator,stretch=aspect/(4.0f/3.0f);
    CHECK(close_to(matrix[0]*aspect,-matrix[5]));
    mod->present();CHECK(shape[0]*denominator==shape[1]*numerator);
    if(frames){CHECK(scope==1);paused=1;mod->present();CHECK(!scope);paused=0;stage=1;mod->present();CHECK(scope==2);stage=0;}
    float quad[4][4]={{50,30,0,1},{180,30,0,1},{50,80,0,1},{180,80,0,1}};
    struct PilotTitleModDraw draw={0xef2ae,0x144,6,4,1,(const unsigned char*)quad,16,0x42,1,0,0,0};
    mod->draw(&draw);
    CHECK(close_to(130*draw.x_scale*stretch,130));
    CHECK(close_to((50*draw.x_scale+draw.x_anchor*(1-draw.x_scale))*stretch,50));
    quad[0][0]=quad[2][0]=500;quad[1][0]=quad[3][0]=600;
    draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(640*stretch-(600*draw.x_scale+draw.x_anchor*(1-draw.x_scale))*stretch,40));
    quad[0][0]=quad[2][0]=200;quad[1][0]=quad[3][0]=440;
    quad[0][1]=quad[1][1]=250;quad[2][1]=quad[3][1]=350;
    draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to((320*draw.x_scale+draw.x_anchor*(1-draw.x_scale))*stretch,320*stretch));
    /* The collected-crystal queue is left anchored, while the neighboring
     * rewind button belongs to the centered time-control group. These sprite
     * inputs are centered XY, unlike the font's absolute-pixel inputs above. */
    quad[0][0]=quad[2][0]=-270;quad[1][0]=quad[3][0]=-108;
    quad[0][1]=quad[1][1]=128;quad[2][1]=quad[3][1]=210;
    draw.caller=0x33811;draw.position_type=0x32;draw.shader=1;draw.x_scale=1;draw.x_anchor=0;
    mod->draw(&draw);
    CHECK(draw.x_anchor==0 && close_to((50*draw.x_scale)*stretch,50));
    CHECK(close_to(162*draw.x_scale*stretch,162));
    /* Actual populated-HUD trace: the slot-frame texture has transparent
     * padding across absolute X=0..256, Y=388..452 (33811, float3 quad).
     * Move its artwork with the icons, while retaining the neighboring
     * rewind button's independent center anchor below. */
    quad[0][0]=quad[2][0]=-320;quad[1][0]=quad[3][0]=-64;
    quad[0][1]=quad[1][1]=148;quad[2][1]=quad[3][1]=212;
    draw.x_scale=1;draw.x_anchor=320;mod->draw(&draw);
    CHECK(draw.x_anchor==0 && close_to(256*draw.x_scale*stretch,256));
    quad[0][0]=quad[2][0]=-100;quad[1][0]=quad[3][0]=-64;
    quad[0][1]=quad[1][1]=160;quad[2][1]=quad[3][1]=194;
    draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);CHECK(draw.x_anchor==320);
    quad[0][0]=quad[2][0]=170;quad[1][0]=quad[3][0]=270;
    draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(draw.x_anchor==640);
    CHECK(close_to(640*stretch-(590*draw.x_scale+draw.x_anchor*(1-draw.x_scale))*stretch,50));
    draw.caller=0x34a07;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);CHECK(draw.x_scale==1);
    phase=5;mod->present();CHECK(shape[0]==4 && shape[1]==3);
    draw.caller=0xef2ae;draw.x_scale=1;mod->draw(&draw);CHECK(draw.x_scale==1);
    CHECK(writes==1);return 0;
}
