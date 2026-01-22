CC=${CC:-clang}
OUT=build/boids

# sources as array
mapfile -t SRC < <(find src -name '*.c')

# includes & libs as arrays
INC=(-Iinclude)
LIBS=(-lGL -lX11)

# flags
if [ $# -eq 0 ]; then
    CFLAGS=(-std=c99 -Wall -Wextra -g -O0)
else
    CFLAGS=("$@")
fi

mkdir -p build

# array expansion preserves word boundaries
"$CC" "${CFLAGS[@]}" "${INC[@]}" "${SRC[@]}" -o "$OUT" "${LIBS[@]}"
