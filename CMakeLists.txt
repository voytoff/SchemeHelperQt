cmake_minimum_required(VERSION 3.20)

project(SchemeHelper LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Quick Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Widgets Svg)

add_library(SchemeHelper SHARED
  SchemeHelper_global.h
  schemehelper.cpp
  schemehelper.h
  action.h action.cpp
)

target_link_libraries(SchemeHelper PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Svg
  dwmapi # DwmSetWindowAttribute
)

target_compile_definitions(SchemeHelper PRIVATE SCHEMEHELPER_LIBRARY)
