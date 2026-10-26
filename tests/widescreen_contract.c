#include "title_mod.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"failed line %d: %s\n",__LINE__,#c);return 1;}}while(0)
static float matrix[16]={.75f,0,0,0,0,-1,0,0,0,0,1,1,0,0,-1,0};
static unsigned phase=0x89,numerator,denominator,shape[2],writes;
static unsigned paused,stage,scope,frames,time_state;
static float cover_progress,cover_dim;
static unsigned cover_held,cover_slot=92,glow_slot=93,label_slot=94;
static unsigned trim_slot=29,large_trim_slot=30,trim_header=0xd0000628,large_trim_header=0xd0000648;
static unsigned cover_header=0xd0000e08,glow_header=0xd0000e28,label_header=0xd0000e48;
static unsigned noise_slot=24,noise_header=0xd0000528,bound_texture;
static unsigned preview_slots[9]={200,201,202,203,204,205,206,207,208};
static unsigned round_slots[2]={209,210};
static unsigned currency_slots[3]={211,212,213};
static unsigned billboard_programs[2]={0x123001,0x456001};
static int read_data(unsigned address,void *data,unsigned bytes)
{
    unsigned guard=0xbfaaaaab;
    const unsigned currency_records[3]={0x23b578,0x23b5a0,0x23b5e0};
    for(unsigned i=0;i<3;++i) {
        unsigned texture=0xd0007508+32*i;
        if(address==currency_records[i]+28 && bytes==4){memcpy(data,&currency_slots[i],4);return 1;}
        if(address==0x8d2860+4*currency_slots[i] && bytes==4){memcpy(data,&texture,4);return 1;}
    }
    for(unsigned i=0;i<9;++i) {
        unsigned texture=0xd0007008+32*i;
        if(address==0x1e351c+32*i && bytes==4){memcpy(data,&preview_slots[i],4);return 1;}
        if(address==0x8d2860+4*preview_slots[i] && bytes==4){memcpy(data,&texture,4);return 1;}
    }
    for(unsigned i=0;i<2;++i) {
        unsigned texture=0xd0007408+32*i;
        if(address==(i?0x23b33c:0x23b294) && bytes==4){memcpy(data,&round_slots[i],4);return 1;}
        if(address==0x8d2860+4*round_slots[i] && bytes==4){memcpy(data,&texture,4);return 1;}
    }
    if(address==0x2c86b && bytes==4)memcpy(data,&guard,4);
    else if(address==0x50a57c && bytes==4)memcpy(data,&phase,4);
    else if(address==0x62f308 && bytes==4)memcpy(data,&cover_progress,4);
    else if(address==0x62ff34 && bytes==4)memcpy(data,&cover_held,4);
    else if(address==0x50aa64 && bytes==4)memcpy(data,&cover_dim,4);
    else if(address==0x23b184 && bytes==4)memcpy(data,&cover_slot,4);
    else if(address==0x23b1a4 && bytes==4)memcpy(data,&glow_slot,4);
    else if(address==0x23b1c4 && bytes==4)memcpy(data,&label_slot,4);
    else if(address==0x8d2860+cover_slot*4 && bytes==4)memcpy(data,&cover_header,4);
    else if(address==0x8d2860+glow_slot*4 && bytes==4)memcpy(data,&glow_header,4);
    else if(address==0x8d2860+label_slot*4 && bytes==4)memcpy(data,&label_header,4);
    else if(address==0x1e22ac && bytes==4)memcpy(data,&trim_slot,4);
    else if(address==0x1e22cc && bytes==4)memcpy(data,&large_trim_slot,4);
    else if(address==0x8d2860+trim_slot*4 && bytes==4)memcpy(data,&trim_header,4);
    else if(address==0x8d2860+large_trim_slot*4 && bytes==4)memcpy(data,&large_trim_header,4);
    else if(address==0x50a534 && bytes==4)memcpy(data,&paused,4);
    else if(address==0x50312c && bytes==4)memcpy(data,&stage,4);
    else if(address==0x50a428 && bytes==4)memcpy(data,&time_state,4);
    else if(address==0x1e220c && bytes==4)memcpy(data,&noise_slot,4);
    else if(address==0x142e58 && bytes==4)memcpy(data,&bound_texture,4);
    else if(address==0x8d2860+noise_slot*4 && bytes==4)memcpy(data,&noise_header,4);
    else if(address==0x39d910 && bytes==64)memcpy(data,matrix,64);
    else if(address==0x6678a8 && bytes==8)memcpy(data,billboard_programs,8);
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
    _putenv_s("XLLE_PILOT_MOD_PLAN","");
    if(argc>=2)_putenv_s(argc==3 && !strcmp(argv[2],"--legacy-plan")?"BLINX_MOD_PLAN":"XLLE_PILOT_MOD_PLAN",argv[1]);
    if(argc==3 && !strcmp(argv[2],"--common-priority"))_putenv_s("BLINX_MOD_PLAN","missing-legacy-plan");
#else
    unsetenv("BLINX_MOD_PLAN");
    unsetenv("XLLE_PILOT_MOD_PLAN");
    if(argc>=2)setenv(argc==3 && !strcmp(argv[2],"--legacy-plan")?"BLINX_MOD_PLAN":"XLLE_PILOT_MOD_PLAN",argv[1],1);
    if(argc==3 && !strcmp(argv[2],"--common-priority"))setenv("BLINX_MOD_PLAN","missing-legacy-plan",1);
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
    /* Covers also run before the first gameplay camera is ever submitted. */
    phase=5;cover_progress=.5f;mod->present();
    CHECK(shape[0]*denominator==shape[1]*numerator && writes==0);
    float opening[4][3]={{-320,-240,1},{-96,-240,1},{-320,240,1},{-96,240,1}};
    struct PilotTitleModDraw cold={0x33811,1,6,4,1,(const unsigned char*)opening,12,0x32,1,0,0,0};
    bound_texture=cover_header;mod->draw(&cold);CHECK(cold.x_scale==1 && cold.x_anchor==0);
    bound_texture=label_header;mod->draw(&cold);CHECK(cold.x_anchor==320);
    cover_progress=0;bound_texture=0;mod->present();CHECK(shape[0]==4 && shape[1]==3);
    phase=0x89;
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
    /* Actual inventory-frame trace: these two decorative continuation
     * pieces leave a hanging spike beside the independently anchored icons.
     * Only their live resources are clipped, only on the wide default HUD. */
    for(unsigned resource=0;resource<2;++resource)for(unsigned part=0;part<2;++part) {
        quad[0][0]=quad[2][0]=235;quad[1][0]=quad[3][0]=299;
        quad[0][1]=quad[1][1]=part?220:92;quad[2][1]=quad[3][1]=part?240:220;
        bound_texture=resource?large_trim_header:trim_header;
        draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
        float left=555*draw.x_scale+draw.x_anchor*(1-draw.x_scale);
        if(stretch>1.001f)CHECK(left>640);
        else CHECK(draw.x_scale==1);
        /* Ordinary icons, other targets and projected geometry still draw. */
        bound_texture=0;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);CHECK(draw.x_anchor==640);
        bound_texture=trim_header;draw.default_target=0;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
        CHECK(draw.x_scale==1 && draw.x_anchor==0);draw.default_target=1;
    }
    bound_texture=0;
    /* Captured Gold pickup stack: third and later rows cross above Y=320.
     * All ten original rows, their glyphs and total panel stay in one group. */
    for(unsigned asset=0;asset<3;++asset)for(unsigned row=0;row<10;++row) {
        bound_texture=0xd0007508+32*asset;
        quad[0][0]=quad[2][0]=-270.6667f;quad[1][0]=quad[3][0]=-126.6667f;
        quad[0][1]=quad[1][1]=103.6667f-16*row;
        quad[2][1]=quad[3][1]=119.6667f-16*row;
        draw.x_scale=1;draw.x_anchor=320;mod->draw(&draw);
        CHECK(draw.x_anchor==0 && close_to(draw.x_scale*stretch,1));
    }
    /* The same number atlas in the middle of the screen is unrelated UI. */
    quad[0][0]=quad[2][0]=-20;quad[1][0]=quad[3][0]=20;
    quad[0][1]=quad[1][1]=70;quad[2][1]=quad[3][1]=86;
    draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);CHECK(draw.x_anchor==320);
    bound_texture=0;
    /* Visibility proxies share the immediate sprite helper. A proxy in a
     * HUD corner must follow its world position rather than that HUD edge. */
    draw.caller=0x30c0a;draw.depth_test=1;draw.depth_write=0;
    for(unsigned side=0;side<2;++side) {
        float center=side?700:-60;
        quad[0][0]=quad[2][0]=center-340;quad[1][0]=quad[3][0]=center-300;
        quad[0][1]=quad[1][1]=side?-220:100;quad[2][1]=quad[3][1]=side?-180:140;
        draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
        CHECK(draw.x_anchor==320);
        float mapped=center*draw.x_scale+draw.x_anchor*(1-draw.x_scale);
        CHECK(close_to((mapped-320)*stretch,center-320));
        /* After scene history initializes, these same original submissions
         * target a scene texture rather than the default backbuffer. */
        draw.default_target=0;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
        CHECK(draw.x_anchor==320);
        mapped=center*draw.x_scale+draw.x_anchor*(1-draw.x_scale);
        CHECK(close_to((mapped-320)*stretch,center-320));
        draw.default_target=1;
    }
    /* The full-screen tint uses the immediate helper in Retry and neighboring
     * lifecycle states. The raster-depth/default-target contract fills the
     * wide raster; ordinary world proxies and neighboring geometry still map. */
    quad[0][0]=quad[2][0]=-320;quad[1][0]=quad[3][0]=320;
    quad[0][1]=quad[1][1]=-240;quad[2][1]=quad[3][1]=240;
    for(unsigned i=0;i<4;++i)quad[i][2]=1;
    time_state=4;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(draw.x_scale==1 && draw.x_anchor==0);
    time_state=0;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(draw.x_scale==1 && draw.x_anchor==0);
    quad[3][2]=2;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(draw.x_scale*stretch,1) && draw.x_anchor==320);
    quad[3][2]=1;
    time_state=4;draw.default_target=0;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(draw.x_scale*stretch,1) && draw.x_anchor==320);
    draw.default_target=1;draw.depth_write=1;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(draw.x_scale*stretch,1) && draw.x_anchor==320);
    draw.depth_write=0;draw.depth_test=0;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(draw.x_scale==1 && draw.x_anchor==0); /* Existing non-depth full-screen path. */
    draw.depth_test=1;quad[3][0]=319;
    quad[1][0]=319;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(draw.x_scale*stretch,1) && draw.x_anchor==320);
    time_state=0;
    draw.depth_test=draw.depth_write=0;
    /* Original damaged-enemy gauge fixture: queued caller33172, two float3
     * quads with a common depth and live 2D billboard program, on the scene
     * texture. The center is already projected; only its width contracts. */
    quad[0][0]=quad[2][0]=-28.5298f;quad[1][0]=quad[3][0]=12.7124f;
    quad[0][1]=quad[1][1]=41.1283f;quad[2][1]=quad[3][1]=82.3705f;
    for(unsigned i=0;i<4;++i)quad[i][2]=44.4867f;
    draw.caller=0x33172;draw.primitive=6;draw.count=4;draw.position_type=0x32;
    draw.shader=0x123001;draw.depth_test=1;draw.default_target=0;
    draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    float gauge_center=320+(-28.5298f+12.7124f)*.5f;
    CHECK(close_to(draw.x_anchor,gauge_center));
    CHECK(close_to(41.2422f*draw.x_scale*stretch,41.2422f));
    CHECK(close_to(gauge_center*draw.x_scale+draw.x_anchor*(1-draw.x_scale),gauge_center));
    /* Neighboring mesh/shader/depth/topology contracts are not billboards. */
    draw.shader=0x777001;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
    billboard_programs[1]=0;
    draw.shader=0;draw.x_scale=1;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
    billboard_programs[1]=0x456001;
    draw.shader=0x123001;draw.depth_write=1;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
    draw.depth_write=0;draw.depth_test=0;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
    draw.depth_test=1;draw.primitive=4;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
    draw.primitive=6;draw.count=3;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
    draw.count=4;quad[3][2]+=1;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
    quad[3][2]-=1;quad[0][2]=NAN;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
    quad[0][2]=44.4867f;quad[3][2]=NAN;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
    quad[3][2]=44.4867f;draw.default_target=1;draw.depth_test=0;
    /* A world marker's center is already projected with the wide matrix;
     * its original pixel radius must shrink without moving that center. */
    draw.caller=0x33811;draw.depth_test=1;draw.shader=0x123001;
    quad[0][0]=quad[2][0]=250;quad[1][0]=quad[3][0]=290;
    quad[0][1]=quad[1][1]=50;quad[2][1]=quad[3][1]=90;
    draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(draw.x_anchor,590));
    CHECK(close_to(40*draw.x_scale*stretch,40));
    CHECK(close_to(590*draw.x_scale+draw.x_anchor*(1-draw.x_scale),590));
    draw.default_target=0;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(draw.x_anchor,590) && close_to(40*draw.x_scale*stretch,40));
    CHECK(close_to(590*draw.x_scale+draw.x_anchor*(1-draw.x_scale),590));
    /* Offscreen HUD sprites are not the world scene's billboard contract. */
    draw.depth_test=0;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(draw.x_scale==1 && draw.x_anchor==0);
    draw.default_target=1;draw.depth_test=1;
    draw.shader=0x777001;draw.x_scale=1;mod->draw(&draw);CHECK(draw.x_scale==1);
    draw.depth_test=draw.depth_write=0;
    draw.caller=0x34a07;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);CHECK(draw.x_scale==1);
    /* Actual automatic Retry: caller33811, live o_noise owner, centered
     * X=-320..320 and six moving 12-pixel strips. Preserve full-width raster
     * mapping across all aspect profiles; ordinary wide UI still contracts. */
    draw.caller=0x33811;draw.position_type=0x32;draw.shader=1;
    quad[0][0]=quad[2][0]=-320;quad[1][0]=quad[3][0]=320;
    quad[0][1]=quad[1][1]=-238;quad[2][1]=quad[3][1]=-226;
    bound_texture=noise_header;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(draw.x_scale==1 && draw.x_anchor==0);
    bound_texture=noise_header+32;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(draw.x_scale*stretch,1) && draw.x_anchor==320);
    /* Reallocated resource headers are resolved live, not cached. */
    noise_slot=37;noise_header=bound_texture;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(draw.x_scale==1 && draw.x_anchor==0);
    noise_slot=1024;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(draw.x_scale*stretch,1) && draw.x_anchor==320);
    noise_slot=37;draw.default_target=0;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(draw.x_scale==1 && draw.x_anchor==0);
    draw.default_target=1;draw.depth_test=1;draw.caller=0x30c0a;
    draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(draw.x_scale*stretch,1) && draw.x_anchor==320);
    draw.depth_test=0;bound_texture=0;
    /* Results replays use the same 3D camera, with the overlay contained in
     * the centered original 4:3 region. Its left/right text is not HUD. */
    phase=10;mod->present();matrix[0]=.75f;
    mod->transform(1,0x39d910,0x2c898);
    CHECK(close_to(matrix[0]*aspect,-matrix[5]));
    mod->present();CHECK(shape[0]*denominator==shape[1]*numerator);
    if(frames)CHECK(!scope);
    draw.caller=0xef2ae;draw.x_scale=1;draw.x_anchor=0;
    quad[0][0]=quad[2][0]=50;quad[1][0]=quad[3][0]=180;
    quad[0][1]=quad[1][1]=30;quad[2][1]=quad[3][1]=80;
    mod->draw(&draw);CHECK(draw.x_anchor==320);
    CHECK(close_to(130*draw.x_scale*stretch,130));
    phase=5;mod->present();CHECK(shape[0]==4 && shape[1]==3);
    draw.caller=0xef2ae;draw.x_scale=1;mod->draw(&draw);CHECK(draw.x_scale==1);
    CHECK(writes==2);
    /* A stage-select screen stays 4:3 at rest. During the actual shutter
     * animation its original UI keeps its proportions inside the wide frame;
     * only the identified cover/glow atlas fills the raster. */
    draw.caller=0x33811;draw.position_type=0x32;draw.count=4;
    draw.depth_test=draw.depth_write=0;draw.default_target=1;
    quad[0][0]=quad[2][0]=-320;quad[1][0]=quad[3][0]=-96;
    quad[0][1]=quad[1][1]=-240;quad[2][1]=quad[3][1]=240;
    for(unsigned control=0;control<3;++control) {
        cover_progress=control==0?.5f:0;cover_held=control==1;cover_dim=control==2?.3f:0;
        mod->present();CHECK(shape[0]*denominator==shape[1]*numerator);
        for(unsigned texture=0;texture<3;++texture) {
            bound_texture=texture==0?cover_header:texture==1?glow_header:label_header;
            draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
            if(texture<2)CHECK(draw.x_scale==1 && draw.x_anchor==0);
            else CHECK(close_to(draw.x_scale*stretch,1) && draw.x_anchor==320);
        }
        /* The same texture on an offscreen producer is not a cover. */
        bound_texture=cover_header;draw.default_target=0;
        draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
        draw.default_target=1;
    }
    /* Original Stage Select lower-right panel: X=183..640, height 220.
     * Its upper-left corner must follow the proportional menu layout so it
     * clears Stage 1, while its right edge still reaches the wide raster.
     * Blend back to the closed pose so no seam opens during a transition. */
    cover_progress=0;cover_held=1;cover_dim=0;mod->present();
    /* Captured outgoing/incoming preview poses must retain one raster map,
     * even while moving or zooming. All nine live resources can be selected. */
    for(unsigned texture=0;texture<9;++texture) {
        bound_texture=0xd0007008+32*texture;
        quad[0][0]=quad[2][0]=-341.6889f;quad[1][0]=quad[3][0]=319.6444f;
        quad[0][1]=quad[1][1]=-248;quad[2][1]=quad[3][1]=248;
        draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
        quad[0][0]=quad[2][0]=-10.6667f;quad[1][0]=quad[3][0]=629.3333f;
        quad[0][1]=quad[1][1]=-240;quad[2][1]=quad[3][1]=240;
        draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);CHECK(draw.x_scale==1 && draw.x_anchor==0);
    }
    bound_texture=0;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(draw.x_scale*stretch,1) && draw.x_anchor==320);
    /* Round word/digit share one anchor inside the stretched panel, keeping
     * their original spacing and glyph widths at every supported aspect. */
    for(unsigned language=0;language<2;++language)for(unsigned digit=0;digit<2;++digit) {
        bound_texture=0xd0007408+32*language;
        quad[0][0]=quad[2][0]=digit?251:159;quad[1][0]=quad[3][0]=digit?275:287;
        quad[0][1]=quad[1][1]=-201;quad[2][1]=quad[3][1]=-169;
        draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
        CHECK(close_to(draw.x_scale*stretch,1) && draw.x_anchor==479);
        CHECK(close_to(479*draw.x_scale+draw.x_anchor*(1-draw.x_scale),479));
    }
    /* The Stage Select heading shares the atlas but belongs to the center. */
    quad[0][0]=quad[2][0]=-257;quad[1][0]=quad[3][0]=-1;
    draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);CHECK(draw.x_anchor==320);
    bound_texture=cover_header;
    quad[0][0]=quad[2][0]=183-320;quad[1][0]=quad[3][0]=640-320;
    for(unsigned pose=0;pose<3;++pose) {
        cover_progress=1-.5f*pose;mod->present();
        float y=260;
        quad[0][1]=quad[1][1]=y-240;quad[2][1]=quad[3][1]=y+220-240;
        draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
        float left=183*draw.x_scale+draw.x_anchor*(1-draw.x_scale);
        float expected=183+(.8f*(320-183))*(1-1/stretch)*(.5f*pose);
        CHECK(close_to(left,expected));
        CHECK(close_to(640*draw.x_scale+draw.x_anchor*(1-draw.x_scale),640));
    }
    bound_texture=glow_header;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(draw.x_scale==1 && draw.x_anchor==0);
    /* Neighboring cover tiles and the gameplay side retain their mapping. */
    bound_texture=cover_header;quad[1][0]-=1;quad[3][0]-=1;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(draw.x_scale==1 && draw.x_anchor==0);quad[1][0]+=1;quad[3][0]+=1;
    quad[0][0]=quad[2][0]=-320;quad[1][0]=quad[3][0]=-96;
    quad[0][1]=quad[1][1]=-240;quad[2][1]=quad[3][1]=240;
    /* Menu-to-gameplay camera setup and changing live resource ownership. */
    cover_progress=.25f;cover_dim=0;cover_held=0;
    matrix[0]=.75f;mod->transform(1,0x39d910,0x2c898);CHECK(writes==3);
    CHECK(close_to(matrix[0]*aspect,-matrix[5]));
    cover_slot=101;cover_header=0xd0005008;bound_texture=cover_header;
    draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);CHECK(draw.x_scale==1);
    cover_slot=1024;draw.x_scale=1;draw.x_anchor=0;mod->draw(&draw);
    CHECK(close_to(draw.x_scale*stretch,1) && draw.x_anchor==320);
    cover_progress=cover_dim=0;cover_held=0;
    mod->present();CHECK(shape[0]==4 && shape[1]==3);
    return 0;
}
