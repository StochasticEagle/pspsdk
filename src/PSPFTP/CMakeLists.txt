add_prx_module(pspftp_prx exports.exp)
set_target_properties(pspftp_prx PROPERTIES OUTPUT_NAME pspftp)
target_sources(pspftp_prx PRIVATE main.c)
target_compile_options(pspftp_prx PRIVATE
    -std=c99
    -O2
    -Os
    -G0
    -Wall
    -fshort-wchar
    -fno-pic
    -mno-check-zero-division
)
psp_link_user_prx(pspftp_prx
    -lpspftp
    pspnet_apctl
    pspsystemctrl_user
    pspsysc_user
)
pspsdk_stage_prx(pspftp_prx pspftp)
