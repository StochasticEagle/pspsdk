project(usbdevice VERSION 1.0 LANGUAGES C ASM)

add_prx_module(usbdevice exports.exp)

target_sources(usbdevice PRIVATE 
   main.c
)

target_compile_options(usbdevice PRIVATE -Os -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division )
target_include_directories(usbdevice PRIVATE ${CFWSDK}/include)
target_link_directories(usbdevice PRIVATE ${CFWSDK}/libs)
target_link_libraries(usbdevice PRIVATE
   -nostartfiles
   -nostdlib
   pspsystemctrl_kernel
   pspumd_kernel
   pspkubridge
   pspsdk
   pspmodinfo
   pspkernel
)