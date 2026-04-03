#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/build"

mkdir -p "$BUILD"
cd "$DIR"
cmake -S . -B build
cmake --build build
