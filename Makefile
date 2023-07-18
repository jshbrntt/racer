ifndef DOCKER

export BUILDKIT_PROGRESS=plain

.PHONY: build
build: docker-build
build: docker-run

.PHONY: docker-build
docker-build:
	docker build -t racer .

.PHONY: docker-run
docker-run:
	docker run -it --rm -e DOCKER=1 -v `pwd`:/root/racer -w /root/racer racer bash

else

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

.ONESHELL:

ifdef LINUX
.PHONY: build
build: configure
	cd build/linux
	cmake --build .

.PHONY: configure
configure: $(if $(CLEAN),clean)
	mkdir -p build/linux
	cd build/linux
	cmake ../..

.PHONY: clean
clean:
	rm -rf build/linux
endif

ifdef WIN32
.PHONY: build
build: configure
	cd build/win32
	cmake --build .

.PHONY: configure
configure: $(if $(CLEAN),clean)
	mkdir -p build/win32
	cd build/win32
	cmake ../.. \
	-DCMAKE_TOOLCHAIN_FILE=$(ROOT_DIR)clang_windows_cross.cmake \
	-DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_AR=/usr/bin/llvm-lib \
	-DCMAKE_RC_COMPILER=/usr/bin/llvm-windres \
	-DCMAKE_C_COMPILER=/usr/bin/clang-cl \
	-DCMAKE_CXX_COMPILER=/usr/bin/clang-cl \
	-DCMAKE_LINKER=/usr/bin/lld-link \
	-DMSVC_BASE=/xwin/crt \
	-DWINSDK_BASE=/xwin/sdk \
	-DWINSDK_VER=10.0.22000 \
	-DTARGET_ARCH=x86_64 \
	-DHAVE_STDINT_H=1 \
	-DSDL_JOYSTICK_XINPUT=1

.PHONY: clean
clean:
	rm -rf build/win32
endif

ifdef MACOS

.PHONY: build
build: configure
	cd build/macos
	cmake --build .

.PHONY: configure
configure: $(if $(CLEAN),clean)
	mkdir -p build/macos
	cd build/macos
	OSXCROSS_TARGET=darwin21.4 \
	OSXCROSS_HOST=x86_64-apple-darwin21.4 \
	OSXCROSS_TARGET_DIR=/osxcross/target \
	OSXCROSS_SDK=/osxcross/target/SDK/MacOSX12.3.sdk \
	cmake ../.. \
	-DCMAKE_TOOLCHAIN_FILE=/osxcross/tools/toolchain.cmake

.PHONY: clean
clean:
	rm -rf build/macos
endif

endif
