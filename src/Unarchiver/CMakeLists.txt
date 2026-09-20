add_prx_module(unarchive_prx exports.exp)
set_target_properties(unarchive_prx PROPERTIES OUTPUT_NAME unarchive)
target_sources(unarchive_prx PRIVATE main.c)
target_compile_options(unarchive_prx PRIVATE
    -std=c99
    -O2
    -Os
    -G0
    -Wall
    -fshort-wchar
    -fno-pic
    -mno-check-zero-division
)
psp_link_user_prx(unarchive_prx
    unarchive_helper
    unarr
    z
    bz2
    lzma
    pspsysc_user
    psprtc
)
pspsdk_stage_prx(unarchive_prx unarchive)
