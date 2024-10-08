cmake_minimum_required(VERSION 3.28)
project(pdf)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加include目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加可执行文件
add_executable(pdfium_app main.cpp
        include/stbi/stb_image.h
        include/stbi/stb_image_resize.h
        include/stbi/stb_image_write.h
)

include(FindPackageHandleStandardArgs)

find_path(PDFium_INCLUDE_DIR
        NAMES "fpdfview.h"
        PATHS "$ENV{PDFium_DIR}"
        PATH_SUFFIXES "include"
)

set(PDFium_VERSION "130.0.6707.0")

if(WIN32)
    find_file(PDFium_LIBRARY
            NAMES "pdfium.dll"
            PATHS "$ENV{PDFium_DIR}"
            PATH_SUFFIXES "bin"
    )

    find_file(PDFium_IMPLIB
            NAMES "pdfium.dll.lib"
            PATHS "$ENV{PDFium_DIR}"
            PATH_SUFFIXES "lib"
    )

    if(NOT PDFium_LIBRARY OR NOT PDFium_IMPLIB)
        message(FATAL_ERROR "Could not find PDFium library or import library.")
    endif()

    if(NOT TARGET pdfium)
        add_library(pdfium SHARED IMPORTED)
        set_target_properties(pdfium
                PROPERTIES
                IMPORTED_LOCATION             "${PDFium_LIBRARY}"
                IMPORTED_IMPLIB               "${PDFium_IMPLIB}"
                INTERFACE_INCLUDE_DIRECTORIES "${PDFium_INCLUDE_DIR};${PDFium_INCLUDE_DIR}/cpp"
        )
    endif()

    find_package_handle_standard_args(PDFium
            REQUIRED_VARS PDFium_LIBRARY PDFium_IMPLIB PDFium_INCLUDE_DIR
            VERSION_VAR PDFium_VERSION
    )
else()
    find_library(PDFium_LIBRARY
            NAMES "pdfium"
            PATHS "$ENV{PDFium_DIR}"
            PATH_SUFFIXES "lib"
    )

    if(NOT PDFium_LIBRARY)
        message(FATAL_ERROR "Could not find PDFium library.")
    endif()

    if(NOT TARGET pdfium)
        add_library(pdfium SHARED IMPORTED
        )
        set_target_properties(pdfium
                PROPERTIES
                IMPORTED_LOCATION             "${PDFium_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${PDFium_INCLUDE_DIR};${PDFium_INCLUDE_DIR}/cpp"
        )
    endif()

    find_package_handle_standard_args(PDFium
            REQUIRED_VARS PDFium_LIBRARY PDFium_INCLUDE_DIR
            VERSION_VAR PDFium_VERSION
    )
endif()

# 链接 PDFium 库
target_link_libraries(pdfium_app pdfium)

#######################################################################
################### libjpeg ###########################################
## 包含头文件
#include_directories("${PROJECT_SOURCE_DIR}/include/libjg")
#
## 链接库文件路径
##link_directories("${PROJECT_SOURCE_DIR}/lib")
#
## 显式链接 libjpeg 库
## 链接库
#target_link_libraries(pdfium_app "${PROJECT_SOURCE_DIR}/lib/libjpeg.lib")
#
## 拷贝动态库到输出目录
#add_custom_command(TARGET pdfium_app POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${PROJECT_SOURCE_DIR}/bin/libjpeg-9.dll"
#        $<TARGET_FILE_DIR:pdfium_app>)
#######################################################################
