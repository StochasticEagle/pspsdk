project(kbooti_update VERSION 1.0 LANGUAGES C ASM)

add_prx_module(kbooti_update exports.exp)

target_sources(kbooti_update PRIVATE
   main.c
)

target_compile_options(kbooti_update PRIVATE -Os -G0 -Wall -fno-pic -std=c99)
target_include_directories(kbooti_update PRIVATE ${CFWSDK}/include)
target_link_directories(kbooti_update PRIVATE ${CFWSDK}/libs)
target_link_libraries(kbooti_update PRIVATE
   -nostartfiles
   -nostdlib
   pspsystemctrl_kernel
   pspsdk
   pspmodinfo
   pspkernel
)