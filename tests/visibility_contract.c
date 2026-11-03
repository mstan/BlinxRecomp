#include "title_mod.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"failed line %d: %s\n",__LINE__,#c);exit(1);}}while(0)
static unsigned projection_seen=1,frame_aspect[2]={3440,1387},far_clip,lights,reads;
static float view[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
static float projection[16]={.86f,0,0,0,0,-1.6f,0,0,0,0,1,1,0,0,-1,0};
static float lit[4];
static FILE *trace;
static void fail(const char *message){fprintf(stderr,"%s\n",message);exit(1);}
static int scene_mode(void){return 1;}
static void read_data(unsigned a,void *data,unsigned n)
{
    unsigned index=0;float scale=1;++reads;
    if(a==0x39d8d0 && n==64)memcpy(data,view,n);
    else if(a==0x39d910 && n==64)memcpy(data,projection,n);
    else if(a==0x9dda40 && n==4)memcpy(data,&index,n);
    else if(a==0x6678e0 && n==4)memcpy(data,&scale,n);
    else if(a==0x39d978 && n==4)memcpy(data,&far_clip,n);
    else fail("unexpected visibility input");
}
static int invoke(unsigned id,unsigned object,const unsigned *args,unsigned count,unsigned *result)
{CHECK(id==11 && object==0 && count==4);memcpy(lit,args,16);++lights;*result=0;return 1;}
static const struct PilotTitleModHost api={.invoke=invoke};
static const struct PilotTitleModHost *host=&api;
#include "../src/mods/visibility.inc"
static unsigned sphere(float x,float y,float z,float radius,unsigned original)
{
    float input[4]={x,y,z,radius};struct PilotTitleModCall call={0};
    call.id=10;call.phase=1;call.gpr[0]=original;memcpy(call.arguments,input,16);
    CHECK(!visibility_function(&call));CHECK(!memcmp(call.arguments,input,16));return call.gpr[0];
}
int main(void)
{
    /* Original margin: x <= z/.86 + 1.2*r = 128.28. The actual plane
     * intersects this sphere at x=130, so it must survive at either edge. */
    CHECK(sphere(130,0,100,10,0)==1 && lights==1 && lit[0]==130 && lit[3]==10);
    CHECK(sphere(-130,0,100,10,0)==1 && lights==2);
    CHECK(!sphere(150,0,100,10,0) && lights==2);
    CHECK(!sphere(0,100,100,10,0) && !sphere(0,0,-20,10,0));
    far_clip=1;CHECK(!sphere(0,0,420,10,0));far_clip=0;
    CHECK(sphere(0,0,420,10,0)==1);
    unsigned before=reads;CHECK(sphere(10000,0,100,10,1)==1 && reads==before);
    frame_aspect[0]=4;frame_aspect[1]=3;
    CHECK(!sphere(130,0,100,10,0) && reads==before);
    frame_aspect[0]=32;frame_aspect[1]=9;projection[0]=.6f;
    CHECK(sphere(181,0,100,10,0)==1 && !sphere(200,0,100,10,0));
    /* Rotate the same world-space bound around the camera: every frame uses
     * the current model-view matrix, rather than remembered visibility. */
    for(unsigned i=0;i<8;++i) {
        float a=i*.78539816339f,c=cosf(a),s=sinf(a);
        view[0]=c;view[2]=-s;view[8]=s;view[10]=c;
        CHECK(sphere(c*181-s*100,0,s*181+c*100,10,0)==1);
    }
    puts("PASS: ultrawide sphere edges, camera rotation, outside/behind/far rejection, original results and native aspect");return 0;
}
