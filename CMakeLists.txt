cmake_minimum_required(VERSION 3.25.2)
project(gfx3D)

# Add source files
set(SOURCES
    main.cpp
    glad.c)

# Create executable
add_executable(gfx3D ${SOURCES})

add_subdirectory(external/glfw)

target_include_directories(gfx3D
    PRIVATE external/glfw/include
    PRIVATE include)

target_link_directories(gfx3D
    PRIVATE external/glfw/src
    PRIVATE include)

target_link_libraries(gfx3D
    glfw)


