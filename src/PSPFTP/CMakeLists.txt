add_prx_module(pspftp exports.exp)

target_sources(pspftp PRIVATE
    main.c
)

target_compile_options(pspftp PRIVATE
    -std=c99
    -O2
    -Os
    -G0
    -Wall
    -fshort-wchar
    -fno-pic
    -mno-check-zero-division
)

psp_link_user_prx(pspftp
    -lpspftp
    pspnet_apctl
    pspsystemctrl_user
    pspsysc_user
)
