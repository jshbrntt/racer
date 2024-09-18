ifndef HOSTNAME
# Outside container

include picard/picard.mk

REGISTRY_HOSTNAME := ghcr.io
REGISTRY_NAMESPACE := jshbrntt

PLATFORM ?= linux

.PHONY: shell
shell: IMAGE_TARGET := devcontainer_$(PLATFORM)
shell: COMMAND := bash
shell: docker-command

.PHONY: clean
clean: IMAGE_TARGET := devcontainer_$(PLATFORM)
clean: COMMAND := make clean TARGET=$(PLATFORM)$(if $(DEBUG), DEBUG=1)
clean: docker-command

.PHONY: clean-%
clean-%:
	$(MAKE) clean PLATFORM=$*

.PHONY: build
build: IMAGE_TARGET := devcontainer_$(PLATFORM)
build: COMMAND := make $(if $(CLEAN),clean )build TARGET=$(PLATFORM)$(if $(DEBUG), DEBUG=1)
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

SUPPORTED_TARGETS = linux windows macosx

$(if $(filter-out $(SUPPORTED_TARGETS),$(TARGET)),$(error error: unsupported target: $(TARGET), expected one of the following: [$(call list_join,|,$(SUPPORTED_TARGETS))]))

BUILD_TYPE := $(if $(DEBUG),Debug,Release)
BUILD_TYPE_LC := $(call lowercase,$(BUILD_TYPE))
BUILD_DIRECTORY_PARTS := build $(TARGET) $(BUILD_TYPE_LC)
BUILD_DIRECTORY := $(call list_join,/,$(BUILD_DIRECTORY_PARTS))

.PHONY: all
all: clean build

$(BUILD_DIRECTORY):
	id
	ls -lah
	mkdir -p $(BUILD_DIRECTORY)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIRECTORY)

.PHONY: build
build: configure
	cmake --build $(BUILD_DIRECTORY)

.PHONY: configure
configure: $(BUILD_DIRECTORY)
	cmake -B $(BUILD_DIRECTORY) -S . \
-DCMAKE_TOOLCHAIN_FILE=$(CWD_PATH)/cmake/$(TARGET)-toolchain.cmake \
-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

endif
