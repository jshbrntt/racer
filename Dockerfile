# syntax=docker/dockerfile:1-labs
FROM ubuntu:24.04@sha256:77d57fd89366f7d16615794a5b53e124d742404e20f035c22032233f1826bd6a AS base
ARG DEBIAN_FRONTEND="noninteractive"
ENV TZ="Etc/UTC"
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
git=1:2.43.0-1ubuntu7.1 \
lsb-release=12.0-2 \
make=4.3-4.1build2 \
software-properties-common=0.99.48 \
wget=1.21.4-1ubuntu4.1 \
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
&& OS_CODENAME=$(lsb_release -sc) \
&& add-apt-repository "deb https://apt.kitware.com/ubuntu/ ${OS_CODENAME} main" \
# Install CMake
&& apt-get update \
&& apt-get install --no-install-recommends --yes \
cmake=3.30.2-0kitware1ubuntu24.04.1 \
&& rm -rf /var/lib/apt/lists/*

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
libssl-dev=3.0.13-0ubuntu3.4 \
libxml2-dev=2.9.14+dfsg-1.3ubuntu3 \
patch=2.7.6-7build3 \
xz-utils=5.6.1+really5.4.5-1build0.1 \
zlib1g-dev=1:1.3.dfsg-3.1ubuntu2.1 \
&& rm -rf /var/lib/apt/lists/*
# Install macOS SDK (https://github.com/tpoechtrager/osxcross/blob/ed079949e7aee248ad7e7cb97726cd1c8556afd1/README.md#installation)
ADD https://github.com/tpoechtrager/osxcross.git#be6ffb3cbc6c0228614ebe6a4b5cd2726339ecc9 /osxcross/
WORKDIR /osxcross/tarballs
ARG MACOSX_DEPLOYMENT_TARGET="12.3"
ENV MACOSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}"
RUN wget -q https://github.com/joseluisq/macosx-sdks/releases/download/${MACOSX_DEPLOYMENT_TARGET}/MacOSX${MACOSX_DEPLOYMENT_TARGET}.sdk.tar.xz
WORKDIR /osxcross
RUN UNATTENDED=1 ./build.sh \
&& mkdir -p /osxcross/target/macports \
&& echo "https://packages.macports.org" > /osxcross/target/macports/MIRROR
ENV PATH="${PATH}:/osxcross/target/bin"

FROM llvm-cmake AS devcontainer_windows
RUN apt-get update \
&& apt-get install --no-install-recommends --yes \
zip=3.0-13build1 \
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
libasound2-dev=1.2.11-1build2 \
libgles2-mesa-dev=24.0.9-0ubuntu0.1 \
libxext-dev=2:1.3.4-1build2 \
&& rm -rf /var/lib/apt/lists/*
