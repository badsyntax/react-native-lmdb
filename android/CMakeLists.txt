project(Lmdb)
cmake_minimum_required(VERSION 3.9.0)

set (PACKAGE_NAME "react-native-lmdb")
set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 17)
set (BUILD_DIR ${CMAKE_SOURCE_DIR}/build)

include_directories(
        ../cpp
        ../lmdb/libraries/liblmdb
)

add_library(
      ${PACKAGE_NAME}
        SHARED
        ../cpp/react-native-lmdb.cpp
        ../cpp/react-native-lmdb.h
        ../lmdb/libraries/liblmdb/mdb.c
        ../lmdb/libraries/liblmdb/midl.c
        cpp-adapter.cpp
)


set_target_properties(
        ${PACKAGE_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
)

find_package(ReactAndroid REQUIRED CONFIG)
find_package(fbjni REQUIRED CONFIG)
find_library(log-lib log)

target_link_libraries(
        ${PACKAGE_NAME}
        ${log-lib}          # <-- Logcat logger
        ReactAndroid::jsi
        android
)
