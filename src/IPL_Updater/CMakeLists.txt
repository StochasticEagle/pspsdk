add_prx_module(ipl_update_prx exports.exp)
set_target_properties(ipl_update_prx PROPERTIES OUTPUT_NAME ipl_update)
target_sources(ipl_update_prx PRIVATE
    main.c
    imports.S
)
target_compile_options(ipl_update_prx PRIVATE -O2 -G0 -Wall -fno-pic)
psp_link_kernel_prx(ipl_update_prx
    pspnand_driver
    pspsystemctrl_kernel
)
pspsdk_stage_prx(ipl_update_prx ipl_update)
