cmake_minimum_required(VERSION 3.23)
aux_source_directory(. DIR_LIB_SRCS)
add_library(platform_windows ${DIR_LIB_SRCS})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH $ENV{QT_DIR}/$ENV{QT_VERSION}/mingw73_64/lib/cmake)

include_directories(BEFORE $ENV{QT_DIR}/$ENV{QT_VERSION}/mingw73_64/include/)
include_directories(BEFORE $ENV{QT_DIR}/$ENV{QT_VERSION}/mingw73_64/include/QtGui)
include_directories(BEFORE $ENV{QT_DIR}/$ENV{QT_VERSION}/mingw73_64/include/QtWinExtras)

set(DEBUG_SUFFIX)
if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
    set(DEBUG_SUFFIX "d")
endif ()

add_custom_command(TARGET platform_windows POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_PREFIX_PATH}/../../bin/Qt5WinExtras${DEBUG_SUFFIX}.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>")


find_package(Qt5 COMPONENTS
        WinExtras
        REQUIRED)

target_link_libraries(platform_windows
        Qt5::WinExtras
        )
