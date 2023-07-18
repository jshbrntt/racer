# syntax=docker/dockerfile:1-labs
FROM ubuntu:23.04@sha256:b5d5cddaeb8d2f150660cf8f9a1203dd450ac765e6c07630176ac6486eceaddb AS base

RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
ca-certificates=20230311ubuntu0.23.04.1 \
git=1:2.39.2-1ubuntu1.1 \
gnupg=2.2.40-1.1ubuntu1 \
lsb-release=12.0-1ubuntu1 \
make=4.3-4.1build1 \
software-properties-common=0.99.35 \
wget=1.21.3-1ubuntu1 \
patch=2.7.6-7build2 \
libxml2-dev=2.9.14+dfsg-1.1ubuntu0.1 \
libssl-dev=3.0.8-1ubuntu1.2 \
zlib1g-dev=1:1.2.13.dfsg-1ubuntu4 \
xz-utils=5.4.1-0.2

# Add LLVM APT repository (https://apt.llvm.org/)
RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc \
&& OS_CODENAME=$(lsb_release -sc) \
&& add-apt-repository "deb http://apt.llvm.org/${OS_CODENAME}/ llvm-toolchain-${OS_CODENAME} main"

ARG LLVM_VERSION="16"

# Install LLVM
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
clang-${LLVM_VERSION} \
llvm-${LLVM_VERSION} \
lld-${LLVM_VERSION}

# Create missing LLVM symlinks
RUN ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang /usr/bin/clang \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang-cl /usr/bin/clang-cl \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang-cpp /usr/bin/clang-cpp \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang++ /usr/bin/clang++ \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/ld.lld /usr/bin/ld.lld \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/lld /usr/bin/lld \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/lld-link /usr/bin/lld-link \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/llvm-lib /usr/bin/llvm-lib \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/llvm-windres /usr/bin/llvm-windres \
&& ln -s clang /usr/lib/llvm-${LLVM_VERSION}/bin/clang-cl

# Add CMake APT repository (https://apt.kitware.com/)
RUN wget -qO- https://apt.kitware.com/keys/kitware-archive-latest.asc | tee /etc/apt/trusted.gpg.d/apt.kitware.com.asc \
# TODO: Update this when a lunar one is available.
&& OS_CODENAME=jammy \
&& add-apt-repository "deb https://apt.kitware.com/ubuntu/ ${OS_CODENAME} main"

# Install CMake
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
cmake \
&& rm -rf /var/lib/apt/lists/*

# RUN ln -s clang-${LLVM_VERSION} /usr/bin/clang \
# && ln -s clang /usr/bin/clang++ \
# && ln -s lld-${LLVM_VERSION} /usr/bin/ld.lld \
# # We also need to setup symlinks ourselves for the MSVC shims because they aren't in the debian packages
# && ln -s clang-${LLVM_VERSION} /usr/lib/llvm-${LLVM_VERSION}/bin/clang-cl \
# && ln -s /usr/lib/llvm-${LLVM_VERSION}/bin/clang-cl /usr/bin/clang-cl \
# && ln -s lld-link-${LLVM_VERSION} /usr/bin/lld-link \
# # Use clang instead of gcc when compiling and linking binaries targeting the host (eg proc macros, build files)
# && update-alternatives --install /usr/bin/cc cc /usr/bin/clang 100 \
# && update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 100 \
# && update-alternatives --install /usr/bin/ld ld /usr/bin/ld.lld 100\

# Note that we're using the full target triple for each variable instead of the
# simple CC/CXX/AR shorthands to avoid issues when compiling any C/C++ code for
# build dependencies that need to compile and execute in the host environment

# ENV CC_x86_64_pc_windows_msvc="clang-cl" \
# CXX_x86_64_pc_windows_msvc="clang-cl" \

# Note that we only disable unused-command-line-argument here since clang-cl
# doesn't implement all of the options supported by cl, but the ones it doesn't
# are _generally_ not interesting.

# CL_FLAGS="-Wno-unused-command-line-argument -fuse-ld=lld-link /imsvc/xwin/crt/include /imsvc/xwin/sdk/include/ucrt /imsvc/xwin/sdk/include/um /imsvc/xwin/sdk/include/shared"

# These are separate since docker/podman won't transform environment variables defined in the same ENV block
# ENV CFLAGS_x86_64_pc_windows_msvc="${CL_FLAGS}" \
# CXXFLAGS_x86_64_pc_windows_msvc="${CL_FLAGS}"

# Install Windows SDK (https://github.com/Jake-Shadle/xwin/blob/97d180c6d537c0bfd52f8ec559c45b247277f156/xwin.dockerfile#LL49C1-L58C1)
ARG XWIN_VERSION="0.2.12"
RUN XWIN_PREFIX="xwin-$XWIN_VERSION-x86_64-unknown-linux-musl"; \
wget -qO- https://github.com/Jake-Shadle/xwin/releases/download/$XWIN_VERSION/$XWIN_PREFIX.tar.gz | tar -xvz -C /usr/local/bin --strip-components=1 $XWIN_PREFIX/xwin \
&& mkdir -p /xwin \
&& xwin --accept-license splat --include-debug-libs --include-debug-symbols --output /xwin

# Install macOS SDK (https://github.com/tpoechtrager/osxcross/blob/ed079949e7aee248ad7e7cb97726cd1c8556afd1/README.md#installation)
ADD https://github.com/tpoechtrager/osxcross.git#564e2b9aa8e7a40da663d890c0e853a1259ff8b1 /osxcross/
WORKDIR /osxcross/tarballs
ARG MACOSX_DEPLOYMENT_TARGET="12.3"
ENV MACOSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}"
RUN wget -q https://github.com/joseluisq/macosx-sdks/releases/download/${MACOSX_DEPLOYMENT_TARGET}/MacOSX${MACOSX_DEPLOYMENT_TARGET}.sdk.tar.xz
WORKDIR /osxcross
RUN UNATTENDED=1 ./build.sh \
&& mkdir -p /osxcross/target/macports \
&& echo "https://packages.macports.org" > /osxcross/target/macports/MIRROR
ENV PATH="${PATH}:/osxcross/target/bin"

# Required for linux build
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
libgles2-mesa-dev=23.0.4-0ubuntu1~23.04.1 \
libxext-dev=2:1.3.4-1build1 \
&& rm -rf /var/lib/apt/lists/*
