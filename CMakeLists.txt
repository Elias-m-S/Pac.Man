cmake_minimum_required(VERSION 3.30)
project(PacMan)

set(CMAKE_CXX_STANDARD 17)

# Include Raylib (adjust path as needed)
include_directories(${PROJECT_SOURCE_DIR}/raylib/include)
link_directories(${PROJECT_SOURCE_DIR}/raylib/lib)

# Include headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather sources
file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/main.cpp"
    "${PROJECT_SOURCE_DIR}/source/*.cpp"
)

# Executable
add_executable(pacman ${SOURCES})

# Link libraries
target_link_libraries(pacman raylib opengl32 gdi32 winmm)
