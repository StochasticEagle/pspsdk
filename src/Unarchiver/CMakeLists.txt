add_prx_module(unarchive exports.exp)

target_sources(unarchive PRIVATE
    main.c
)

target_compile_options(unarchive PRIVATE
    -std=c99
    -O2
    -Os
    -G0
    -Wall
    -fshort-wchar
    -fno-pic
    -mno-check-zero-division
)

psp_link_user_prx(unarchive
    unarchive_helper
    unarr
    z
    bz2
    lzma
    pspsysc_user
    psprtc
)
