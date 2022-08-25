cmake_minimum_required(VERSION 3.14)

project(SIUtils LANGUAGES CXX)

find_package(Qt6 COMPONENTS
  Core
  Gui
  Qml
  Quick
  QuickControls2
  Svg
  Xml
  REQUIRED
)

qt6_add_library(SIUtils STATIC)
qt6_add_qml_module(SIUtils
    URI SIUtils
    VERSION 1.0
    SOURCES
        src/utils.h
        src/utils.cpp
)

target_link_libraries(SIUtils PRIVATE
  Qt::Core
  Qt::Gui
  Qt::Svg
  Qt::Xml
  Qt::Qml
  Qt::Quick
  Qt::QuickControls2
)
