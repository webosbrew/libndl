cmake_minimum_required(VERSION 3.1)

include(WebOSPackage)

project(sample LANGUAGES C)

find_package(PkgConfig REQUIRED)

pkg_check_modules(Wayland REQUIRED IMPORTED_TARGET wayland-webos-client wayland-egl)
pkg_check_modules(OpenGL REQUIRED IMPORTED_TARGET egl glesv2)
pkg_check_modules(GStreamer REQUIRED IMPORTED_TARGET gstreamer-1.0 gstreamer-app-1.0)
pkg_check_modules(json REQUIRED IMPORTED_TARGET json-c)

add_executable(sample src/main.c src/gst_sample.c)

target_compile_definitions(sample PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_include_directories(sample PRIVATE ${CMAKE_SOURCE_DIR}/utils/include)

target_link_libraries(sample PRIVATE
 PkgConfig::Wayland PkgConfig::OpenGL PkgConfig::GStreamer
 NDL_directmedia
 PkgConfig::json
)

set_target_properties(sample PROPERTIES
    WEBOS_APPINFO_ID org.webosbrew.sample.ndl-directmedia
    WEBOS_APPINFO_VERSION 0.0.1
    WEBOS_APPINFO_VENDOR "Mariotaku Lee"
    WEBOS_APPINFO_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icon.png
    WEBOS_APPINFO_TITLE "NDL Sample"
    WEBOS_APPINFO_EXTRA "nativeLifeCycleInterfaceVersion=2"
)

target_webos_package(sample)

add_custom_target(webos-install-sample
    COMMAND ares-install -d hometv ${CMAKE_CURRENT_BINARY_DIR}/org.webosbrew.sample.ndl-directmedia_0.0.1_arm.ipk
    DEPENDS webos-package-sample
)