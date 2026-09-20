project(pspav VERSION 1.0 LANGUAGES C ASM)

add_prx_module(pspav exports.exp)

target_sources(pspav PRIVATE 
   main.c
)

target_compile_options(pspav PRIVATE -std=c99 -O2 -Os -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division)
target_include_directories(pspav PRIVATE ${CFWSDK}/include)
target_link_directories(pspav PRIVATE ${CFWSDK}/libs)
target_link_libraries(pspav PRIVATE
   -nostartfiles
   -lpspav
   psppower
   pspaudio
   pspatrac3
   pspmpeg
   pspsystemctrl_user
   pspsysc_user
)