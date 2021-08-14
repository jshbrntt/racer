cmake_minimum_required(VERSION 3.5)

project(racer CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Reorder linker flags to fix 'undefined reference to WinMain'
# https://github.com/msys2/MINGW-packages/issues/6379#issue-598436450
if (MINGW)
    list(REMOVE_ITEM CONAN_LIBS "mingw32")
    list(APPEND CONAN_LIBS "-mwindows")
    list(PREPEND CONAN_LIBS "mingw32")
endif()

file(GLOB_RECURSE RACER_SRCS src/racer/*.cpp)
file(GLOB_RECURSE RACER_HDRS src/racer/*.h)
add_executable(racer ${RACER_SRCS} ${RACER_HDRS})
target_link_libraries(racer ${CONAN_LIBS})
# target_compile_features(racer PRIVATE cxx_std_17)

file(GLOB_RECURSE SNAKE_SRCS src/snake/*.cpp)
file(GLOB_RECURSE SNAKE_HDRS src/snake/*.h)
add_executable(snake ${SNAKE_SRCS} ${SNAKE_HDRS})
target_link_libraries(snake ${CONAN_LIBS})
target_compile_features(snake PRIVATE cxx_std_17)
