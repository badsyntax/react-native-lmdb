project(Lmdb)
cmake_minimum_required(VERSION 3.4.1)

set (PACKAGE_NAME "react-native-lmdb")
set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 17)

add_library(cpp
        SHARED
        ../cpp/react-native-lmdb.cpp
        ../lmdb/libraries/liblmdb/mdb.c
        ../lmdb/libraries/liblmdb/midl.c
        cpp-adapter.cpp
)

# Specifies a path to native header files.
include_directories(
        ../cpp
)

include_directories(
        ../lmdb/libraries/liblmdb
)
