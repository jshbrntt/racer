cmake_minimum_required(VERSION 3.19.2)

project(racer VERSION 0.1.0)

# Racer
file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# FindSDL2 Modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Fetch Content Module
include(FetchContent)

# SDL2
FetchContent_Declare(
  SDL2
  URL     https://www.libsdl.org/release/SDL2-devel-2.0.14-mingw.tar.gz
  URL_MD5 1f331c4e29f54cfc470976ac43994617
)
FetchContent_MakeAvailable(SDL2)
set(SDL2_PATH ${sdl2_SOURCE_DIR}/x86_64-w64-mingw32)
find_package(SDL2 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
add_custom_command(TARGET racer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${sdl2_SOURCE_DIR}/x86_64-w64-mingw32/bin
        $<TARGET_FILE_DIR:racer>)

# SDL2_image
FetchContent_Declare(
  SDL2_image
  URL     https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-mingw.tar.gz
  URL_MD5 2704080296a24b765ce1e9c3c427a970
)
FetchContent_MakeAvailable(SDL2_image)
message(${sdl2_image_SOURCE_DIR})
set(SDL2_IMAGE_PATH ${sdl2_image_SOURCE_DIR}/x86_64-w64-mingw32)
find_package(SDL2_image REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARIES})
add_custom_command(TARGET racer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${sdl2_image_SOURCE_DIR}/x86_64-w64-mingw32/bin
        $<TARGET_FILE_DIR:racer>)
