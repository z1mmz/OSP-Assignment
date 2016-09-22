cmake_minimum_required(VERSION 3.6)
project(s3495671_osp_2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp external.c external.h central.c central.h)
add_executable(s3495671_osp_2 ${SOURCE_FILES})