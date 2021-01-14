cmake_minimum_required(VERSION 3.1)
project(libndl)

set(PROJECT_NAME "libndl")
set(PROJECT_DESCRIPTION "Native API Development Library for webOS")
set(PROJECT_AUTHOR "Mariotaku Lee")
set(PROJECT_URL "https://github.com/webosbrew/libndl")

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(BIN_INSTALL_DIR bin CACHE STRING "Binary installation directory relative to the install prefix")
set(LIB_INSTALL_DIR lib CACHE STRING "Library installation directory relative to the install prefix")
set(INCLUDE_INSTALL_DIR include CACHE STRING "Header installation directory relative to the install prefix")

set(HEADERS "include/NDL_directmedia.h")

set(SRC "src/NDL_directmedia.c")

add_library(NDL_directmedia ${HEADERS} ${SRC})

set_target_properties(NDL_directmedia PROPERTIES
    PUBLIC_HEADER         ${HEADERS}
    VERSION               ${PROJECT_VERSION}
    SOVERSION             ${PROJECT_VERSION_MAJOR}
)

target_include_directories(NDL_directmedia PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
    "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>"
)

target_link_libraries(NDL_directmedia dl)

install(TARGETS NDL_directmedia
    EXPORT        libndl-export
    RUNTIME       DESTINATION "${BIN_INSTALL_DIR}"
    LIBRARY       DESTINATION "${LIB_INSTALL_DIR}"
    ARCHIVE       DESTINATION "${LIB_INSTALL_DIR}"
    PUBLIC_HEADER DESTINATION "${INCLUDE_INSTALL_DIR}"
)