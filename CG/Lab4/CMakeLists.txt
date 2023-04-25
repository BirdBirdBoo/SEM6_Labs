cmake_minimum_required(VERSION 3.21)
project(Lab4)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/../libraries/include)

find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

add_executable(Lab4 main.cpp)
target_link_libraries(Lab4 PUBLIC OpenGL::GL glu32)

find_package(FreeGLUT CONFIG REQUIRED)
target_link_libraries(Lab4 PRIVATE $<IF:$<TARGET_EXISTS:FreeGLUT::freeglut>,FreeGLUT::freeglut,FreeGLUT::freeglut_static>)