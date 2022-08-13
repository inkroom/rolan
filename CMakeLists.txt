cmake_minimum_required(VERSION 3.23)
project(Rolan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)



if(WIN32)
    aux_source_directory(windows PLATFORM_SRC)
else()
    aux_source_directory(unix PLATFORM_SRC)
endif()

if (WIN32)
else()
    set(CMAKE_PREFIX_PATH $ENV{QT_DIR}/$ENV{QT_VERSION}/gcc_64/lib/cmake)
endif ()
include(FetchContent)
FetchContent_Declare(qhotkey GIT_REPOSITORY https://github.com/Skycoder42/QHotkey.git GIT_TAG 1.5.0 SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/QHotkey)
if(NOT EXISTS modules)
    FILE(MAKE_DIRECTORY modules)
endif()

FetchContent_MakeAvailable(qhotkey)


#add_subdirectory(QHotkey)

# file(GLOB_RESOURCE QHotkey QHotkey/QHotkey/*)

aux_source_directory(src SRC_DIR)
# aux_source_directory 不识别头文件 只能用FILE，区别就是拿到的是绝对路径
FILE(GLOB_RECURSE HEADER_DIR CONFIGURE_DEPENDS includes/*.h)

include_directories(includes QHotkey/QHotkey)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
qt5_add_resources(QRC_FILES resources/r.qrc)

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${HEADER_DIR} ${SRC_DIR} ${PLATFORM_SRC} ${QRC_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        qhotkey
        )
