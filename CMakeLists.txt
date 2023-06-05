cmake_minimum_required(VERSION 3.26.4)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(snake VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SNAKE_HDRS src/snake/*.h)
file(GLOB_RECURSE SNAKE_SRCS src/snake/*.cpp)
add_executable(${PROJECT_NAME} ${SNAKE_HDRS} ${SNAKE_SRCS})

target_link_libraries(${PROJECT_NAME} ${WINSDK_LIB}/um/${TARGET_ARCH}/shell32.lib)

# Dependency of SDL2
# https://sjm.io/blog/sdl2-msvc-without-visual-studio/
# target_link_libraries(${PROJECT_NAME} ${WINSDK_LIB}/um/x64/shell32.lib)

include(FetchContent)

# EnTT
FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG        v3.2.2
)

FetchContent_MakeAvailable(EnTT)

target_include_directories(${PROJECT_NAME} PUBLIC ${entt_SOURCE_DIR}/src)

# Download SDL2
FetchContent_Declare(
  SDL2
  URL      https://libsdl.org/release/SDL2-devel-2.26.4-VC.zip
  URL_HASH MD5=3dbecd1fe24a43e4e563c5abf91c2426
)

FetchContent_MakeAvailable(SDL2)

# Find SDL2
set(SDL2_DIR ${sdl2_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)

# Link SDL2
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# Copy SDL2 lib files to build directory for runtime.
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${sdl2_SOURCE_DIR}/lib/x64
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# Download SDL2_image
FetchContent_Declare(
  SDL2_image
  URL      https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.6.3-VC.zip
  URL_HASH MD5=55e7eebdcdd0fe38ee0344e3ad280a54
)

FetchContent_MakeAvailable(SDL2_image)

# Find SDL2_image
set(SDL2_image_DIR ${sdl2_image_SOURCE_DIR}/cmake)
find_package(SDL2_image REQUIRED)

# Link SDL2_image
target_include_directories(${PROJECT_NAME} PRIVATE ${sdl2_image_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} SDL2_image::SDL2_image)

# Copy SDL2_image lib files to build directory for runtime.
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${sdl2_image_SOURCE_DIR}/lib/x64
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)


# Download SDL2_ttf
FetchContent_Declare(
  SDL2_ttf
  URL      https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.20.2/SDL2_ttf-devel-2.20.2-VC.zip
  URL_HASH MD5=96ffce885b6e1d2148ea36cedccccf3a
)

FetchContent_MakeAvailable(SDL2_ttf)

# Find SDL2_ttf
set(SDL2_ttf_DIR ${sdl2_ttf_SOURCE_DIR}/cmake)
find_package(SDL2_ttf REQUIRED)

# Link SDL2_ttf
target_include_directories(${PROJECT_NAME} PRIVATE ${sdl2_ttf_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} SDL2_ttf::SDL2_ttf)

# Copy SDL2_ttf lib files to build directory for runtime.
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${sdl2_ttf_SOURCE_DIR}/lib/x64
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
