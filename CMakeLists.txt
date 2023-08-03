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

# Racer
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(racer VERSION 1.0.0)

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
else()
  set(TARGET_ARCHIVE "${TARGET_BASENAME}.tar.gz")
endif()
set(TARGET_SHA256 "${TARGET_ARCHIVE}.sha256")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_custom_target(
    target ALL
    DEPENDS $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TARGET_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/src/racer/assets
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TARGET_EXECUTABLE} ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}/${TARGET_EXECUTABLE}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/racer/assets ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}/assets
    COMMAND cd ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY} && zip -r ${TARGET_ARCHIVE} ${TARGET_BASENAME}
    COMMAND sha256sum ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY}/${TARGET_ARCHIVE} > ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY}/${TARGET_SHA256}
  )
else()
  add_custom_target(
    target ALL
    DEPENDS $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TARGET_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/src/racer/assets
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TARGET_EXECUTABLE} ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}/${TARGET_EXECUTABLE}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/racer/assets ${CMAKE_SOURCE_DIR}/${TARGET_DIRECTORY}/assets
    COMMAND tar -czvf ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY}/${TARGET_ARCHIVE} -C ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY} ${TARGET_BASENAME}
    COMMAND sha256sum ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY}/${TARGET_ARCHIVE} > ${CMAKE_SOURCE_DIR}/${OUTPUT_DIRECTORY}/${TARGET_SHA256}
  )
endif()
