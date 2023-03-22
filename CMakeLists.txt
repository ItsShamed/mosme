cmake_minimum_required(VERSION 3.5)
project(mosme)

include(cmake/get_cpm.cmake)

CPMAddPackage("gh:TheLartians/Ccache.cmake@1.2.4")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

add_subdirectory(src)
