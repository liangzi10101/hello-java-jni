cmake_minimum_required(VERSION 3.4.1)

set(root .)

include_directories(native-lib PUBLIC
        ${root}/static_lib
        ${root}/skia
        )
#link_directories(${root}/../../../libs/armeabi-v7a)
aux_source_directory(${CMAKE_SOURCE_DIR} SRC_LIST)

# =====================================
# math-lib库
add_library(math-lib STATIC
        ${root}/static_lib/math_add.cpp
        )

# =====================================
# skia库
add_library(skia SHARED IMPORTED)
set_target_properties(skia PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/../../../libs/${ANDROID_ABI}/libskia.so
        )

message("================== file src:${SRC_LIST}")

# =====================================
# native-lib库
add_library(
        # 设置so文件名称.
        native-lib

        # 设置这个so文件为动态库(SHARED)。静态库使用STATIC
        SHARED

        # c/c++源文件的相对路径（相对于CMakeLists.txt）
        ${root}/native-lib.cpp)

find_library(opensl OpenSLES)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries(native-lib PUBLIC
        -ljnigraphics
        -landroid
        ${opensl}
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        math-lib
        skia
        )

