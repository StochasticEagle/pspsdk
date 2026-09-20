add_prx_module(pspav_prx exports.exp)
set_target_properties(pspav_prx PROPERTIES OUTPUT_NAME pspav)
target_sources(pspav_prx PRIVATE main.c)
target_compile_options(pspav_prx PRIVATE
    -std=c99
    -O2
    -Os
    -G0
    -Wall
    -fshort-wchar
    -fno-pic
    -mno-check-zero-division
)
psp_link_user_prx(pspav_prx
    -lpspav
    pspvram
    psppower
    pspaudio
    pspatrac3
    pspmpeg
    pspmp3
    pspsystemctrl_user
    pspsysc_user
)
pspsdk_stage_prx(pspav_prx pspav)
