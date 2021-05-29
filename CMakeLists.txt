cmake_minimum_required(VERSION 3.19.2)

project(racer VERSION 0.1.0)

include(FetchContent)

# Download libraries
FetchContent_Declare(
  SDL2
  URL     https://www.libsdl.org/release/SDL2-devel-2.0.14-VC.zip
  URL_MD5 2b521c5ec247955dc342235d06ebd874
)
FetchContent_Declare(
  SDL2_image
  URL     https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-VC.zip
  URL_MD5 b6c8c85b64e01b7096364036438b3333
)
FetchContent_MakeAvailable(SDL2)
FetchContent_MakeAvailable(SDL2_image)

# FindSDL2 modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Locate dependencies
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Racer target
file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

# Link OpenGL
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# Link SDL2
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# Link SDL2_image
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARIES})

# Copy libraries to build directory for runtime.
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${sdl2_SOURCE_DIR}/lib/x64
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${sdl2_image_SOURCE_DIR}/lib/x64
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
