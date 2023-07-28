export CWD := $(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST)))))

ifndef DOCKER

IMAGE := ghcr.io/jshbrntt/racer/devcontainer:latest
DOCKER := docker
WORKDIR := /root/racer

export BUILDKIT_PROGRESS = plain

.PHONY: build-linux
build-linux: COMMAND := make $(if $(CLEAN),CLEAN=1) LINUX=1
build-linux: docker-command

.PHONY: build-win32
build-win32: COMMAND := make $(if $(CLEAN),CLEAN=1) WIN32=1
build-win32: docker-command

.PHONY: build-macos
build-macos: COMMAND := make $(if $(CLEAN),CLEAN=1) MACOS=1
build-macos: docker-command

.PHONY: shell
shell: COMMAND := bash
shell: docker-command

.PHONY: debug
debug:
	printenv

.PHONY: docker-command
docker-command: docker-build
docker-command: docker-run

.PHONY: docker-build
docker-build:
	$(DOCKER) build \
	--tag $(IMAGE) \
	.

.PHONY: docker-run
docker-run:
	docker run \
	--interactive \
	--tty \
	--rm \
	--env DOCKER=1 \
	--volume racer_build:$(WORKDIR)/build \
	--volume $(CWD):$(WORKDIR) \
	--workdir $(WORKDIR) \
	$(IMAGE) \
	$(COMMAND)

else

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
	cmake ../.. \
	-DCMAKE_BUILD_TYPE=Debug

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
	-DCMAKE_TOOLCHAIN_FILE=$(CWD)/clang_windows_cross.cmake \
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
	-DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_TOOLCHAIN_FILE=/osxcross/tools/toolchain.cmake

.PHONY: clean
clean:
	rm -rf build/macos

endif

endif
