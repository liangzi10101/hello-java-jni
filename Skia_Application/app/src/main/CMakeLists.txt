cmake_minimum_required(VERSION 3.4.1)

set(root .)

include_directories(native-lib PUBLIC
        ${root}/cpp/static_lib
        )

add_library(
        # 设置so文件名称.
        native-lib

        # 设置这个so文件为动态库(SHARED)。静态库使用STATIC
        SHARED

        # c/c++源文件的相对路径（相对于CMakeLists.txt）
        ${root}/bindings/native-lib.cpp)

add_library(math-lib STATIC
        ${root}/cpp/static_lib/math_add.cpp
        )

target_link_libraries(native-lib PUBLIC
        math-lib
        )

