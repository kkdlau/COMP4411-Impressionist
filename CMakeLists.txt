cmake_minimum_required(VERSION 3.0)

project(PROJ1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_EXTENSIONS ON)

find_package(OpenGL REQUIRED)
find_package(FLTK REQUIRED)

add_executable(proj1)

target_include_directories(proj1 PUBLIC ./)
target_include_directories(proj1 PUBLIC ./fltk-1.3.3/)

target_link_libraries(proj1 PRIVATE ${OPENGL_LIBRARIES})

include_directories(${FLTK_INCLUDE_DIR})
target_link_libraries(proj1 PRIVATE ${FLTK_LIBRARIES})

target_sources(proj1 PRIVATE 
    Bitmap.cpp
    ImpBrush.cpp
    Impressionist.cpp
    ImpressionistDoc.cpp
    ImpressionistUI.cpp
    OriginalView.cpp
    PaintView.cpp
    PointBrush.cpp
    # LineBrush.cpp
    # ScatteredPointBrush.cpp
    # CircleBrush.cpp
    # ScatteredCircleBrush.cpp
    # ScatteredLineBrush.cpp
    # DirectionLine.cpp
    # FilterBrush.cpp
    # KernalBrush.cpp
    # AlphaMappingBrush.cpp
    )