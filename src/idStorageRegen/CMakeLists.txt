add_prx_module(idsregeneration_prx exports.exp)
set_target_properties(idsregeneration_prx PROPERTIES OUTPUT_NAME idsregeneration)
target_sources(idsregeneration_prx PRIVATE
    main.c
    src/idsregeneration.c
    src/aes_core.c
)
target_compile_options(idsregeneration_prx PRIVATE
    -O2
    -Os
    -G0
    -Wall
    -fshort-wchar
    -fno-pic
    -mno-check-zero-division
)
target_include_directories(idsregeneration_prx PRIVATE include)
target_link_options(idsregeneration_prx PRIVATE -nodefaultlibs)
psp_link_kernel_prx(idsregeneration_prx
    pspsystemctrl_kernel
    pspdisplay_driver
    pspnand_driver
    psplflash_fatfmt
)
pspsdk_stage_prx(idsregeneration_prx idsregeneration)
