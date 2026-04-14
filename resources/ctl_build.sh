#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/ctl/build"

rm -rf "$BUILD"
mkdir -p "$BUILD"
cmake -S "$DIR/ctl" -B "$BUILD"
cmake --build "$BUILD"
