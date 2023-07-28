cmake_minimum_required(VERSION 3.26.4)

# Set to ON to debug the build
set(CMAKE_VERBOSE_MAKEFILE OFF)

include(FetchContent)

# EnTT
FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG        v3.2.2
  GIT_SHALLOW    ON
  GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(EnTT)

# SDL2
set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-2.28.1
  GIT_SHALLOW    ON
  GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(SDL2)

# SDL2_image
set(SDL2IMAGE_INSTALL OFF)

FetchContent_Declare(
  SDL2_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG        release-2.6.3
  GIT_SHALLOW    ON
  GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(SDL2_image)

# SDL2_ttf
set(SDL2TTF_INSTALL OFF)
set(SDL2TTF_VENDORED ON)

FetchContent_Declare(
  SDL2_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG        release-2.20.2
  GIT_SHALLOW    ON
  GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(SDL2_ttf)

# Snake
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# project(snake VERSION 1.0)

# file(GLOB_RECURSE SNAKE_HDRS src/snake/*.h)
# file(GLOB_RECURSE SNAKE_SRCS src/snake/*.cpp)

# add_executable(${PROJECT_NAME} ${SNAKE_HDRS} ${SNAKE_SRCS})
# target_link_libraries(
#   ${PROJECT_NAME}
#   EnTT::EnTT
#   SDL2::SDL2main
#   SDL2::SDL2-static
#   SDL2_image::SDL2_image-static
#   SDL2_ttf::SDL2_ttf-static
# )

# Racer
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(racer VERSION 1.0)

file(GLOB_RECURSE RACER_HDRS src/racer/*.h)
file(GLOB_RECURSE RACER_SRCS src/racer/*.cpp)

add_executable(${PROJECT_NAME} ${RACER_HDRS} ${RACER_SRCS})
target_link_libraries(
  ${PROJECT_NAME}
  EnTT::EnTT
  SDL2::SDL2main
  SDL2::SDL2-static
  SDL2_image::SDL2_image-static
  SDL2_ttf::SDL2_ttf-static
)

# Copy executable to root directory
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
if(CMAKE_SYSTEM_NAME MATCHES Linux OR CMAKE_SYSTEM_NAME MATCHES Darwin)
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/racer
    ${CMAKE_SOURCE_DIR}/racer
  )
endif()
if(CMAKE_SYSTEM_NAME MATCHES Windows)
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/racer.exe
    ${CMAKE_SOURCE_DIR}/racer.exe
  )
endif()
