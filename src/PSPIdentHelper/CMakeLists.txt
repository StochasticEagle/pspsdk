add_prx_module(kpspident exports.exp)

target_sources(kpspident PRIVATE
    main.c
)

target_compile_options(kpspident PRIVATE
    -Os
    -G0
    -Wall
    -fno-pic
)

psp_link_kernel_prx(kpspident)
