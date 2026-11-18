/* Exercise the actual title registry over an isolated writable image. */
#include "title_mod.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"failed line %d: %s\n",__LINE__,#c);return 1;}}while(0)
#define QUAD 0x1db6b0u
static unsigned char image[144],before[144];
static unsigned reads,writes,locked;
static const unsigned offsets[]={28,64,100,136};
static void put(unsigned at,uint32_t value){memcpy(image+at,&value,4);}
static int read_data(uint32_t address,void *data,uint32_t bytes)
{
    if(address<QUAD || address-QUAD>sizeof(image) || bytes>sizeof(image)-(address-QUAD))return 0;
    ++reads;memcpy(data,image+address-QUAD,bytes);return 1;
}
static int write_data(uint32_t address,const void *data,uint32_t bytes)
{
    if(locked || address<QUAD || address-QUAD>sizeof(image) || bytes>sizeof(image)-(address-QUAD))return 0;
    ++writes;memcpy(image+address-QUAD,data,bytes);return 1;
}
static void selection(const char *key,const char *value)
{
#ifdef _WIN32
    _putenv_s(key,value?value:"");
#else
    if(value)setenv(key,value,1);else unsetenv(key);
#endif
}
int main(int argc,char **argv)
{
    const char *mode=argc>1?argv[1]:"default";
    struct PilotTitleModHost host={PILOT_TITLE_MOD_ABI,
        "e6b7cd8a5527dd001e05bb3e881d9b24ffc58824ff67def394215ddbc8009249",read_data,write_data};
    selection("XLLE_PILOT_MOD_PLAN",NULL);selection("BLINX_MOD_PLAN",NULL);
    selection("XLLE_PILOT_TITLE_MOD_NO_TIME_LIMIT","0");
    selection("XLLE_PILOT_TITLE_MOD_MORE_STOCK","0");
    selection("XLLE_PILOT_TITLE_MOD_INFINITE_TIME_CONTROLS","0");
    selection("XLLE_PILOT_TITLE_MOD_BLUR_REMOVAL",!strcmp(mode,"default")?NULL:!strcmp(mode,"off")?"0":"1");
    memset(image,0x5a,sizeof(image));
    for(unsigned i=0;i<4;++i)put(offsets[i],i<2?0xbf800000:0x441fc000);
    memcpy(before,image,sizeof(image));
    if(!strcmp(mode,"invalid"))selection("XLLE_PILOT_TITLE_MOD_BLUR_REMOVAL","yes");
    if(!strcmp(mode,"identity"))host.xbe_sha256="wrong";
    if(!strcmp(mode,"guard"))put(offsets[3],0);
    if(!strcmp(mode,"locked"))locked=1;
    if(!strcmp(mode,"no-writer"))host.write_data=NULL;
    const struct PilotTitleMod *mod=pilot_title_mod_open(&host);
    CHECK(!mod); /* This initialization-only mod adds no per-frame callback. */
    if(!strcmp(mode,"off")){CHECK(!reads && !writes && !memcmp(image,before,sizeof(image)));}
    else {
        CHECK(reads==4 && writes==4);
        for(unsigned i=0;i<4;++i){uint32_t value=i<2?0:0x44200000;memcpy(before+offsets[i],&value,4);}
        CHECK(!memcmp(image,before,sizeof(image))); /* All other fields intact. */
    }
    puts("PASS Blinx scene-copy coordinates");return 0;
}
