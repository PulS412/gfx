cmake_minimum_required(VERSION 3.25.2)
project(gfx3D)

# Add source files
set(SOURCES main.cpp)

# Create executable
add_executable(gfx3D ${SOURCES})

add_subdirectory(external/glfw)
target_include_directories(gfx3D
    PUBLIC external/glfw/include
        PUBLIC include)

target_link_directories(gfx3D
    PRIVATE external/glfw/src)

target_link_libraries(gfx3D glfw)


