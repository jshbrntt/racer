# syntax=docker/dockerfile:1-labs
FROM ubuntu:23.04@sha256:b5d5cddaeb8d2f150660cf8f9a1203dd450ac765e6c07630176ac6486eceaddb AS base
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
git=1:2.39.2-1ubuntu1.1 \
lsb-release=12.0-1ubuntu1 \
make=4.3-4.1build1 \
software-properties-common=0.99.35 \
wget=1.21.3-1ubuntu1 \
&& rm -rf /var/lib/apt/lists/*

FROM base AS llvm
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
lld-${LLVM_VERSION} \
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
&& ln -s clang /usr/lib/llvm-${LLVM_VERSION}/bin/clang-cl

FROM llvm AS llvm-cmake
# Add CMake APT repository (https://apt.kitware.com/)
RUN wget -qO- https://apt.kitware.com/keys/kitware-archive-latest.asc | tee /etc/apt/trusted.gpg.d/apt.kitware.com.asc \
# TODO: Update this when a lunar one is available.
&& OS_CODENAME=jammy \
&& add-apt-repository "deb https://apt.kitware.com/ubuntu/ ${OS_CODENAME} main" \
# Install CMake
&& apt-get update \
&& apt-get install --no-install-recommends --yes \
cmake=3.27.1-0kitware1ubuntu22.04.1 \
&& rm -rf /var/lib/apt/lists/*

FROM base AS windows-sdk
# Install Windows SDK (https://github.com/Jake-Shadle/xwin/blob/97d180c6d537c0bfd52f8ec559c45b247277f156/xwin.dockerfile#LL49C1-L58C1)
ARG XWIN_VERSION="0.2.12"
RUN XWIN_PREFIX="xwin-${XWIN_VERSION}-x86_64-unknown-linux-musl"; \
wget -qO- https://github.com/Jake-Shadle/xwin/releases/download/${XWIN_VERSION}/${XWIN_PREFIX}.tar.gz | tar -xvz -C /usr/local/bin --strip-components=1 ${XWIN_PREFIX}/xwin \
&& mkdir -p /xwin \
&& xwin --accept-license splat --include-debug-libs --output /xwin

FROM llvm-cmake AS macosx-toolchain
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
libssl-dev=3.0.8-1ubuntu1.2 \
libxml2-dev=2.9.14+dfsg-1.1ubuntu0.1 \
patch=2.7.6-7build2 \
xz-utils=5.4.1-0.2 \
zlib1g-dev=1:1.2.13.dfsg-1ubuntu4 \
&& rm -rf /var/lib/apt/lists/*
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

FROM llvm-cmake AS windows
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
zip=3.0-13 \
&& rm -rf /var/lib/apt/lists/*
COPY --from=windows-sdk /xwin /xwin
RUN groupadd --gid 123 docker \
&& useradd --uid 1001 --gid docker --shell /bin/bash --create-home runner
USER ubuntu

FROM llvm-cmake AS macosx
COPY --from=macosx-toolchain /osxcross/target/bin /osxcross/target/bin
COPY --from=macosx-toolchain /osxcross/target/lib /osxcross/target/lib
COPY --from=macosx-toolchain /osxcross/target/SDK/MacOSX12.3.sdk /osxcross/target/SDK/MacOSX12.3.sdk
COPY --from=macosx-toolchain /osxcross/tools/toolchain.cmake /osxcross/tools/toolchain.cmake
ARG MACOSX_DEPLOYMENT_TARGET="12.3"
ENV MACOSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}"
ENV PATH="${PATH}:/osxcross/target/bin"
RUN groupadd --gid 123 docker \
&& useradd --uid 1001 --gid docker --shell /bin/bash --create-home runner
USER ubuntu

FROM llvm-cmake AS linux
# Required for linux build
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
libgles2-mesa-dev=23.0.4-0ubuntu1~23.04.1 \
libxext-dev=2:1.3.4-1build1 \
&& rm -rf /var/lib/apt/lists/*
RUN groupadd --gid 123 docker \
&& useradd --uid 1001 --gid docker --shell /bin/bash --create-home runner
USER ubuntu
