cmake_minimum_required(VERSION 3.21)
project(Lab2)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/../libraries/include)

link_directories(${CMAKE_SOURCE_DIR}/../libraries/lib)
link_directories(${CMAKE_SOURCE_DIR}/../libraries/lib/x64)


find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

message(OGL FOUND: ${OpenGL_OpenGL_FOUND})
message(GLU FOUND: ${OPENGL_glu_LIBRARY})

add_executable(Lab2 main.cpp main.h)
target_link_libraries(Lab2 PUBLIC OpenGL::GL freeglut glu32)