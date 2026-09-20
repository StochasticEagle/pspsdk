add_prx_module(iop exports.exp)

target_sources(iop PRIVATE
    main.c
)

target_compile_options(iop PRIVATE
    -O2
    -Os
    -G0
    -Wall
    -fshort-wchar
    -fno-pic
    -mno-check-zero-division
)

psp_link_kernel_prx(iop
    pspsystemctrl_kernel
)
