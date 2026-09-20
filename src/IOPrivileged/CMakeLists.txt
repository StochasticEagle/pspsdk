project(iop VERSION 1.0 LANGUAGES C ASM)

add_prx_module(iop exports.exp)

target_sources(iop PRIVATE 
   main.c
)

target_compile_options(iop PRIVATE -O2 -Os -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division)
target_include_directories(iop PRIVATE ${CFWSDK}/include)
target_link_directories(iop PRIVATE ${CFWSDK}/libs)
target_link_libraries(iop PRIVATE
   -nostartfiles
   -nostdlib
   pspsystemctrl_kernel
   pspsdk
   pspmodinfo
   pspkernel
)