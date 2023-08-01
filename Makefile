export CWD := $(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST)))))

ifndef DOCKER

IMAGE := ghcr.io/jshbrntt/racer/devcontainer
DOCKER := docker
WORKDIR := /root/racer

export BUILDKIT_PROGRESS = plain

.PHONY: build-linux
build-linux: TARGET := linux
build-linux: COMMAND := printenv
build-linux: docker-command

.PHONY: build-windows
build-windows: TARGET := windows
build-windows: COMMAND := printenv
build-windows: docker-command

.PHONY: build-macosx
build-macosx: TARGET := macosx
build-macosx: COMMAND := printenv
build-macosx: docker-command

.PHONY: push-linux
push-linux: TARGET := linux
push-linux: docker-push

.PHONY: push-windows
push-windows: TARGET := windows
push-windows: docker-push

.PHONY: push-macosx
push-macosx: TARGET := macosx
push-macosx: docker-push

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
	--pull \
	--target $(TARGET) \
	--tag $(IMAGE)/$(TARGET) \
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
	$(IMAGE)/$(TARGET) \
	$(COMMAND)

.PHONY: docker-push
docker-push:
	docker push \
	$(IMAGE)/$(TARGET)

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

ifdef WINDOWS

.PHONY: build
build: configure
	cd build/windows
	cmake --build .

.PHONY: configure
configure: $(if $(CLEAN),clean)
	mkdir -p build/windows
	cd build/windows
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
	rm -rf build/windows

endif

ifdef MACOSX

.PHONY: build
build: configure
	cd build/macosx
	cmake --build .

.PHONY: configure
configure: $(if $(CLEAN),clean)
	mkdir -p build/macosx
	cd build/macosx
	OSXCROSS_TARGET=darwin21.4 \
	OSXCROSS_HOST=x86_64-apple-darwin21.4 \
	OSXCROSS_TARGET_DIR=/osxcross/target \
	OSXCROSS_SDK=/osxcross/target/SDK/MacOSX12.3.sdk \
	cmake ../.. \
	-DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_TOOLCHAIN_FILE=/osxcross/tools/toolchain.cmake

.PHONY: clean
clean:
	rm -rf build/macosx

endif

endif
