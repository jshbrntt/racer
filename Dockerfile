FROM ubuntu:23.04 AS base

RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
ca-certificates \
git \
gnupg \
lsb-release \
make \
software-properties-common \
wget \
patch \
libxml2-dev \
libssl-dev \
zlib1g-dev \
xz-utils \
rc

# Add LLVM APT repository (https://apt.llvm.org/)
RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc \
&& OS_CODENAME=$(lsb_release -sc) \
&& add-apt-repository "deb http://apt.llvm.org/${OS_CODENAME}/ llvm-toolchain-${OS_CODENAME} main"

ARG LLVM_VERSION="16"

# Install clang and lld
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
clang-${LLVM_VERSION} \
llvm-${LLVM_VERSION} \
lld-${LLVM_VERSION}

# Add CMake APT repository (https://apt.kitware.com/)
RUN wget -qO- https://apt.kitware.com/keys/kitware-archive-latest.asc | tee /etc/apt/trusted.gpg.d/apt.kitware.com.asc \
# TODO: Update this when a lunar one is available.
&& OS_CODENAME=jammy \
&& add-apt-repository "deb https://apt.kitware.com/ubuntu/ ${OS_CODENAME} main"

# Install cmake and make
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
cmake \
&& rm -rf /var/lib/apt/lists/*

# COPY --from=xwin /xwin /xwin

# Create missing LLVM symlinks
RUN ln -s clang /usr/lib/llvm-${LLVM_VERSION}/bin/clang-cl \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang /usr/bin/clang \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang-cl /usr/bin/clang-cl \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang-cpp /usr/bin/clang-cpp \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang++ /usr/bin/clang++ \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/lld /usr/bin/lld \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/lld-link /usr/bin/lld-link \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/ld.lld /usr/bin/ld.lld

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
ENV CC_x86_64_pc_windows_msvc="clang-cl" \
CXX_x86_64_pc_windows_msvc="clang-cl" \
# Note that we only disable unused-command-line-argument here since clang-cl
# doesn't implement all of the options supported by cl, but the ones it doesn't
# are _generally_ not interesting.
CL_FLAGS="-Wno-unused-command-line-argument -fuse-ld=lld-link /imsvc/xwin/crt/include /imsvc/xwin/sdk/include/ucrt /imsvc/xwin/sdk/include/um /imsvc/xwin/sdk/include/shared"

# These are separate since docker/podman won't transform environment variables defined in the same ENV block
ENV CFLAGS_x86_64_pc_windows_msvc="${CL_FLAGS}" \
CXXFLAGS_x86_64_pc_windows_msvc="${CL_FLAGS}"

ARG XWIN_VERSION="0.2.12"

# Download WinSDK and WinRT (https://github.com/Jake-Shadle/xwin/blob/main/xwin.dockerfile#LL49C1-L58C1)
RUN XWIN_PREFIX="xwin-$XWIN_VERSION-x86_64-unknown-linux-musl"; \
# Install xwin to cargo/bin via github release. Note you could also just use `cargo install xwin`.
wget -qO- https://github.com/Jake-Shadle/xwin/releases/download/$XWIN_VERSION/$XWIN_PREFIX.tar.gz | tar -xvz -C /usr/local/bin --strip-components=1 $XWIN_PREFIX/xwin \
&& mkdir -p /xwin \
# Splat the CRT and SDK files to /xwin/crt and /xwin/sdk respectively
&& xwin --accept-license splat --include-debug-libs --include-debug-symbols --output /xwin

COPY osxcross /osxcross

WORKDIR /osxcross/tarballs

ENV MACOSX_DEPLOYMENT_TARGET="12.3"
RUN wget -q https://github.com/joseluisq/macosx-sdks/releases/download/${MACOSX_DEPLOYMENT_TARGET}/MacOSX${MACOSX_DEPLOYMENT_TARGET}.sdk.tar.xz

WORKDIR /osxcross

RUN UNATTENDED=1 ./build.sh

ENV PATH="${PATH}:/osxcross/target/bin"

RUN mkdir -p /osxcross/target/macports \
&& echo "https://packages.macports.org" > /osxcross/target/macports/MIRROR

RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
# Required to install freetype via macports
bzip2 \
&& rm -rf /var/lib/apt/lists/*

# SDL_ttf requires freetype when targetting macosx
RUN osxcross-macports install freetype

# Required for linux build
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
libfreetype-dev \
libgles2-mesa-dev \
libxext-dev \
&& rm -rf /var/lib/apt/lists/*
