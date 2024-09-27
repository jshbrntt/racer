cmake_minimum_required(VERSION 3.26.4)

# Set to ON to debug the build
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Build and configure dependencies
# cxxopts
add_subdirectory(${CMAKE_SOURCE_DIR}/external/cxxopts)

# entt
add_subdirectory(${CMAKE_SOURCE_DIR}/external/entt)

# SDL2
set(BUILD_SHARED_LIBS OFF)
set(SDL_TEST_ENABLED_BY_DEFAULT OFF)
set(SDL_3DNOW OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL)

# SDL2_image
set(SDL2IMAGE_INSTALL OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL_image)

# SDL2_ttf
set(SDL2TTF_INSTALL OFF)
set(SDL2TTF_VENDORED ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL_ttf)

# C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project
project(racer VERSION 1.0.0)

# Set semantic version variables
set(SEMVER_VERSION_CORE ${PROJECT_VERSION})
if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
  # If not release assume it is a release candidate build
  set(SEMVER_PRE_RELEASE "rc")
endif()
find_package(Git)
if(Git_FOUND)
  execute_process(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE SEMVER_BUILD
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  string(SUBSTRING ${SEMVER_BUILD} 0 7 SEMVER_BUILD)
endif()

configure_file(
  "${CMAKE_SOURCE_DIR}/src/racer/version.hpp.in"
  "${CMAKE_SOURCE_DIR}/src/racer/version.hpp"
)

file(GLOB_RECURSE RACER_HDRS src/racer/*.hpp)
file(GLOB_RECURSE RACER_SRCS src/racer/*.cpp)

# Create bundle on macOS
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${RACER_HDRS} ${RACER_SRCS})
else()
  add_executable(${PROJECT_NAME} ${RACER_HDRS} ${RACER_SRCS})
endif()

# Run dsymutil to generate debug symbols for macOS
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND dsymutil $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Running dsymutil on ${PROJECT_NAME}"
  )
endif()

# Link project dependenciess
target_link_libraries(
  ${PROJECT_NAME}
  cxxopts::cxxopts
  EnTT::EnTT
  SDL2::SDL2main
  SDL2::SDL2-static
  SDL2_image::SDL2_image-static
  SDL2_ttf::SDL2_ttf-static
)

# Add exe extension to executable on Windows
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(TARGET_EXECUTABLE "${PROJECT_NAME}.exe")
else()
  set(TARGET_EXECUTABLE "${PROJECT_NAME}")
endif()

# Create target directory, only include project version in release builds
string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE_LC)
set(TARGET_DIRECTORY "target")
set(TARGET_BASENAME "${PROJECT_NAME}-${PROJECT_VERSION}-${TARGET_TRIPLE}-${BUILD_TYPE_LC}")
set(TARGET_PATH "${TARGET_DIRECTORY}/${TARGET_BASENAME}")

# Create zip archive on Windows, tar.gz archive on other platforms
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(TARGET_ARCHIVE "${TARGET_BASENAME}.zip")
  set(ARCHIVE_COMMAND cd ${CMAKE_SOURCE_DIR}/${TARGET_PATH} && zip -r ${TARGET_ARCHIVE} ${TARGET_BASENAME})
else()
  set(TARGET_ARCHIVE "${TARGET_BASENAME}.tar.gz")
  set(ARCHIVE_COMMAND tar -czvf ${CMAKE_SOURCE_DIR}/${TARGET_PATH}/${TARGET_ARCHIVE} -C ${CMAKE_SOURCE_DIR}/${TARGET_PATH} ${TARGET_BASENAME})
endif()

set(PROJECT_ASSETS_SOURCE_PATH ${CMAKE_SOURCE_DIR}/src/racer/assets)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  # Copy executable and assets to target directory for distribution
  add_custom_target(
    target ALL
    DEPENDS $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TARGET_EXECUTABLE}
            ${PROJECT_ASSETS_SOURCE_PATH}
    COMMAND ${CMAKE_COMMAND} -E create_symlink $<PATH:RELATIVE_PATH,${PROJECT_ASSETS_SOURCE_PATH},$<TARGET_FILE_DIR:${PROJECT_NAME}>> $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    # Packaging for distribution
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/${TARGET_PATH}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/${TARGET_PATH}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TARGET_EXECUTABLE} ${CMAKE_SOURCE_DIR}/${TARGET_PATH}/${TARGET_EXECUTABLE}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_ASSETS_SOURCE_PATH} ${CMAKE_SOURCE_DIR}/${TARGET_PATH}/assets
    COMMAND ${ARCHIVE_COMMAND}
    COMMAND sha256sum ${CMAKE_SOURCE_DIR}/${TARGET_PATH}/${TARGET_ARCHIVE} > ${CMAKE_SOURCE_DIR}/${TARGET_PATH}/${TARGET_ARCHIVE}.sha256
  )
else()
  # Create symlink to assets directory for development
  add_custom_target(
    target ALL
    DEPENDS $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TARGET_EXECUTABLE}
            ${PROJECT_ASSETS_SOURCE_PATH}
    COMMAND ${CMAKE_COMMAND} -E create_symlink $<PATH:RELATIVE_PATH,${PROJECT_ASSETS_SOURCE_PATH},$<TARGET_FILE_DIR:${PROJECT_NAME}>> $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  )
endif()
