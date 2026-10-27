/* Contract for the Blinx gameplay cheats over a modeled guest image. Each mode
 * runs in its own process because the registry keeps per-process state. */
#include "title_mod.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"failed line %d: %s\n",__LINE__,#c);return 1;}}while(0)
#define MEMORY 0x700000u
static uint8_t memory[MEMORY];
static unsigned rdata_locked,unlinks;
static uint32_t unlinked;
static int read_data(uint32_t address,void *data,uint32_t bytes)
{if(address>=MEMORY || bytes>MEMORY-address)return 0;memcpy(data,memory+address,bytes);return 1;}
static int write_data(uint32_t address,const void *data,uint32_t bytes)
{
    if(address>=MEMORY || bytes>MEMORY-address)return 0;
    if(address>=0x189e40 && address<0x1d0660)return 0; /* .rdata stays read-only */
    memcpy(memory+address,data,bytes);return 1;
}
static int write_image(uint32_t address,const void *data,uint32_t bytes)
{
    if(rdata_locked || address<0x189e40 || address>=0x1d0660 || (address&4095)+bytes>4096)return 0;
    memcpy(memory+address,data,bytes);return 1;
}
static int invoke(uint32_t id,uint32_t ecx,const uint32_t *args,uint32_t count,uint32_t *result)
{
    if(id!=7 || ecx || count!=1)return 0;
    ++unlinks;unlinked=args[0];*result=0;return 1;
}
static void put32(uint32_t a,uint32_t v){memcpy(memory+a,&v,4);}
static uint32_t get32(uint32_t a){uint32_t v;memcpy(&v,memory+a,4);return v;}
static void putf(uint32_t a,float v){memcpy(memory+a,&v,4);}
static float getf(uint32_t a){float v;memcpy(&v,memory+a,4);return v;}
static int8_t count(void){return (int8_t)memory[0x50a39a];}
static uint8_t kind(unsigned i){return memory[0x50a39b+i*2];}
static uint16_t id(unsigned i){uint16_t v;memcpy(&v,memory+0x50a3b0+i*2,2);return v;}
/* Original-layout record, as C26E0/A60E2 write it. */
static void add(uint8_t k)
{
    unsigned i=(unsigned)count();uint32_t serial=get32(0x62f0a0);
    memory[0x50a39b+i*2]=k;memory[0x50a39c+i*2]=10;memcpy(memory+0x50a3b0+i*2,&serial,2);
    memset(memory+0x50a3c4+i*8,0x11,8);memory[0x50a39a]=(uint8_t)(i+1);put32(0x62f0a0,serial+1);
}
/* The original inline consumer after C2A20 (A80A1-A812D). */
static void remove_slot(unsigned index)
{
    unsigned n=(unsigned)count();
    memmove(memory+0x50a39b+index*2,memory+0x50a39b+(index+1)*2,(n-index-1)*2);
    memmove(memory+0x50a3b0+index*2,memory+0x50a3b0+(index+1)*2,(n-index-1)*2);
    memmove(memory+0x50a3c4+index*8,memory+0x50a3c4+(index+1)*8,(n-index-1)*8);
    memory[0x50a39a]=(uint8_t)(n-1);
}
static const struct PilotTitleMod *mod;
static uint32_t stack[4];
static void call(uint32_t function,uint32_t phase,uint32_t eax)
{
    struct PilotTitleModCall c={0};uint32_t entry[8]={0};
    entry[4]=0xd0001000;entry[6]=stack[1];put32(0x50a5c0,stack[1]);c.id=function;c.phase=phase;c.entry=entry;c.gpr[0]=eax;
    if(mod->function(&c))exit(3); /* Cheats never replace an original. */
}
static void frame(void){call(1,0,0);call(1,1,0);}
static void setenv_value(const char *name,const char *value)
{
#ifdef _WIN32
    _putenv_s(name,value);
#else
    setenv(name,value,1);
#endif
}
static void base(void)
{
    putf(0x1c960c,17989.0f);putf(0x1c9608,17999.0f);
    put32(0x50a57c,9);put32(0x50a5c8,3);put32(0x50a428,0);put32(0x50a424,0xffffffffu);
    put32(0x62f0a0,100);put32(0x50312c,4);put32(0x40d7b0,5);put32(0x50a4ec,2);put32(0x50a480,3);
}
int main(int argc,char **argv)
{
    struct PilotTitleModHost host={PILOT_TITLE_MOD_ABI,
        "e6b7cd8a5527dd001e05bb3e881d9b24ffc58824ff67def394215ddbc8009249",read_data,write_data};
    host.invoke=invoke;host.write_image=write_image;
    const char *mode=argc==2?argv[1]:"";
    setenv_value("BLINX_MOD_PLAN","");setenv_value("XLLE_PILOT_MOD_PLAN","");
    setenv_value("XLLE_PILOT_TITLE_MOD_NO_TIME_LIMIT",!strcmp(mode,"time") || !strcmp(mode,"locked")?"1":"0");
    setenv_value("XLLE_PILOT_TITLE_MOD_MORE_STOCK",!strcmp(mode,"more") || !strcmp(mode,"both")?"1":!strcmp(mode,"invalid")?"2":"0");
    setenv_value("XLLE_PILOT_TITLE_MOD_INFINITE_TIME_CONTROLS",!strcmp(mode,"infinite") || !strcmp(mode,"both")?"1":"0");
    base();
    mod=pilot_title_mod_open(&host);
    if(!strcmp(mode,"off")){CHECK(!mod);return 0;}
    CHECK(mod && mod->abi==PILOT_TITLE_MOD_ABI && mod->function && !mod->transform && !mod->draw && !mod->present);
    if(!strcmp(mode,"locked")){rdata_locked=1;frame();return 0;} /* Must stop: WILL_FAIL. */
    if(!strcmp(mode,"time")) {
        frame();
        CHECK(getf(0x1c960c)>1e38f && getf(0x1c9608)>1e38f);
        frame();CHECK(getf(0x1c960c)>1e38f); /* Idempotent after the first frame. */
        /* Dial sees zero, elapsed readout (after the split) sees the real clock. */
        putf(0x50c2bc,25000.0f);
        call(4,0,0);CHECK(getf(0x50c2bc)==0);
        call(8,0,0);CHECK(getf(0x50c2bc)==25000.0f);
        call(8,1,0);call(4,1,0);CHECK(getf(0x50c2bc)==25000.0f);
        /* Early A18C0 exit (stage 37) skips the split; exit still restores. */
        call(4,0,0);call(4,1,0);CHECK(getf(0x50c2bc)==25000.0f);
        /* Stock is untouched in this mode. */
        add(2);frame();CHECK(count()==1 && get32(0x50a5c8)==3);
        return 0;
    }
    if(!strcmp(mode,"more")) {
        frame();CHECK(get32(0x50a5c8)==10 && getf(0x1c960c)==17989.0f);
        for(uint8_t k=0;k<10;++k)add((uint8_t)(2+k%5));
        uint16_t oldest=id(0);uint8_t oldest_kind=kind(0);
        /* C26E0 at capacity: drop oldest, append new (kind 4). */
        call(2,0,0);remove_slot(0);add(4);call(2,1,0);
        CHECK(count()==10 && id(0)!=oldest);
        /* Activation consumes slot 3; refill waits for the control to finish. */
        put32(0x50a424,3);stack[1]=3;call(3,0,0);remove_slot(3);frame();CHECK(count()==9);
        put32(0x50a424,0xffffffffu);put32(0x50a428,2);frame();CHECK(count()==9);
        put32(0x50a428,0);frame();
        CHECK(count()==10 && kind(9)==oldest_kind && memory[0x50a3c4+9*8]==0xff && id(9)==get32(0x62f0a0)-1);
        /* The reserve holds 10 (20 total) and survives stage initialization. */
        for(unsigned n=0;n<30;++n){call(2,0,0);remove_slot(0);add(5);call(2,1,0);}
        call(5,0,0);call(5,1,0);
        /* Slots are ten REW; the reserve kept the first ten dropped records
         * (3,4,5,2,3,4,5,6,4,2). HUD (A0B90) sees slots plus reserve with each
         * badge limited to 10, so the two reserve REW are omitted: 18 shown. */
        uint8_t saved[42];memcpy(saved,memory+0x50a39a,42);
        call(9,0,0);CHECK(count()==18 && kind(10)==3 && kind(12)==2 && kind(17)==2);call(9,1,0);
        CHECK(!memcmp(saved,memory+0x50a39a,42)); /* IDs overlapped by the view are restored. */
        unsigned refilled=0;
        for(unsigned n=0;n<40;++n){remove_slot(0);frame();if(count()!=10)break;++refilled;}
        CHECK(refilled==10);
        call(9,0,0);CHECK(count()==9);call(9,1,0); /* Empty reserve: HUD untouched. */
        /* No crystal suppression in this mode. */
        memory[0x4f2aa0]=1;memory[0x4f2aa2]=0x5a;call(6,1,0x4f2aa0);CHECK(memory[0x4f2aa0]==1 && !unlinks);
        return 0;
    }
    if(!strcmp(mode,"infinite")) {
        add(3);add(3);frame();
        CHECK(get32(0x50a5c8)==5 && count()==5);
        unsigned seen=0;for(int i=0;i<count();++i)seen|=1u<<kind((unsigned)i);
        CHECK(seen==0x7c);
        /* Activated slot 1 returns at the same position after the inline removal. */
        uint8_t used=kind(1);stack[1]=1;call(3,0,0);put32(0x50a424,2);remove_slot(1);frame();
        CHECK(count()==5 && kind(1)==used);
        /* A full stock that lost a kind evicts the oldest duplicate. */
        memory[0x50a39b]=4;frame();
        seen=0;for(int i=0;i<count();++i)seen|=1u<<kind((unsigned)i);CHECK(seen==0x7c && count()==5);
        /* Non-heart crystals are removed exactly like an original pickup. */
        const uint8_t kinds[]={0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x15};
        const uint8_t removed[]={1,1,1,0,1,1,1,1,1,0,1,1,0,0};
        for(unsigned i=0;i<sizeof(kinds);++i) {
            uint32_t object=0x4f2aa0+i*0x60,live=get32(0x40d7b0),before=unlinks;
            memory[object]=1;memory[object+2]=kinds[i];call(6,1,object);
            CHECK(memory[object]==(removed[i]?0:1) && get32(0x40d7b0)==live-removed[i] &&
                  unlinks==before+removed[i] && (!removed[i] || unlinked==object));
        }
        call(6,1,0);/* Failed allocation stays failed. */
        put32(0x50312c,39);memory[0x4f2aa0]=1;memory[0x4f2aa2]=0x5a;call(6,1,0x4f2aa0);
        CHECK(memory[0x4f2aa0]==1); /* Final boss keeps original crystals. */
        /* Outside gameplay the stock is untouched. */
        put32(0x50a57c,10);remove_slot(0);frame();CHECK(count()==4);
        return 0;
    }
    if(!strcmp(mode,"both")) {
        frame();CHECK(get32(0x50a5c8)==10 && count()==5);
        /* Infinite keeps the used slot; More still keeps overflow. */
        for(uint8_t k=0;k<5;++k)add((uint8_t)(2+k));
        call(2,0,0);remove_slot(0);add(6);call(2,1,0);CHECK(count()==10);
        uint8_t used=kind(4);stack[1]=4;call(3,0,0);put32(0x50a424,4);remove_slot(4);frame();
        CHECK(count()==10 && kind(4)==used);
        put32(0x50a424,0xffffffffu);call(9,0,0);CHECK(count()==11);call(9,1,0);CHECK(count()==10);
        CHECK(get32(0x50a4ec)==2 && get32(0x50a480)==3);
        return 0;
    }
    return 2;
}
