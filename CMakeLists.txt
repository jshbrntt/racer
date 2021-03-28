cmake_minimum_required(VERSION 3.19.2)

project(racer VERSION 0.1.0)

# Racer
file(GLOB SOURCES src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# CMake Modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
# set(SDL2_PATH libsdl-org/SDL)
# set(SDL2_IMAGE_PATH libsdl-org/SDL_image)

# OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# SDL2
add_subdirectory(libsdl-org/SDL)
find_package(SDL2 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# SDL2_image
add_subdirectory(libsdl-org/SDL_image)
find_package(SDL2_image REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARIES})
