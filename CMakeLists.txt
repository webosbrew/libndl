cmake_minimum_required(VERSION 3.1)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(ndl)

project(sample LANGUAGES C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GStreamer REQUIRED IMPORTED_TARGET gstreamer-1.0 gstreamer-app-1.0)

find_package(Qt5 REQUIRED COMPONENTS Core Quick REQUIRED)
find_package(Threads REQUIRED)

set(EXECUTABLE_NAME "sample")

set(BASE_SRC
    "src/main.cpp"
    "src/backend.cpp"
)

set(QRC_SRC
    "src/qml.qrc"
)

add_executable(${EXECUTABLE_NAME} ${BASE_SRC} ${QRC_SRC})

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    CXX_STANDARD                11
    CXX_STANDARD_REQUIRED       ON
    AUTOMOC                     ON
    AUTORCC                     ON
    RUNTIME_OUTPUT_DIRECTORY    ${CMAKE_BINARY_DIR}/pkg_$ENV{ARCH}/
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
 Qt5::Core Qt5::Quick # QT libraries
 PkgConfig::GStreamer
 NDL_directmedia
)

add_custom_target("webos-package" COMMAND ${CMAKE_SOURCE_DIR}/scripts/package.sh ${CMAKE_SOURCE_DIR}
    DEPENDS ${EXECUTABLE_NAME}
)

add_custom_target("device-install" COMMAND ${CMAKE_SOURCE_DIR}/scripts/device-install.sh ${CMAKE_SOURCE_DIR}
    DEPENDS "webos-package"
)

add_custom_target("device-launch" COMMAND CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/scripts/device-launch.sh)