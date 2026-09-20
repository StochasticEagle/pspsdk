add_prx_module(ipl_update exports.exp)

target_sources(ipl_update PRIVATE
    main.c
    imports.S
)

target_compile_options(ipl_update PRIVATE
    -O2
    -G0
    -Wall
    -fno-pic
)

psp_link_kernel_prx(ipl_update
    pspnand_driver
    pspsystemctrl_kernel
)
