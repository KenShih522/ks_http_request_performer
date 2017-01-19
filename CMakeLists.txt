cmake_minimum_required(VERSION 2.8)

project(KS_HTTP_REQUEST_PERFORMER)

set(CMAKE_MODULE_PATH "${KS_HTTP_REQUEST_PERFORMER_SOURCE_DIR}/cmake")

file(
    GLOB_RECURSE
    SRC_FILES
    src/*
    sample/main.cpp
)

set(KS_HTTP_REQUEST_PERFORMER_OUTPUT_NAME "ks_http_request_performer")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/sample)

include_directories("3rd/inc")
include_directories("src")
link_directories("3rd/lib")

add_executable(sample sample/main.cpp)
add_library(${KS_HTTP_REQUEST_PERFORMER_OUTPUT_NAME} STATIC ${SRC_FILES})
target_link_libraries(sample ${KS_HTTP_REQUEST_PERFORMER_OUTPUT_NAME} ssl crypto boost)
