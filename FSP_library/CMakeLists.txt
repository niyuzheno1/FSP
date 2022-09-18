cmake_minimum_required (VERSION 3.16)
project(FSP_Library VERSION 1.0)

include(FetchContent)

FetchContent_Declare(
    MMS
    GIT_REPOSITORY https://github.com/niyuzheno1/MMS.git
    GIT_TAG        8f813bc4301449b2fcdb81ba14b181211827e230 # release-1.0.1
)

FetchContent_MakeAvailable(MMS)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

file(GLOB_RECURSE  FSP_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.c
     )
    



add_library(FSP_Library ${FSP_SOURCE_FILES})
target_include_directories(FSP_Library  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(FSP_Library PUBLIC ${MMS_SOURCE_DIR}/export_headers)
target_link_libraries(FSP_Library MMS_Library)