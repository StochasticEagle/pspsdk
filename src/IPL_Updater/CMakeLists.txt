project(ipl_update VERSION 1.0 LANGUAGES C ASM)

add_prx_module(ipl_update exports.exp)

target_sources(ipl_update PRIVATE 
   main.c
   imports.S
)

target_compile_options(ipl_update PRIVATE -O2 -G0 -Wall -fno-pic)
target_include_directories(ipl_update PRIVATE ./ ${CFWSDK}/include)
target_link_directories(ipl_update PRIVATE ${CFWSDK}/libs)
target_link_libraries(ipl_update PRIVATE
   -nostartfiles
   -nostdlib
   pspsystemctrl_kernel
   pspnand_driver
   pspsdk
   pspmodinfo
   pspkernel
)