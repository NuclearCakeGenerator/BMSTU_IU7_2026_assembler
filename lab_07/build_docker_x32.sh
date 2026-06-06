#!/bin/bash
# Build x32 version inside Docker using a prebuilt x86 cross-toolchain.

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "Building x32 inside Docker container..."

if ! command -v docker &> /dev/null; then
    echo "✗ Docker is not installed"
    exit 1
fi

docker run --rm -v "$SCRIPT_DIR":/work -w /work dockcross/linux-x86 bash -lc '
    set -e
    echo "Compiling x32 version inside Docker..."
    g++ -m32 -std=c++20 -O0 x32.cpp string_ops32.s -o x32_app.exe
    echo "✓ x32_app built successfully inside Docker"
    ./x32_app.exe
'
