add_prx_module(pspav exports.exp)

target_sources(pspav PRIVATE
    main.c
)

target_compile_options(pspav PRIVATE
    -std=c99
    -O2
    -Os
    -G0
    -Wall
    -fshort-wchar
    -fno-pic
    -mno-check-zero-division
)

psp_link_user_prx(pspav
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
