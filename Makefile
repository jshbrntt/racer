ifndef DOCKER

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

.ONESHELL:

.PHONY: build
build: clean
build: configure

.PHONY: clean
clean:
	rm -rf build

.PHONY: configure
configure:
	mkdir -p build
	cd build
	cmake .. \
	-DCMAKE_TOOLCHAIN_FILE=clang_windows_cross.cmake \
	-DCMAKE_C_COMPILER=clang-cl \
	-DCMAKE_CXX_COMPILER=clang-cl \
	-DCMAKE_LINKER=lld-link \
	-DMSVC_BASE=/xwin/crt \
	-DWINSDK_BASE=/xwin/sdk \
	-DWINSDK_VER=10.0.22000 \
	-DTARGET_ARCH=x86_64
	cmake --build .

endif
