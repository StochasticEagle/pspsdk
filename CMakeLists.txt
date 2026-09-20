cmake_minimum_required(VERSION 3.16)

project(pspcfwsdk VERSION 1.0 LANGUAGES C ASM)

option(PSPSDK_BUILD_CFW_LIBRARIES "Build the integrated CFW SDK libraries" ON)
option(PSPSDK_BUILD_DYNAMIC_MODULES "Build the integrated PRX modules" OFF)

set(SDK_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(SDK_LIBS_DIR "${CMAKE_CURRENT_LIST_DIR}/libs")

if(PSPSDK_BUILD_CFW_LIBRARIES)
    set(STAMP_FILE "${SDK_LIBS_DIR}/.copied.stamp")

    add_custom_command(
        OUTPUT ${STAMP_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_LIST_DIR}/src/pre-built"
            "${SDK_LIBS_DIR}"
        COMMAND ${CMAKE_COMMAND} -E touch "${STAMP_FILE}"
        COMMENT "Copying pre-built SDK libraries"
    )

    add_custom_target(CopyPrebuiltSdkLibs ALL
        DEPENDS ${STAMP_FILE}
    )

    add_subdirectory(src)
    add_subdirectory(build-tools)
endif()

if(PSPSDK_BUILD_DYNAMIC_MODULES)
    if(NOT PSP)
        message(FATAL_ERROR
            "Dynamic PSP modules require the PSP toolchain. Configure with "
            "-DCMAKE_TOOLCHAIN_FILE=$ENV{PSPDEV}/psp/share/pspdev.cmake")
    endif()

    include("${CMAKE_CURRENT_LIST_DIR}/src/base/AddPrxModule.cmake")
    include_directories("${SDK_INCLUDE_DIR}")

    remove_definitions("-D_PSP_FW_VERSION=600")
    add_compile_definitions(_PSP_FW_VERSION=660)

    function(psp_link_kernel_prx target)
        target_link_options(${target} PRIVATE -nostdlib)
        target_link_libraries(${target} PRIVATE
            ${ARGN}
            pspdebug
            pspdisplay_driver
            pspctrl_driver
            pspmodinfo
            pspsdk
            pspkernel
        )
    endfunction()

    function(psp_link_user_prx target)
        target_link_libraries(${target} PRIVATE
            ${ARGN}
            pspdebug
            pspdisplay
            pspge
            pspctrl
        )
    endfunction()

    set(PSPSDK_DYNAMIC_PRX_DIR "${CMAKE_BINARY_DIR}/dynamic")

    function(pspsdk_stage_prx target output_name)
        add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                "${PSPSDK_DYNAMIC_PRX_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:${target}>.prx"
                "${PSPSDK_DYNAMIC_PRX_DIR}/${output_name}.prx"
            COMMENT "Staging ${output_name}.prx"
        )
    endfunction()

    add_subdirectory(src/IOPrivileged)
    add_subdirectory(src/IPL_Updater)
    add_subdirectory(src/KBooti_Updater)
    add_subdirectory(src/LibPNG)
    add_subdirectory(src/PSPAV)
    add_subdirectory(src/PSPFTP)
    add_subdirectory(src/PSPIdentHelper)
endif()
