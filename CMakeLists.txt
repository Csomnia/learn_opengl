cmake_minimum_required(VERSION 3.6)

project(opengl_lenrn)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(HEADER
    "include"
    "lib/glad/include"
    "lib/stb_image/include"
    "lib/glm-0.9.8.4/include")

set(SOURCE
    "src/main.cpp")


INCLUDE_DIRECTORIES(${HEADER})

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)

set(GLAD "lib/glad/src/glad.c")
# message(${OPENGL_gl_LIBRARY})

add_executable(${PROJECT_NAME} ${SOURCE} ${GLAD})

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_gl_LIBRARY})
