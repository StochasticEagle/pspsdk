add_prx_module(kbooti_update exports.exp)

target_sources(kbooti_update PRIVATE
    main.c
)

target_compile_options(kbooti_update PRIVATE
    -Os
    -G0
    -Wall
    -fno-pic
    -std=c99
)

psp_link_kernel_prx(kbooti_update
    pspsystemctrl_kernel
)
