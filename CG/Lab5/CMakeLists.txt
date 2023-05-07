cmake_minimum_required(VERSION 3.21)
project(Lab5)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/../libraries/include)

find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

add_executable(Lab5 main.cpp)
target_link_libraries(Lab5 PUBLIC OpenGL::GL glu32)

find_package(FreeGLUT CONFIG REQUIRED)
target_link_libraries(Lab5 PRIVATE $<IF:$<TARGET_EXISTS:FreeGLUT::freeglut>,FreeGLUT::freeglut,FreeGLUT::freeglut_static>)