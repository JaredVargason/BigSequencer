cmake_minimum_required(VERSION 3.14.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(BigSequencer
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "BigSequencer VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(BigSequencer
    source/version.h
    source/plugids.h
    source/scales.h
    source/scales.cpp
    source/sequencer.h
    source/sequencer.cpp
    source/notedatagenerator.h
    source/bigsequencercids.h
    source/bigsequencerprocessor.h
    source/bigsequencerprocessor.cpp
    source/bigsequencercontroller.h
    source/bigsequencercontroller.cpp
    source/bigsequencerentry.cpp
    source/bigsequencereditor.h
    source/bigsequencereditor.cpp
    source/sequencerview.h
    source/sequencerview.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(BigSequencer
        PRIVATE
            resource/bigsequencereditor.uidesc
    )
    target_link_libraries(BigSequencer
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(BigSequencer
        RESOURCES
            "resource/bigsequencereditor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (BigSequencer
    RESOURCES
        resource/28B960F2B70D5AE2BD663D5EEBAE6B26_snapshot.png
        resource/28B960F2B70D5AE2BD663D5EEBAE6B26_snapshot_2.0x.png
)

target_link_libraries(BigSequencer
    PRIVATE
        sdk
)

smtg_target_configure_version_file(BigSequencer)

if(SMTG_MAC)
    smtg_target_set_bundle(BigSequencer
        BUNDLE_IDENTIFIER com.vargason.bigsequencer
        COMPANY_NAME "Vargason"
    )
    smtg_target_set_debug_executable(BigSequencer
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(BigSequencer PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT BigSequencer)

        smtg_target_set_debug_executable(BigSequencer
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
