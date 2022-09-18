cmake_minimum_required (VERSION 3.16)
project(FSP_Library VERSION 1.0)

include(FetchContent)

FetchContent_Declare(
    MMS
    GIT_REPOSITORY https://github.com/niyuzheno1/MMS.git
    GIT_TAG        fe259b9cc5f2ffa79f6800d83c0b0490c8fe58c7 # release-1.0.0
)

FetchContent_MakeAvailable(MMS)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

file(GLOB_RECURSE  FIOU_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.c
     )
    



add_library(FSP_Library ${FIOU_SOURCE_FILES})
target_include_directories(FSP_Library  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
