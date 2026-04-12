#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/daemon/build"

rm -rf "$BUILD"
mkdir -p "$BUILD"
cmake -S "$DIR/daemon" -B "$BUILD"
cmake --build "$BUILD"
