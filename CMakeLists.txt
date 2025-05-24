cmake_minimum_required(VERSION 3.30)
project(Pacman)

set(CMAKE_CXX_STANDARD 17)

# --- Include Raylib ---
include_directories("C:/raylib/include")
link_directories("C:/raylib/lib")

# --- Include your own headers ---
include_directories(${PROJECT_SOURCE_DIR}/include)

# --- Add executable ---
add_executable(pacman
    main.cpp
    source/PacMan.cpp
    source/Entity.cpp
    source/Map.cpp
)

# --- Link libraries ---
target_link_libraries(pacman raylib opengl32 gdi32 winmm)
