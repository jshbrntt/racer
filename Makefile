.DEFAULT_GOAL = docker-run
PROJECT = racer

OS = Windows
ARCH = x86_64
COMPILER = gcc
COMPILER_VERSION = 7
BUILD_TYPE = Debug
BUILD_DIR_PREFIX = build
CONAN_CACHE_VOLUME_PREFIX = cache
CONTAINER_NAME_PREFIX = build
SOURCE_DIR = SnakeGame

BUILD_DIR := $(shell echo $(BUILD_DIR_PREFIX)_$(OS)_$(ARCH)_$(BUILD_TYPE) | tr A-Z a-z)
INSTALL_DIR = $(BUILD_DIR)
CONAN_CACHE_VOLUME := $(shell echo $(PROJECT)_$(CONAN_CACHE_VOLUME_PREFIX)_$(OS) | tr A-Z a-z)
CONTAINER_NAME := $(shell echo $(PROJECT)_$(CONTAINER_NAME_PREFIX)_$(OS) | tr A-Z a-z)
IMAGE := $(shell echo $(PROJECT):$(OS) | tr A-Z a-z)
TARGET := $(shell echo $(OS) | tr A-Z a-z)
WORKDIR := $(shell echo /home/conan/$(PROJECT) | tr A-Z a-z)

export DOCKER_BUILDKIT = 1

.PHONY: conan-source
.ONESHELL: conan-source
conan-source:
	if [ ! -d "$(SOURCE_DIR)" ]; then
		conan source .
	fi

.PHONY: conan-install
.ONESHELL: conan-install
conan-install:
	conan install . \
	--build missing \
	--install-folder $(INSTALL_DIR) \
	--settings os=$(OS) \
	--settings arch=$(ARCH) \
	--settings compiler=$(COMPILER) \
	--settings compiler.version=$(COMPILER_VERSION) \
	--settings build_type=$(BUILD_TYPE)

.PHONY: conan-build
.ONESHELL: conan-build
conan-build:
	conan build . \
	--install-folder $(INSTALL_DIR) \
	--build-folder $(BUILD_DIR)

.PHONY: build
build: conan-source
build: conan-install
build: conan-build

.PHONY: docker-build
.ONESHELL: docker-build
docker-build:
	docker build \
	--build-arg BUILDKIT_INLINE_CACHE=1 \
	--target $(TARGET) \
	--tag $(IMAGE) .

.PHONY: clean
.ONESHELL: clean
clean:
	rm -rf $(BUILD_DIR_PREFIX)*

.PHONY: docker-run
.ONESHELL: docker-run
docker-run: docker-build
	docker run \
	--interactive \
	--tty \
	--rm \
	--volume $(CONAN_CACHE_VOLUME):/home/conan/.conan \
	--volume $(PWD):$(WORKDIR) \
	--workdir $(WORKDIR) \
	--name $(CONTAINER_NAME) \
	$(IMAGE) \
	make \
	ARCH=$(ARCH) \
	BUILD_DIR=$(BUILD_DIR) \
	BUILD_TYPE=$(BUILD_TYPE) \
	COMPILER_VERSION=$(COMPILER_VERSION) \
	COMPILER=$(COMPILER) \
	INSTALL_DIR=$(INSTALL_DIR) \
	OS=$(OS) \
	build
