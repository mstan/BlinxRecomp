/* 34D90's original 1.2 * projected-radius margin encloses the native
 * frustum, but becomes too small as the horizontal projection widens.
 * Keep its matrix/light setup and accepted results. Recheck rejected spheres
 * against the actual camera planes, then run the original light selector. */
enum {VISIBILITY_SPHERE=10,VISIBILITY_LIGHTS=11};
static unsigned visibility_trace_count;
static int visibility_function(struct PilotTitleModCall *call)
{
    if(call->id!=VISIBILITY_SPHERE || !call->phase || call->gpr[0] ||
       !projection_seen || !scene_mode() ||
       (unsigned long long)frame_aspect[0]*3<=(unsigned long long)frame_aspect[1]*4)return 0;
    float input[4],view[16],projection[16],scale;
    unsigned index,far_clip;
    memcpy(input,call->arguments,sizeof(input));
    read_data(0x39d8d0,view,sizeof(view));read_data(0x39d910,projection,sizeof(projection));
    read_data(0x9dda40,&index,4);if(index>=128)fail("unexpected Blinx model matrix index");
    read_data(0x6678e0+index*4,&scale,4);read_data(0x39d978,&far_clip,4);
    float center[3];
    for(unsigned i=0;i<3;++i)
        center[i]=(float)((double)input[0]*view[i]+(double)input[1]*view[4+i]+
                         (double)input[2]*view[8+i]+view[12+i]);
    float radius=input[3]*scale;
    if(!isfinite(radius) || radius<=0 || !isfinite(center[2]) ||
       center[2]+radius<=0 || (far_clip && center[2]-radius>=400))return 0;
    /* A sphere intersects w +/- p*x when its signed plane distance is at
     * least -r*|normal|. This also handles a near-plane-intersecting sphere;
     * dividing its center by Z would reverse the inequality behind camera. */
    double w=(double)projection[11]*center[2];
    for(unsigned i=0;i<2;++i) {
        double p=projection[i?5:0],distance=w-fabs(p*center[i]);
        double extent=radius*sqrt(p*p+(double)projection[11]*projection[11]);
        if(!isfinite(distance) || !isfinite(extent) || distance < -extent-1e-5)return 0;
    }
    uint32_t args[4],result;
    memcpy(args,center,sizeof(center));memcpy(args+3,&radius,4);
    if(!host->invoke || !host->invoke(VISIBILITY_LIGHTS,0,args,4,&result))
        fail("Blinx original visibility light selector is unavailable");
    call->gpr[0]=1;
    if(trace && visibility_trace_count<32) {
        fprintf(trace,"visibility caller=%08x center=%.9g,%.9g,%.9g radius=%.9g projection=%.9g,%.9g\n",
            call->caller,center[0],center[1],center[2],radius,projection[0],projection[5]);
        ++visibility_trace_count;fflush(trace);
    }
    return 0;
}
