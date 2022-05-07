cmake_minimum_required(VERSION 3.14)

project(SIUtils LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS
  Core
  Gui
  Qml
  Quick
  QuickControls2
  Svg
  Xml
)

set(SOURCES
  src/utils.cpp
)

set(HEADERS
  include/utils.h
)


add_library(SIUtils STATIC ${HEADERS} ${SOURCES})
target_compile_definitions(SIUtils PUBLIC)
target_include_directories(SIUtils PUBLIC include)

target_link_libraries(SIUtils PUBLIC
  Qt::Core
  Qt::Gui
  Qt::Svg
  Qt::Xml
  Qt::Qml
  Qt::Quick
  Qt::QuickControls2
)
