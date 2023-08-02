export CWD := $(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST)))))

ifndef DOCKER

DOCKER_REGISTRY_URL := ghcr.io

ifdef CI
DOCKER_REGISTRY_USERNAME := $(GITHUB_ACTOR)
DOCKER_REGISTRY_PASSWORD := $(GITHUB_TOKEN)
endif

IMAGE := $(DOCKER_REGISTRY_URL)/jshbrntt/racer/devcontainer
DOCKER := docker
WORKDIR := /home/ubuntu/racer

export DOCKER_BUILDKIT = 1
export BUILDKIT_PROGRESS = plain

.PHONY: require-%
require-%:
	@#$(or ${$*}, $(error $* is not set))

.PHONY: required-login-variables
required-login-variables: require-DOCKER_REGISTRY_URL
required-login-variables: require-DOCKER_REGISTRY_USERNAME
required-login-variables: require-DOCKER_REGISTRY_PASSWORD

.PHONY: docker-login
docker-login: required-login-variables
docker-login:
	echo $(DOCKER_REGISTRY_PASSWORD) | $(DOCKER) login --password-stdin --username $(DOCKER_REGISTRY_USERNAME) $(DOCKER_REGISTRY_URL)

.PHONY: build-linux
build-linux: TARGET := linux
build-linux: COMMAND := make $(if $(CLEAN),CLEAN=1 )$(if $(DEBUG),DEBUG=1 )LINUX=1
build-linux: docker-command

.PHONY: build-windows
build-windows: TARGET := windows
build-windows: COMMAND := make $(if $(CLEAN),CLEAN=1 )$(if $(DEBUG),DEBUG=1 )WINDOWS=1
build-windows: docker-command

.PHONY: build-macosx
build-macosx: TARGET := macosx
build-macosx: COMMAND := make $(if $(CLEAN),CLEAN=1 )$(if $(DEBUG),DEBUG=1 )MACOSX=1
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
shell: TARGET ?= windows
shell: COMMAND := bash
shell: docker-command

.PHONY: debug
debug:
	printenv

.PHONY: docker-command
docker-command: docker-build
docker-command: docker-push
docker-command: docker-run

.PHONY: docker-build
docker-build: $(if $(CI),docker-login)
	$(DOCKER) build \
	--pull \
	--cache-from $(IMAGE)/$(TARGET) \
	--build-arg BUILDKIT_INLINE_CACHE=1 \
	--target $(TARGET) \
	--tag $(IMAGE)/$(TARGET) \
	.

.PHONY: docker-run
docker-run:
	docker run \
	$(if $(CI),,--interactive )--tty \
	--rm \
	--env DOCKER=1 \
	--volume $(CWD):$(WORKDIR) \
	--workdir $(WORKDIR) \
	$(IMAGE)/$(TARGET) \
	$(COMMAND)

.PHONY: docker-push
docker-push: $(if $(CI),docker-login)
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
	-DCMAKE_BUILD_TYPE=$(if $(DEBUG),Debug,Release)

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
	-DCMAKE_TOOLCHAIN_FILE=$(CWD)/cmake/clang_windows_cross.cmake \
	-DCMAKE_BUILD_TYPE=$(if $(DEBUG),Debug,Release) \
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
	-DCMAKE_BUILD_TYPE=$(if $(DEBUG),Debug,Release) \
	-DCMAKE_TOOLCHAIN_FILE=/osxcross/tools/toolchain.cmake

.PHONY: clean
clean:
	rm -rf build/macosx

endif

endif
