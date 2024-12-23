# Cross toolchain configuration for using clang-cl.
set(CMAKE_AR "/usr/bin/llvm-lib")
set(CMAKE_RC_COMPILER "/usr/bin/llvm-windres")
set(CMAKE_C_COMPILER "/usr/bin/clang-cl")
set(CMAKE_CXX_COMPILER "/usr/bin/clang-cl")
set(CMAKE_LINKER "/usr/bin/lld-link")
set(MSVC_BASE "/xwin/crt")
set(WINSDK_BASE "/xwin/sdk")
set(WINSDK_VER "10.0.26100")
set(TARGET_ARCH "x86_64")

# SDL2
set(HAVE_STDINT_H "1")
set(SDL_JOYSTICK_XINPUT "1")

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

if(NOT DEFINED CMAKE_C_COMPILER)
  set(CMAKE_C_COMPILER "/usr/bin/clang-cl")
endif()
if(NOT DEFINED CMAKE_CXX_COMPILER)
  set(CMAKE_CXX_COMPILER "/usr/bin/clang-cl")
endif()
if(NOT DEFINED CMAKE_LINKER)
  set(CMAKE_LINKER "/usr/bin/lld-link")
endif()

if(NOT DEFINED TARGET_ARCH)
  set(TARGET_ARCH "x86_64")
endif()
if(NOT DEFINED MSVC_BASE)
  set(MSVC_BASE "/mnt/c/Program\ Files\ \(x86\)/Microsoft\ Visual\ Studio/2019/Community/VC/Tools/MSVC/14.23.28105")
endif()
if(NOT DEFINED MSVC_INCLUDE)
  set(MSVC_INCLUDE "${MSVC_BASE}/include")
endif()
if(NOT DEFINED MSVC_LIB)
  set(MSVC_LIB "${MSVC_BASE}/lib")
endif()

if(NOT DEFINED WINSDK_BASE)
  set(WINSDK_BASE "/mnt/c/Program\ Files\ \(x86\)/Windows\ Kits/10")
endif()
if(NOT DEFINED WINSDK_VER)
  set(WINSDK_VER "10.0.18362.0")
endif()
set(WINSDK_INCLUDE "${WINSDK_BASE}/Include/${WINSDK_VER}")
set(WINSDK_LIB "${WINSDK_BASE}/lib/${WINSDK_VER}")

set(COMPILE_FLAGS
  -D_CRT_SECURE_NO_WARNINGS
  -imsvc "'${MSVC_INCLUDE}'"
  -imsvc "'${WINSDK_INCLUDE}/ucrt'"
  -imsvc "'${WINSDK_INCLUDE}/shared'"
  -imsvc "'${WINSDK_INCLUDE}/um'"
  -imsvc "'${WINSDK_INCLUDE}/winrt'"
	-msse3
  /EHsc
)
# msse3: https://github.com/libsdl-org/SDL/issues/5186
# EHsc:  https://learn.microsoft.com/en-us/cpp/build/reference/eh-exception-handling-model?view=msvc-170

string(REPLACE ";" " " COMPILE_FLAGS "${COMPILE_FLAGS}")

set(CMAKE_RC_FLAGS
  -I "'${MSVC_INCLUDE}'"
  -I "'${WINSDK_INCLUDE}/ucrt'"
  -I "'${WINSDK_INCLUDE}/shared'"
  -I "'${WINSDK_INCLUDE}/um'"
  -I "'${WINSDK_INCLUDE}/winrt'"
)

string(REPLACE ";" " " CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS}")

set(_CMAKE_C_FLAGS_INITIAL "${CMAKE_C_FLAGS}" CACHE STRING "")
set(CMAKE_C_FLAGS "${COMPILE_FLAGS} ${_CMAKE_C_FLAGS_INITIAL}" CACHE STRING "" FORCE)

set(_CMAKE_CXX_FLAGS_INITIAL "${CMAKE_C_FLAGS}" CACHE STRING "")
set(CMAKE_CXX_FLAGS "${COMPILE_FLAGS} ${_CMAKE_CXX_FLAGS_INITIAL}" CACHE STRING "" FORCE)

set(LINK_FLAGS
  /manifest:no
  -libpath:"${MSVC_LIB}/${TARGET_ARCH}"
  -libpath:"${WINSDK_LIB}/ucrt/${TARGET_ARCH}"
  -libpath:"${WINSDK_LIB}/um/${TARGET_ARCH}"
)

string(REPLACE ";" " " LINK_FLAGS "${LINK_FLAGS}")

set(_CMAKE_EXE_LINKER_FLAGS_INITIAL "${CMAKE_EXE_LINKER_FLAGS}" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS "${_CMAKE_EXE_LINKER_FLAGS_INITIAL} ${LINK_FLAGS}" CACHE STRING "" FORCE)

set(_CMAKE_MODULE_LINKER_FLAGS_INITIAL "${CMAKE_MODULE_LINKER_FLAGS}" CACHE STRING "")
set(CMAKE_MODULE_LINKER_FLAGS "${_CMAKE_MODULE_LINKER_FLAGS_INITIAL} ${LINK_FLAGS}" CACHE STRING "" FORCE)

set(_CMAKE_SHARED_LINKER_FLAGS_INITIAL "${CMAKE_SHARED_LINKER_FLAGS}" CACHE STRING "")
set(CMAKE_SHARED_LINKER_FLAGS "${_CMAKE_SHARED_LINKER_FLAGS_INITIAL} ${LINK_FLAGS}" CACHE STRING "" FORCE)

# https://github.com/llvm-mirror/llvm/blob/master/cmake/platforms/WinMsvc.cmake#L317-L319
set(CMAKE_C_STANDARD_LIBRARIES "" CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD_LIBRARIES "" CACHE STRING "" FORCE)

set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
