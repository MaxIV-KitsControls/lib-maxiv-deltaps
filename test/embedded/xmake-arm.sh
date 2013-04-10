#!/bin/sh
TOOLCHAIN="arm-cortex_a8-linux-gnueabi"

PATH="${HOME}/x-tools/${TOOLCHAIN}/bin:${PATH}" \
make \
    CC="${TOOLCHAIN}-gcc" \
    CXX="${TOOLCHAIN}-g++" \
    AR="${TOOLCHAIN}-ar" \
    RANLIB="${TOOLCHAIN}-ranlib" \
    $*

