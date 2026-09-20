project(idsregeneration VERSION 1.0 LANGUAGES C ASM)

add_prx_module(idsregeneration exports.exp)

target_sources(idsregeneration PRIVATE 
   main.c
   src/idsregeneration.c
   src/aes_core.c
)

target_compile_options(idsregeneration PRIVATE -O2 -Os -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division)
target_include_directories(idsregeneration PRIVATE include ${CFWSDK}/include)
target_link_directories(idsregeneration PRIVATE ${CFWSDK}/libs)
target_link_libraries(idsregeneration PRIVATE
   -nostartfiles
   -nostdlib
   pspsystemctrl_kernel
   pspdisplay_driver
   pspnand_driver
   psplflash_fatfmt
   pspsdk
   pspmodinfo
   pspkernel
)