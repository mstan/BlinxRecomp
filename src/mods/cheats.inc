/* Authored Blinx gameplay cheats, selected by launcher title switches.
 * Source-bound facts and limits: docs/CHEATS.md. Original CPU bodies stay
 * intact; hooks only adjust reviewed data around their calls. Hearts are never
 * written. The switches are independent and may be combined.
 *   NO_TIME_LIMIT=1            original fail (A6740 -> A2620 at clock>=17989)
 *                              and 17999 clamp never trigger; no countdown.
 *   MORE_STOCK=1               20 held: the original 10 slots plus a 10-entry
 *                              reserve that refills slots; HUD badges count both.
 *   INFINITE_TIME_CONTROLS=1   never consumed, one of each kind always held; no
 *                              non-heart crystals spawn except in the final boss. */
enum {CHEAT_FRAME=1,CHEAT_STOCK_ADD,CHEAT_ACTIVATE,CHEAT_HUD,CHEAT_STAGE_INIT,CHEAT_SPAWN,CHEAT_UNLINK,CHEAT_HUD_SPLIT,
      CHEAT_HUD_STOCK};
#define CHEAT_SLOTS 10u
#define CHEAT_RESERVE 10u
#define CHEAT_BADGE_MAX 10u /* A0B90 badge art holds digits 1-10 */
#define CHEAT_FINAL_BOSS 39u
static unsigned no_time_limit,more_stock,infinite_controls,time_limit_open,cheat_announced;
struct CheatStock {int8_t count;uint8_t rec[CHEAT_SLOTS*2];uint16_t id[CHEAT_SLOTS];uint8_t data[CHEAT_SLOTS*8];};
static struct CheatStock stock_before;
static unsigned stock_before_valid;
static struct {unsigned valid,index,count;uint8_t kind,duration;} used;
static struct {uint8_t kind,duration;} reserve[CHEAT_RESERVE];
static unsigned reserve_head,reserve_length;
static float hud_clock;
static unsigned hud_substituted;
static uint8_t hud_stock[42],hud_stock_saved;

static void cheat_write(uint32_t address,const void *data,uint32_t bytes)
{if(!host->write_data(address,data,bytes))fail("Blinx cheat data is not writable");}
static uint32_t cheat_u32(uint32_t address){uint32_t value;read_data(address,&value,4);return value;}
static unsigned cheat_switch(const char *name)
{
    const char *value=getenv(name);
    if(value && strcmp(value,"0") && strcmp(value,"1"))fail("invalid Blinx cheat selection");
    return value && !strcmp(value,"1");
}
static void cheats_configure(void)
{
    no_time_limit=cheat_switch("XLLE_PILOT_TITLE_MOD_NO_TIME_LIMIT");
    more_stock=cheat_switch("XLLE_PILOT_TITLE_MOD_MORE_STOCK");
    infinite_controls=cheat_switch("XLLE_PILOT_TITLE_MOD_INFINITE_TIME_CONTROLS");
}
static int gameplay(void){return (cheat_u32(0x50a57c)&0x7f)==9;}
static void stock_read(struct CheatStock *s)
{
    read_data(0x50a39a,&s->count,1);read_data(0x50a39b,s->rec,sizeof(s->rec));
    read_data(0x50a3b0,s->id,sizeof(s->id));read_data(0x50a3c4,s->data,sizeof(s->data));
    if(s->count<0 || (unsigned)s->count>CHEAT_SLOTS)fail("unexpected original time-control stock");
}
static void stock_write(const struct CheatStock *s)
{
    cheat_write(0x50a39a,&s->count,1);cheat_write(0x50a39b,s->rec,sizeof(s->rec));
    cheat_write(0x50a3b0,s->id,sizeof(s->id));cheat_write(0x50a3c4,s->data,sizeof(s->data));
}
/* Same record shape as the original stage-start/retry loaders (A60E2): a new
 * serial ID from 62F0A0 and no attached effect handles (-1). */
static void stock_insert(struct CheatStock *s,unsigned index,uint8_t kind,uint8_t duration)
{
    unsigned count=(unsigned)s->count;
    if(count>=CHEAT_SLOTS || index>count || kind<2 || kind>6)fail("invalid time-control stock insertion");
    memmove(s->rec+(index+1)*2,s->rec+index*2,(count-index)*2);
    memmove(s->id+index+1,s->id+index,(count-index)*2);
    memmove(s->data+(index+1)*8,s->data+index*8,(count-index)*8);
    uint32_t serial=cheat_u32(0x62f0a0);
    s->rec[index*2]=kind;s->rec[index*2+1]=duration;s->id[index]=(uint16_t)serial;
    memset(s->data+index*8,0xff,8);s->count=(int8_t)(count+1);
    ++serial;cheat_write(0x62f0a0,&serial,4);
}
static void stock_remove(struct CheatStock *s,unsigned index)
{
    unsigned count=(unsigned)s->count;
    memmove(s->rec+index*2,s->rec+(index+1)*2,(count-index-1)*2);
    memmove(s->id+index,s->id+index+1,(count-index-1)*2);
    memmove(s->data+index*8,s->data+(index+1)*8,(count-index-1)*8);
    s->count=(int8_t)(count-1);
}
/* The reserve lives for the play session; only the original 10 slots are
 * saved. A full reserve loses the record, as the original full stock does. */
