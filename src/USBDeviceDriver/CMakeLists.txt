add_prx_module(usbdevice_prx exports.exp)
set_target_properties(usbdevice_prx PROPERTIES OUTPUT_NAME usbdevice)
target_sources(usbdevice_prx PRIVATE main.c)
target_compile_options(usbdevice_prx PRIVATE
    -Os
    -G0
    -Wall
    -fshort-wchar
    -fno-pic
    -mno-check-zero-division
)
psp_link_kernel_prx(usbdevice_prx
    pspsystemctrl_kernel
    pspumd_kernel
    pspkubridge
)
pspsdk_stage_prx(usbdevice_prx usbdevice)
