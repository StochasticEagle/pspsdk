add_prx_module(libpng exports.exp)

target_sources(libpng PRIVATE
    main.c
)

target_compile_options(libpng PRIVATE
    -std=c99
    -O2
    -G0
)

psp_link_user_prx(libpng
    png
    z
    m
    pspsysc_user
    pspnet
    pspnet_apctl
)
