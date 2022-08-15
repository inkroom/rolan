cmake_minimum_required(VERSION 3.23)
project(Rolan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


if (WIN32)
    set(CMAKE_PREFIX_PATH $ENV{QT_DIR}/$ENV{QT_VERSION}/mingw73_64/lib/cmake)
else ()
    aux_source_directory(unix PLATFORM_SRC)
    set(CMAKE_PREFIX_PATH $ENV{QT_DIR}/$ENV{QT_VERSION}/gcc_64/lib/cmake)
endif ()

include(FetchContent)
FetchContent_Declare(qhotkey GIT_REPOSITORY https://github.com/Skycoder42/QHotkey.git GIT_TAG 1.5.0 SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/QHotkey)
if (NOT EXISTS modules)
    FILE(MAKE_DIRECTORY modules)
endif ()

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

if (WIN32)

    add_subdirectory(windows)

    target_link_libraries(${CMAKE_PROJECT_NAME}
            platform_windows
            )
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()

    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)

else()

endif ()

