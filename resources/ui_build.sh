#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/ui/build"

rm -rf "$BUILD"
mkdir -p "$BUILD"
cmake -S "$DIR/ui" -B "$BUILD"
cmake --build "$BUILD"
