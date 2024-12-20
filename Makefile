ifndef HOSTNAME
# Outside container

include picard/picard.mk

REGISTRY_HOSTNAME := ghcr.io
REGISTRY_NAMESPACE := jshbrntt

PLATFORM ?= linux

.PHONY: shell
shell: IMAGE_TARGET := devcontainer_$(PLATFORM)
shell: COMMAND := bash
shell: RUN_OPTIONS += --env PLATFORM=$(PLATFORM)
shell: docker-command

.PHONY: clean
clean: IMAGE_TARGET := devcontainer_$(PLATFORM)
clean: COMMAND := make clean PLATFORM=$(PLATFORM)$(if $(DEBUG), DEBUG=1)
clean: docker-command

.PHONY: clean-%
clean-%:
	$(MAKE) clean PLATFORM=$*

.PHONY: build
build: IMAGE_TARGET := devcontainer_$(PLATFORM)
build: COMMAND := make $(if $(CLEAN),clean )build PLATFORM=$(PLATFORM)$(if $(DEBUG), DEBUG=1)
build: docker-command

.PHONY: build-%
build-%:
	$(MAKE) build PLATFORM=$*

.PHONY: all
all: build-windows build-linux build-macosx

else
# Inside container

include picard/constants.mk
include picard/list.mk
include picard/string.mk

# Set platform if not already set
ifdef LINUX
	PLATFORM ?= linux
endif
ifdef MACOS
	PLATFORM ?= macosx
endif
PLATFORM ?= windows

# Validate selected platform
SUPPORTED_PLATFORMS = linux windows macosx
$(if $(filter-out $(SUPPORTED_PLATFORMS),$(PLATFORM)),$(error error: unsupported platform: $(PLATFORM), expected one of the following: [$(call list_join,|,$(SUPPORTED_PLATFORMS))]))

# Set default build target for each platform
ifeq ($(PLATFORM),windows)
	BUILD_TARGET_ARCH ?= x86_64
	BUILD_TARGET_VENDOR ?= pc
	BUILD_TARGET_SYS ?= windows
	BUILD_TARGET_ABI ?= msvc
endif
ifeq ($(PLATFORM),linux)
	BUILD_TARGET_ARCH ?= x86_64
	BUILD_TARGET_VENDOR ?= unknown
	BUILD_TARGET_SYS ?= linux
	BUILD_TARGET_ABI ?= gnu
endif
ifeq ($(PLATFORM),macosx)
	BUILD_TARGET_ARCH ?= x86_64
	BUILD_TARGET_VENDOR ?= apple
	BUILD_TARGET_SYS ?= darwin
endif

TARGET_TRIPLE ?= $(call list_join,-,$(BUILD_TARGET_ARCH) $(BUILD_TARGET_VENDOR) $(BUILD_TARGET_SYS) $(BUILD_TARGET_ABI))

BUILD_TYPE := $(if $(DEBUG),Debug,Release)
BUILD_TYPE_LC := $(call lowercase,$(BUILD_TYPE))
BUILD_DIRECTORY_PARTS := build $(TARGET_TRIPLE)-$(BUILD_TYPE_LC)
BUILD_DIRECTORY := $(call list_join,/,$(BUILD_DIRECTORY_PARTS))

SOURCE_DIRECTORY ?= $(CWD_PATH)

.PHONY: all
all: clean build

$(BUILD_DIRECTORY):
	mkdir -p $(BUILD_DIRECTORY)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIRECTORY)

.PHONY: build
build: configure
	cmake --build $(BUILD_DIRECTORY) --parallel $(shell nproc)

.PHONY: configure
configure: $(BUILD_DIRECTORY)
	cmake \
-S $(SOURCE_DIRECTORY) \
-B $(BUILD_DIRECTORY) \
-DTARGET_TRIPLE=$(TARGET_TRIPLE) \
-DCMAKE_TOOLCHAIN_FILE=$(CWD_PATH)/cmake/$(BUILD_TARGET_SYS)-toolchain.cmake \
-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

endif
