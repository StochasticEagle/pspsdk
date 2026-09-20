add_prx_module(usbdevice exports.exp)

target_sources(usbdevice PRIVATE
    main.c
)

target_compile_options(usbdevice PRIVATE
    -Os
    -G0
    -Wall
    -fshort-wchar
    -fno-pic
    -mno-check-zero-division
)

psp_link_kernel_prx(usbdevice
    pspsystemctrl_kernel
    pspumd_kernel
    pspkubridge
)
