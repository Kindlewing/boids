#!/usr/bin/env bash
set -eu

CC=${CC:-clang}
OUT=build/boids
mapfile -t SRC < <(find src -name '*.c' -not -path "src/spark/platform/*")
mapfile -t PLATFORM_SRC < <(echo src/spark/platform/x11/*.c)

SRC+=("${PLATFORM_SRC[@]}")
INC=(-Iinclude)
LIBS=(-lGL -lX11)
if [ $# -eq 0 ]; then
    CFLAGS=(-std=c99 -Wall -Wextra -g -O0)
else
    CFLAGS=("$@")
fi
mkdir -p build
"$CC" "${CFLAGS[@]}" "${INC[@]}" "${SRC[@]}" -o "$OUT" "${LIBS[@]}"

