cmake_minimum_required(VERSION 3.50)
project(Pacman)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/raylib/include")
link_directories("C:/raylib/lib")

add_executable(pacman main.cpp)

target_link_libraries(pacman raylib opengl32 gdi32 winmm)
