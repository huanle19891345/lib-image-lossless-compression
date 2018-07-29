#指定CMake构建版本
cmake_minimum_required(VERSION 3.4.1)



# 配置头文件路径
include_directories("src/main/jni/libjpeg_turbo/libjpeg")

#添加第三方库libjpegbither.so, 命名为jpegbither
add_library(
    jpegbither
    SHARED
    IMPORTED
)

set_target_properties(
   jpegbither
   PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../../jniLibs/${ANDROID_ABI}/libjpegbither.so
)

set(
    CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11"
)


#创建和命名一个库,将其设置为STATIC或SHARED,并提供其源代码的相对路径
#你可以定义多个库,CMake为你构建它,Gradle会自动将共享库和apk包装在一起
add_library(
# Sets the name of the library.
com_baseproject_ndk_ImageLosslessCompress

# STATIC：静态库，是目标文件的归档文件，在链接其它目标的时候使用。
# SHARED：动态库，会被动态链接，在运行时被加载。
# MODULE：模块库，是不会被链接到其它目标中的插件，但是可能会在运行时使用dlopen-系列的函数动态链接。
# Sets the library as a shared library.
SHARED

# Provides a relative path to your source file(s).
# Associated headers in the same location as their source file are automatically included.
com_baseproject_ndk_ImageLosslessCompress.cpp
)


#查找链接库 在指定目录下搜索一个库, 保存在变量log-lib中，如果没有指定路径，则使用默认系统路径
find_library(
# Sets the name of the path variable.
log-lib

# Specifies the name of the NDK library that you want CMake to locate.
log)

# 添加目标库依赖的链接库,相同于指定-l参数,可以链接多个库
target_link_libraries(
  # Specifies the target library.
  com_baseproject_ndk_ImageLosslessCompress

  jpegbither

  # Links the target library to the log library included in the NDK.
  ${log-lib}
  jnigraphics)