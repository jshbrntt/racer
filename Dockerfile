# syntax=docker/dockerfile:1-labs
FROM ubuntu:20.04 AS base
ARG DEBIAN_FRONTEND="noninteractive"
ENV TZ="Etc/UTC"
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
git \
lsb-release \
make \
software-properties-common \
wget \
&& rm -rf /var/lib/apt/lists/*
# Add GitHub Actions runner user
RUN groupadd --gid 127 docker \
&& useradd --uid 1001 --gid docker --shell /bin/bash --create-home runner

FROM base AS llvm
# Add LLVM APT repository (https://apt.llvm.org/)
RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc \
&& OS_CODENAME=$(lsb_release -sc) \
&& add-apt-repository "deb http://apt.llvm.org/${OS_CODENAME}/ llvm-toolchain-${OS_CODENAME} main"
ARG LLVM_VERSION="20"
# Install LLVM
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
clang-${LLVM_VERSION} \
libc++-${LLVM_VERSION}-dev \
lld-${LLVM_VERSION} \
llvm-${LLVM_VERSION} \
&& rm -rf /var/lib/apt/lists/* \
# Create missing LLVM symlinks
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang /usr/bin/clang \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang-cl /usr/bin/clang-cl \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang-cpp /usr/bin/clang-cpp \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/clang++ /usr/bin/clang++ \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/ld.lld /usr/bin/ld.lld \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/lld /usr/bin/lld \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/lld-link /usr/bin/lld-link \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/llvm-lib /usr/bin/llvm-lib \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/llvm-windres /usr/bin/llvm-windres \
&& ln -s ../lib/llvm-${LLVM_VERSION}/bin/dsymutil /usr/bin/dsymutil \
&& ln -s clang /usr/lib/llvm-${LLVM_VERSION}/bin/clang-cl

FROM llvm AS llvm-cmake
# Add CMake APT repository (https://apt.kitware.com/)
RUN wget -qO- https://apt.kitware.com/keys/kitware-archive-latest.asc | tee /etc/apt/trusted.gpg.d/apt.kitware.com.asc \
# TODO: Update this when a lunar one is available.
&& OS_CODENAME=$(lsb_release -sc) \
&& add-apt-repository "deb https://apt.kitware.com/ubuntu/ ${OS_CODENAME} main" \
# Install CMake
&& apt-get update \
&& apt-get install --no-install-recommends --yes \
cmake \
ninja-build \
&& rm -rf /var/lib/apt/lists/*
# Set CMake generator to Ninja
ENV CMAKE_GENERATOR="Ninja"

FROM base AS windows-sdk
# Install Windows SDK (https://github.com/Jake-Shadle/xwin/blob/97d180c6d537c0bfd52f8ec559c45b247277f156/xwin.dockerfile#LL49C1-L58C1)
ARG XWIN_VERSION="0.6.5"
RUN XWIN_PREFIX="xwin-${XWIN_VERSION}-x86_64-unknown-linux-musl"; \
wget -qO- https://github.com/Jake-Shadle/xwin/releases/download/${XWIN_VERSION}/${XWIN_PREFIX}.tar.gz | tar -xvz -C /usr/local/bin --strip-components=1 ${XWIN_PREFIX}/xwin \
&& mkdir -p /xwin \
&& xwin --accept-license splat --include-debug-libs --output /xwin

FROM llvm-cmake AS macosx-toolchain
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
libssl-dev \
libxml2-dev \
patch \
xz-utils \
zlib1g-dev \
&& rm -rf /var/lib/apt/lists/*
# Install macOS SDK (https://github.com/tpoechtrager/osxcross/blob/ed079949e7aee248ad7e7cb97726cd1c8556afd1/README.md#installation)
ADD https://github.com/tpoechtrager/osxcross.git#be6ffb3cbc6c0228614ebe6a4b5cd2726339ecc9 /osxcross/
WORKDIR /osxcross/tarballs
ARG MACOSX_DEPLOYMENT_TARGET="12.3"
ENV MACOSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}"
RUN wget -q https://github.com/joseluisq/macosx-sdks/releases/download/${MACOSX_DEPLOYMENT_TARGET}/MacOSX${MACOSX_DEPLOYMENT_TARGET}.sdk.tar.xz
WORKDIR /osxcross
# osxcross and dependencies appear to use these environment variables to control the build config
ARG NINJA="1"
ARG MAKE="ninja"
ARG JOBS="4"
ARG UNATTENDED="1"
RUN ./build.sh \
&& mkdir -p /osxcross/target/macports \
&& echo "https://packages.macports.org" > /osxcross/target/macports/MIRROR
ENV PATH="${PATH}:/osxcross/target/bin"

FROM llvm-cmake AS devcontainer_windows
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
zip \
&& rm -rf /var/lib/apt/lists/*
COPY --from=windows-sdk /xwin /xwin

FROM llvm-cmake AS devcontainer_macosx
COPY --from=macosx-toolchain /osxcross/target/bin /osxcross/target/bin
COPY --from=macosx-toolchain /osxcross/target/lib /osxcross/target/lib
COPY --from=macosx-toolchain /osxcross/target/SDK/MacOSX12.3.sdk /osxcross/target/SDK/MacOSX12.3.sdk
COPY --from=macosx-toolchain /osxcross/tools/toolchain.cmake /osxcross/tools/toolchain.cmake
ARG MACOSX_DEPLOYMENT_TARGET="12.3"
ENV MACOSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}"
ENV PATH="${PATH}:/osxcross/target/bin"

FROM llvm-cmake AS devcontainer_linux
# Required for linux build
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
libasound2-dev \
libgles2-mesa-dev \
libxext-dev \
&& rm -rf /var/lib/apt/lists/*
