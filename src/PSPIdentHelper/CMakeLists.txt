project(kpspident VERSION 1.0 LANGUAGES C ASM)

add_prx_module(kpspident exports.exp)

target_sources(kpspident PRIVATE 
   main.c
   imports.S
)

target_compile_options(kpspident PRIVATE -Os -G0 -Wall -fno-pic)
target_include_directories(kpspident PRIVATE ${CFWSDK}/include)
target_link_directories(kpspident PRIVATE ${CFWSDK}/libs)
target_link_libraries(kpspident PRIVATE
   -nostartfiles
   -nostdlib
   pspsdk
   pspmodinfo
   pspkernel
)