static void reserve_push(uint8_t kind,uint8_t duration)
{
    if(kind<2 || kind>6 || reserve_length>=CHEAT_RESERVE)return;
    unsigned at=(reserve_head+reserve_length)%CHEAT_RESERVE;
    reserve[at].kind=kind;reserve[at].duration=duration;++reserve_length;
}
static void cheat_capacity(void)
{
    int32_t capacity=(int32_t)cheat_u32(0x50a5c8),wanted=more_stock?(int32_t)CHEAT_SLOTS:5;
    if(capacity<3 || capacity>(int32_t)CHEAT_SLOTS)fail("unexpected original time-control capacity");
    if(capacity<wanted)cheat_write(0x50a5c8,&wanted,4);
}
/* The original consumer (A6740 after C2A20) removes the used slot inline.
 * Restore the same kind/duration at the same position so selection is kept. */
static void infinite_frame(void)
{
    struct CheatStock s;stock_read(&s);
    unsigned dirty=0,capacity=cheat_u32(0x50a5c8);
    if(used.valid && (unsigned)s.count+1==used.count && used.index<=(unsigned)s.count) {
        stock_insert(&s,used.index,used.kind,used.duration);dirty=1;
    }
    used.valid=0;
    if(cheat_u32(0x50a428)==0) {
        for(uint8_t kind=2;kind<=6;++kind) {
            unsigned held[7]={0},present=0;
            for(int i=0;i<s.count;++i){++held[s.rec[i*2]%7];present|=s.rec[i*2]==kind;}
            if(present)continue;
            if((unsigned)s.count>=capacity) {
                int oldest=-1;
                for(int i=0;i<s.count && oldest<0;++i)if(held[s.rec[i*2]%7]>1)oldest=i;
                if(oldest<0)break;
                stock_remove(&s,(unsigned)oldest);
            }
            stock_insert(&s,(unsigned)s.count,kind,10);dirty=1;
        }
    }
    if(dirty)stock_write(&s);
}
static void more_frame(void)
{
    if(!reserve_length || cheat_u32(0x50a428)!=0 || cheat_u32(0x50a424)!=0xffffffffu)return;
    struct CheatStock s;stock_read(&s);
    unsigned capacity=cheat_u32(0x50a5c8),dirty=0;
    while(reserve_length && (unsigned)s.count<capacity) {
        unsigned at=reserve_head;reserve_head=(reserve_head+1)%CHEAT_RESERVE;--reserve_length;
        stock_insert(&s,(unsigned)s.count,reserve[at].kind,reserve[at].duration);dirty=1;
    }
    if(dirty)stock_write(&s);
}
/* A0B90 draws 10 capacity pips from slots [0,capacity) and per-kind badges by
 * counting [0,count). For that call only, append reserve kinds after the real
 * slots (badge-limited to 10 per kind): pips read as full at 10+, badges show
 * the combined count. The overlapped ID bytes are restored on exit. */
