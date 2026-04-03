#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/build"

cd "$BUILD"
if [ -e bunnyboard-daemon ]; then
    ./bunnyboard-daemon
else
    echo "No bunnyboard-daemon file"
    exit 1
fi
