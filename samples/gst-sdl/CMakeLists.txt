cmake_minimum_required(VERSION 3.1)

include(WebOSPackage)

project(gst-sdl LANGUAGES C)

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(OpenGL REQUIRED IMPORTED_TARGET egl glesv2)
pkg_check_modules(GStreamer REQUIRED IMPORTED_TARGET gstreamer-1.0 gstreamer-app-1.0)
pkg_check_modules(json REQUIRED IMPORTED_TARGET json-c)

add_executable(gst-sdl src/main.c src/gst_sample.c)

target_compile_definitions(gst-sdl PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_include_directories(gst-sdl PRIVATE ${CMAKE_SOURCE_DIR}/utils/include)

target_link_libraries(gst-sdl PRIVATE
 PkgConfig::SDL2 PkgConfig::OpenGL PkgConfig::GStreamer
 NDL_directmedia
 m
)

set_target_properties(gst-sdl PROPERTIES
    WEBOS_APPINFO_ID org.webosbrew.sample.ndl-directmedia.gst-sdl
    WEBOS_APPINFO_VERSION 0.0.1
    WEBOS_APPINFO_VENDOR "Mariotaku Lee"
    WEBOS_APPINFO_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icon.png
    WEBOS_APPINFO_TITLE "NDL+GST+SDL"
    WEBOS_APPINFO_EXTRA "nativeLifeCycleInterfaceVersion=2"
)

target_webos_package(gst-sdl)