static void hud_stock_restore(void)
{if(hud_stock_saved){cheat_write(0x50a39a,hud_stock,sizeof(hud_stock));hud_stock_saved=0;}}
static void hud_stock_substitute(void)
{
    hud_stock_restore();
    if(!reserve_length)return;
    read_data(0x50a39a,hud_stock,sizeof(hud_stock));
    int8_t count=(int8_t)hud_stock[0];
    if(count<0 || (unsigned)count>CHEAT_SLOTS)fail("unexpected original time-control stock");
    uint8_t view[1+CHEAT_SLOTS*2+CHEAT_RESERVE*2];unsigned held[7]={0},n=(unsigned)count;
    memcpy(view,hud_stock,1+n*2);
    for(unsigned i=0;i<n;++i)++held[view[1+i*2]%7];
    for(unsigned i=0;i<reserve_length;++i) {
        uint8_t kind=reserve[(reserve_head+i)%CHEAT_RESERVE].kind;
        if(held[kind]>=CHEAT_BADGE_MAX)continue;
        ++held[kind];view[1+n*2]=kind;view[2+n*2]=10;++n;
    }
    if(n==(unsigned)count)return;
    view[0]=(uint8_t)n;hud_stock_saved=1;
    cheat_write(0x50a39a,view,1+n*2);
}
static void open_time_limit(void)
{
    /* A6740 alone reads these .rdata limits: 1C960C at A78A1 (fail through
     * A2620) and 1C9608 at A84D3 (clamp). History rings index modulo 30/300,
     * so the clock may continue past 18000 frames. */
    float fail_at,clamp_at;read_data(0x1c960c,&fail_at,4);read_data(0x1c9608,&clamp_at,4);
    const float open=3.0e38f;
    if((fail_at!=17989.0f && fail_at!=open) || (clamp_at!=17999.0f && clamp_at!=open))
        fail("unexpected original Blinx time-limit constants");
    /* .rdata keeps Xbox read-only protection; the host override restores it. */
    if(!host->write_image || !host->write_image(0x1c960c,&open,4) || !host->write_image(0x1c9608,&open,4))
        fail("Blinx time-limit constants are not writable in this build");
    read_data(0x1c960c,&fail_at,4);read_data(0x1c9608,&clamp_at,4);
    if(fail_at!=open || clamp_at!=open)fail("Blinx time-limit constants did not update");
    time_limit_open=1;
}
static void hud_restore(void)
{if(hud_substituted){cheat_write(0x50c2bc,&hud_clock,4);hud_substituted=0;}}
static int cheat_function(struct PilotTitleModCall *call)
{
    switch(call->id) {
    case CHEAT_FRAME:
        if(!call->phase) {
            if(no_time_limit && !time_limit_open)open_time_limit();
            if(!cheat_announced) {
                fprintf(stderr,"[MOD] cheats no-time-limit=%u more-stock=%u infinite-time-controls=%u\n",
                        no_time_limit,more_stock,infinite_controls);
                cheat_announced=1;
            }
            return 0;
        }
        if((more_stock || infinite_controls) && gameplay()) {
            cheat_capacity();
            if(infinite_controls)infinite_frame();
            if(more_stock)more_frame();
        }
        return 0;
    case CHEAT_STOCK_ADD:
        /* C26E0 drops the oldest record when the stock is full. Keep it. */
        if(!more_stock)return 0;
        if(!call->phase){stock_read(&stock_before);stock_before_valid=1;return 0;}
        if(stock_before_valid) {
            struct CheatStock after;stock_read(&after);
            for(int i=0;i<stock_before.count;++i) {
                int kept=0;
                for(int j=0;j<after.count && !kept;++j)kept=after.id[j]==stock_before.id[i];
                if(!kept)reserve_push(stock_before.rec[i*2],stock_before.rec[i*2+1]);
            }
            stock_before_valid=0;
        }
        return 0;
    case CHEAT_ACTIVATE:
        if(infinite_controls && !call->phase) {
            /* The sole caller (A8099) pushes ESI=[50A5C0], the selected slot.
             * Guest stacks live above the 64 MiB window, so use the global. */
            uint32_t index=cheat_u32(0x50a5c0);struct CheatStock s;
            if(index!=call->entry[6])fail("unexpected time-control activation slot");
            stock_read(&s);
            used.valid=index<(unsigned)s.count;
            if(used.valid){used.index=index;used.count=(unsigned)s.count;used.kind=s.rec[index*2];used.duration=s.rec[index*2+1];}
        }
        return 0;
    case CHEAT_HUD:
        /* A18C0 draws the last-31-second countdown (remaining<3A2), then the
         * elapsed readout. Only the countdown sees a zero clock; the split
         * (2A0B0 from A1A63) restores the real one. */
        if(!no_time_limit)return 0;
        if(!call->phase) {
            const float zero=0;hud_restore();read_data(0x50c2bc,&hud_clock,4);
            cheat_write(0x50c2bc,&zero,4);hud_substituted=1;
        }
        else hud_restore();
        return 0;
    case CHEAT_HUD_SPLIT:
        if(!call->phase)hud_restore();
        return 0;
    case CHEAT_HUD_STOCK:
        if(more_stock) {
            if(!call->phase)hud_stock_substitute();else hud_stock_restore();
        }
        return 0;
    case CHEAT_STAGE_INIT:
        if(call->phase && (more_stock || infinite_controls)) {
            used.valid=0;stock_before_valid=0;
            if(gameplay())cheat_capacity();
        }
        return 0;
    case CHEAT_SPAWN:
        if(call->phase && infinite_controls && call->gpr[0] && cheat_u32(0x50312c)!=CHEAT_FINAL_BOSS) {
            uint32_t object=call->gpr[0];uint8_t header[3];
            read_data(object,header,3);
            /* Kinds 59-64 are C3890 pickups, color (kind-59)%6; color 3 is
             * the heart. Remove others as the original pickup does. */
            if(header[0]==1 && header[2]>=0x59 && header[2]<=0x64 && (header[2]-0x59)%6!=3) {
                const uint8_t inactive=0;uint32_t live=cheat_u32(0x40d7b0)-1,result;
                cheat_write(object,&inactive,1);cheat_write(0x40d7b0,&live,4);
                if(!host->invoke(CHEAT_UNLINK,0,&object,1,&result))fail("crystal unlink original is unavailable");
            }
        }
        return 0;
    default:
        return 0;
    }
}
