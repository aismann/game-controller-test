cmake_minimum_required(VERSION 3.10)

set(APP_NAME game-controller-test)


# "too large PDB" error often occurs in cpp-tests when using default "/Zi"
if (MSVC)
    string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()

project(${APP_NAME})

if(NOT DEFINED BUILD_ENGINE_DONE)
    if(XCODE)
        set(CMAKE_XCODE_GENERATE_TOP_LEVEL_PROJECT_ONLY TRUE)
    endif()

    set(_AX_ROOT "$ENV{AX_ROOT}")
    if(NOT (_AX_ROOT STREQUAL ""))
        set(_AX_ROOT_PATH "${_AX_ROOT}")
        file(TO_CMAKE_PATH ${_AX_ROOT_PATH} _AX_ROOT_PATH) # string(REPLACE "\\" "/" _AX_ROOT_PATH ${_AX_ROOT_PATH})
        message(STATUS "Using system env var _AX_ROOT=${_AX_ROOT}")
    else()
        set(_AX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)
    endif()

    set(CMAKE_MODULE_PATH ${_AX_ROOT_PATH}/cmake/Modules/)

    include(AXBuildSet)
    add_subdirectory(${_AX_ROOT_PATH}/core ${ENGINE_BINARY_PATH}/axys/core)
endif()

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR VS)
    ax_mark_multi_resources(cc_common_res RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

if(ANDROID)
    list(APPEND GAME_HEADER
         Classes/JNITest/JNITest.h
         )
    list(APPEND GAME_SOURCE
         Classes/JNITest/JNITest.cpp
         proj.android/app/jni/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         ${cc_common_res}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios/Classes/testsAppDelegate.h
             proj.ios/Classes/RootViewController.h
             )

        if (TVOS)
            set(APP_UI_RES
                proj.ios/LaunchScreenBackground.png
                proj.ios/targets/tvos/LaunchScreen.storyboard
                proj.ios/targets/tvos/Images.xcassets
                )
        else()
            set(APP_UI_RES
                proj.ios/LaunchScreenBackground.png
                proj.ios/targets/ios/LaunchScreen.storyboard
                proj.ios/targets/ios/Images.xcassets
                )
        endif()

        list(APPEND GAME_SOURCE
             proj.ios/main.m
             proj.ios/Classes/testsAppDelegate.mm
             proj.ios/Classes/RootViewController.mm
             proj.ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.mac/Icon.icns
            proj.mac/Info.plist
            proj.mac/Prefix.pch
            proj.mac/en.lproj/InfoPlist.strings
            )
        list(APPEND GAME_SOURCE
             proj.mac/main.cpp
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${cc_common_res})
endif()

list(APPEND GAME_HEADER
	Classes/AppDelegate.h
	Classes/AppMacros.h
	Classes/GameControllerTest.h
    )

list(APPEND GAME_SOURCE
	Classes/AppDelegate.cpp
	Classes/GameControllerTest.cpp
	)

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )

if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})

    # whole archive for jni
    add_subdirectory(${_AX_ROOT_PATH}/core/platform/android ${ENGINE_BINARY_PATH}/core/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)

    config_android_shared_libs("org.axys1.lib" "${CMAKE_CURRENT_SOURCE_DIR}/proj.android/app/src")
endif()

target_link_libraries(${APP_NAME} ${_AX_CORE_LIB})

target_include_directories(${APP_NAME}
        PRIVATE Classes
)

if(WIN64)
    target_link_options(${APP_NAME} PRIVATE "/STACK:4194304")
endif()

# mark app resources
setup_ax_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    set_xcode_property(${APP_NAME} INSTALL_PATH "\$(LOCAL_APPS_DIR)")
    set_xcode_property(${APP_NAME} PRODUCT_BUNDLE_IDENTIFIER "org.axys1.cpp-tests")

    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.mac/Info.plist")
    elseif(TVOS)
        set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios/targets/tvos/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "Brand Assets")
    elseif(IOS)
        set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios/targets/ios/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()

    # For code-signing, set the DEVELOPMENT_TEAM:
    #set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "GRLXXXX2K9")
elseif(WINDOWS)
    ax_copy_target_dll(${APP_NAME})
endif()

if((WINDOWS AND (CMAKE_GENERATOR STREQUAL "Ninja")) OR LINUX)
    ax_get_resource_path(APP_RES_DIR ${APP_NAME})
    ax_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
elseif(WINDOWS)
    set_property(TARGET ${APP_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${GAME_RES_FOLDER}")
endif()

if(WINDOWS)
    # precompiled header. Compilation time speedup ~4x, but don't know how Ninja to support PCH yet and needs help from someone.
    if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
        target_sources(${APP_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Classes/precheader.cpp")
        set_target_properties(${APP_NAME} PROPERTIES COMPILE_FLAGS "/Yuprecheader.h /FIprecheader.h")
        set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/Classes/precheader.cpp" PROPERTIES COMPILE_FLAGS "/Ycprecheader.h")
    endif()
endif()
