cmake_minimum_required(VERSION 3.26.4)

# Set to ON to debug the build
set(CMAKE_VERBOSE_MAKEFILE OFF)

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

# Racer
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get the Git commit SHA using execute_process
execute_process(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_SHA
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Truncate the Git commit SHA to the first 7 characters
string(SUBSTRING ${GIT_COMMIT_SHA} 0 7 GIT_COMMIT_SHA)

project(racer VERSION 1.0.0)

configure_file(
  "${CMAKE_SOURCE_DIR}/src/racer/version.hpp.in"
  "${CMAKE_SOURCE_DIR}/src/racer/version.hpp"
)

file(GLOB_RECURSE RACER_HDRS src/racer/*.hpp)
file(GLOB_RECURSE RACER_SRCS src/racer/*.cpp)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${RACER_HDRS} ${RACER_SRCS})
else()
  add_executable(${PROJECT_NAME} ${RACER_HDRS} ${RACER_SRCS})
endif()

target_link_libraries(
  ${PROJECT_NAME}
  cxxopts::cxxopts
  EnTT::EnTT
  SDL2::SDL2main
  SDL2::SDL2-static
  SDL2_image::SDL2_image-static
  SDL2_ttf::SDL2_ttf-static
)

# Create release directory
function(get_target_triple TARGET_TRIPLE)
  set(TARGET_ARCH ${CMAKE_SYSTEM_PROCESSOR})
  set(TARGET_VENDOR ${CMAKE_SYSTEM_VENDOR})
  set(TARGET_SYS ${CMAKE_SYSTEM_NAME})

  if(TARGET_SYS STREQUAL "Linux")
    if(NOT TARGET_VENDOR)
      set(TARGET_VENDOR "unknown")
    endif()
    set(TARGET_ENV "gnu")
  endif()

  if(TARGET_SYS STREQUAL "Windows")
    if(TARGET_ARCH STREQUAL "AMD64")
      set(TARGET_ARCH "x86_64")
    endif()
    if(NOT TARGET_VENDOR)
      set(TARGET_VENDOR "pc")
    endif()
    # TODO: Support gnu ABI in future.
    set(TARGET_ENV "msvc")
  endif()

  if(TARGET_SYS STREQUAL "Darwin")
    if(NOT TARGET_VENDOR)
      set(TARGET_VENDOR "apple")
    endif()
  endif()

  set(TARGET_COMPONENTS ${TARGET_ARCH} ${TARGET_VENDOR} ${TARGET_SYS} ${TARGET_ENV})
  string(JOIN "-" TARGET_COMPONENTS ${TARGET_COMPONENTS})
  string(TOLOWER ${TARGET_COMPONENTS} TARGET_COMPONENTS)
  set(${TARGET_TRIPLE} ${TARGET_COMPONENTS} PARENT_SCOPE)
endfunction()

get_target_triple(TARGET_TRIPLE)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(TARGET_EXECUTABLE "${PROJECT_NAME}.exe")
else()
  set(TARGET_EXECUTABLE "${PROJECT_NAME}")
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
set(OUTPUT_DIRECTORY "target/${BUILD_TYPE}")
if(BUILD_TYPE STREQUAL "debug")
  set(TARGET_BASENAME "${PROJECT_NAME}-${TARGET_TRIPLE}")
elseif(BUILD_TYPE STREQUAL "release")
  set(TARGET_BASENAME "${PROJECT_NAME}-${PROJECT_VERSION}-${TARGET_TRIPLE}")
endif()
set(TARGET_DIRECTORY "${OUTPUT_DIRECTORY}/${TARGET_BASENAME}")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(TARGET_ARCHIVE "${TARGET_BASENAME}.zip")
  set(ARCHIVE_COMMAND cd ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY} && zip -r ${TARGET_ARCHIVE} ${TARGET_BASENAME})
else()
  set(TARGET_ARCHIVE "${TARGET_BASENAME}.tar.gz")
  set(ARCHIVE_COMMAND tar -czvf ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY}/${TARGET_ARCHIVE} -C ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY} ${TARGET_BASENAME})
endif()
set(TARGET_SHA256 "${TARGET_ARCHIVE}.sha256")

if(BUILD_TYPE STREQUAL "release")
  # Copy executable and assets to target directory for distribution
  add_custom_target(
    target ALL
    DEPENDS $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TARGET_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/src/racer/assets
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TARGET_EXECUTABLE} ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}/${TARGET_EXECUTABLE}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/racer/assets ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/racer/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMAND ${ARCHIVE_COMMAND}
    COMMAND sha256sum ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY}/${TARGET_ARCHIVE} > ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY}/${TARGET_SHA256}
  )
else()
  # Create symlink to assets directory for development
  add_custom_target(
    target ALL
    DEPENDS $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TARGET_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/src/racer/assets
    COMMAND ${CMAKE_COMMAND} -E create_symlink ../../../src/racer/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  )
endif()
