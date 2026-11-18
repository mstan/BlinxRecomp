/* The original scene-copy quad at 1DB6B0 has two pixel-coordinate texture
 * pairs per vertex (stride 36). Its second pair samples X one pixel left of
 * the first: -1/639 instead of 0/640. Align the pairs before guest execution;
 * 9D9CB submits this same quad on every frame. Position, Y, colour, shaders
 * and other effect quads retain their original data and execution.
 * These are writable, preloaded .data fields in the hash-bound NTSC-U image.
 * No media or generated CPU instructions are patched. */
static void blur_removal_configure(const struct PilotTitleModHost *api)
{
    const char *selection=getenv("XLLE_PILOT_TITLE_MOD_BLUR_REMOVAL");
    if(selection && strcmp(selection,"0") && strcmp(selection,"1"))
        fail("invalid Blinx blur-removal selection");
    if(selection && !strcmp(selection,"0"))return;
    if(!api || api->abi!=PILOT_TITLE_MOD_ABI || !api->xbe_sha256 ||
       strcmp(api->xbe_sha256,BLINX_SHA) || !api->read || !api->write_data)
        fail("blur-removal title identity mismatch");
    static const uint32_t addresses[]={0x1db6cc,0x1db6f0,0x1db714,0x1db738};
    static const uint32_t original[]={0xbf800000,0xbf800000,0x441fc000,0x441fc000};
    static const uint32_t aligned[]={0,0,0x44200000,0x44200000};
    /* Validate the complete selection before changing any field. A different
     * image or unexpected prior modification must never be silently patched. */
    for(unsigned i=0;i<4;++i) {
        uint32_t value;
        if(!api->read(addresses[i],&value,4) || value!=original[i])
            fail("Blinx scene-copy coordinates differ");
    }
    for(unsigned i=0;i<4;++i)
        if(!api->write_data(addresses[i],&aligned[i],4))
            fail("Blinx scene-copy coordinates are not writable");
    fprintf(stderr,"[MOD] blinx.blur-removal=1 scene-copy samples aligned original-image=unchanged\n");
}
