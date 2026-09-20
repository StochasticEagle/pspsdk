add_prx_module(libpng_prx exports.exp)
set_target_properties(libpng_prx PROPERTIES OUTPUT_NAME libpng)
target_sources(libpng_prx PRIVATE main.c)
target_compile_options(libpng_prx PRIVATE -std=c99 -O2 -G0)
psp_link_user_prx(libpng_prx
    png
    z
    m
    pspsysc_user
    pspnet
    pspnet_apctl
)
pspsdk_stage_prx(libpng_prx libpng)
