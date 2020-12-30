cmake_minimum_required (VERSION 3.0)

# Add folder where are supportive functions
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/utils/cmake)

project(NDL C)

add_library(NDL_directmedia SHARED IMPORTED GLOBAL)
target_link_libraries(NDL_directmedia LINK_PUBLIC ${lib1_location})

set_target_properties(NDL_directmedia PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES      ${CMAKE_CURRENT_SOURCE_DIR}/include
    IMPORTED_LOCATION                  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libNDL_directmedia.so.1
)

add_subdirectory(samples)