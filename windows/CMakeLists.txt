cmake_minimum_required(VERSION 3.23)
aux_source_directory(. DIR_LIB_SRCS)
add_library(platform_windows ${DIR_LIB_SRCS})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_STATIC_PATH $ENV{QT_STATIC_DIR})

if (USE_STATIC)
    set(CMAKE_PREFIX_PATH ${QT_STATIC_PATH}/lib/cmake)
    include_directories(BEFORE ${QT_STATIC_PATH}/include/)
    include_directories(BEFORE ${QT_STATIC_PATH}/include/QtGui)
    include_directories(BEFORE ${QT_STATIC_PATH}/include/QtWidgets)
    include_directories(BEFORE ${QT_STATIC_PATH}/include/QtWinExtras)
else ()
    set(CMAKE_PREFIX_PATH $ENV{QT_DIR}/$ENV{QT_VERSION}/mingw73_64/lib/cmake)
    message("window  use static false")
    include_directories(BEFORE $ENV{QT_DIR}/$ENV{QT_VERSION}/mingw73_64/include/)
    include_directories(BEFORE $ENV{QT_DIR}/$ENV{QT_VERSION}/mingw73_64/include/QtGui)
    include_directories(BEFORE $ENV{QT_DIR}/$ENV{QT_VERSION}/mingw73_64/include/QtWidgets)
    include_directories(BEFORE $ENV{QT_DIR}/$ENV{QT_VERSION}/mingw73_64/include/QtWinExtras)
endif ()


set(DEBUG_SUFFIX)
if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
    set(DEBUG_SUFFIX "d")
endif ()
if (USE_STATIC)
else ()
    add_custom_command(TARGET platform_windows POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_PREFIX_PATH}/../../bin/Qt5WinExtras${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
endif ()


find_package(Qt5 COMPONENTS
        Gui
        Widgets
        WinExtras
        REQUIRED)

target_link_libraries(platform_windows
        Qt5::WinExtras
        Qt5::Gui
        Qt5::Widgets
        )
