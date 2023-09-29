# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.22.1)

# Declares and names the project.

project("prj0")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(include)

add_library( # Sets the name of the library.
        prj0

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp
        Network.cpp
        common.cpp
        decoder.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

add_library(
        avcodec
        SHARED
        IMPORTED
)

add_library(
        avdevice
        SHARED
        IMPORTED
)

add_library(
        avfilter
        SHARED
        IMPORTED
)

add_library(
        avformat
        SHARED
        IMPORTED
)

add_library(
        avutil
        SHARED
        IMPORTED
)

add_library(
        swresample
        SHARED
        IMPORTED
)

add_library(
        swscale
        SHARED
        IMPORTED
)

set(my_lib_path ${CMAKE_SOURCE_DIR}/../jniLibs)

#指定第三方库的绝对路径
set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libavcodec.so)

set_target_properties(avdevice
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libavdevice.so)

set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libavfilter.so)

set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libavformat.so)

set_target_properties(avutil
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libavutil.so)

set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libswresample.so)

set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${my_lib_path}/${ANDROID_ABI}/libswscale.so)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        prj0
        avcodec
        avdevice
        avfilter
        avformat
        avutil
        swresample
        swscale
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})