cmake_minimum_required(VERSION 3.0)

project(FSP VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(FSP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(FSP_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(FSP_EXPORT_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/export_headers")


file(GLOB_RECURSE  FSP_SOURCE_FILES
     "${FSP_SOURCE_DIR}/*.cpp"
     "${FSP_HEADER_DIR}/*.h"
      "${FSP_EXPORT_HEADER_DIR}/*.h"
     "${FSP_SOURCE_DIR}/*.c"
     )

add_subdirectory(FSP_Library)
list(APPEND EXTRA_LIBS FSP_Library)

add_executable(FileSerializationPrototype ${FSP_SOURCE_FILES})
target_include_directories(FileSerializationPrototype PUBLIC "${FSP_HEADER_DIR}")
target_include_directories(FileSerializationPrototype PUBLIC "${FSP_EXPORT_HEADER_DIR}")
target_link_libraries(FileSerializationPrototype ${EXTRA_LIBS})
target_include_directories(FileSerializationPrototype PUBLIC ${PROJECT_BINARY_DIR}/include)