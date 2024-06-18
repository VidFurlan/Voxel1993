cmake_minimum_required(VERSION 3.12)
project(3d-engine)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_library(glad src/glad.c)
set(LIBS ${LIBS} glad)

include(FetchContent)
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

# Header includes
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/3D_ENGINE)

file(GLOB PROJECT_HEADERS 
    ${CMAKE_SOURCE_DIR}/include/3D_ENGINE/*.h
    ${CMAKE_SOURCE_DIR}/include/3D_ENGINE/*.hpp
)

set(SOURCES
    src/main.cpp
    src/window.cpp
    src/player.cpp
    src/renderer.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw glm::glm ${OPENGL_LIBRARIES})
