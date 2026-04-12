#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/daemon/build"

cd "$BUILD"
if [ -e phantomboard-daemon ]; then
    ./phantomboard-daemon $1
else
    echo "No phantomboard-daemon file"
    exit 1
fi
