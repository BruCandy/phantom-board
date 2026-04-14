#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/ui/build"

cd "$BUILD"
if [ -e phantomboard-ui ]; then
    ./phantomboard-ui
else
    echo "No phantomboard-ui file"
    exit 1
fi
