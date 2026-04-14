#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/ctl/build"

cd "$BUILD"
if [ -e phantomboard-ctl ]; then
    ./phantomboard-ctl $1
else
    echo "No phantomboard-ctl file"
    exit 1
fi
