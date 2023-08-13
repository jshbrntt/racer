
set(ENV{OSXCROSS_TARGET} "darwin21.4")
set(ENV{OSXCROSS_HOST} "x86_64-apple-darwin21.4")
set(ENV{OSXCROSS_TARGET_DIR} "/osxcross/target")
set(ENV{OSXCROSS_SDK} "/osxcross/target/SDK/MacOSX12.3.sdk")
include(/osxcross/tools/toolchain.cmake)
