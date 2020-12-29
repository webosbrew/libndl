cmake_minimum_required(VERSION 3.1)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/utils/cmake)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(WebOSPackage)

add_subdirectory(ndl)

project(sample LANGUAGES C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GStreamer REQUIRED IMPORTED_TARGET gstreamer-1.0 gstreamer-app-1.0)

find_package(Qt5 REQUIRED COMPONENTS Core Quick REQUIRED)
set(BASE_SRC
    "src/main.cpp"
    "src/backend.cpp"
)

set(QRC_SRC
    "src/qml.qrc"
)

add_executable(sample ${BASE_SRC} ${QRC_SRC})

target_compile_definitions(sample PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_include_directories(sample PRIVATE ${CMAKE_SOURCE_DIR}/utils/include)

set_target_properties(sample PROPERTIES
    CXX_STANDARD                11
    CXX_STANDARD_REQUIRED       ON
    AUTOMOC                     ON
    AUTORCC                     ON
)

target_link_libraries(sample PRIVATE
 Qt5::Core Qt5::Quick # QT libraries
 PkgConfig::GStreamer
 NDL_directmedia
)

set_target_properties(sample PROPERTIES
    WEBOS_APPINFO_ID org.webosbrew.sample.ndl-directmedia
    WEBOS_APPINFO_VERSION 0.0.1
    WEBOS_APPINFO_VENDOR "Mariotaku Lee"
    WEBOS_APPINFO_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icon.png
    WEBOS_APPINFO_TITLE "NDL Sample"
    WEBOS_APPINFO_EXTRA "nativeLifeCycleInterfaceVersion=2"
    WEBOS_PACKAGE_ASSETS "assets"
)

target_webos_package(sample)

add_custom_target(webos-install-sample
    COMMAND ares-install -d hometv ${CMAKE_CURRENT_BINARY_DIR}/org.webosbrew.sample.ndl-directmedia_0.0.1_arm.ipk
    DEPENDS webos-package-sample
)