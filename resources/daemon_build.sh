#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/build"

rm -rf "$BUILD"
mkdir "$BUILD"
cd "$DIR"
cmake -S . -B build
cmake --build build
