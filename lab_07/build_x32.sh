#!/bin/bash
# Build x32.cpp (32-bit assignment version)

set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "Building x32 (32-bit version)..."

RESULT=1

# Try native 32-bit compilation first.
if command -v g++ &> /dev/null; then
    g++ -m32 -std=c++20 -O0 x32.cpp string_ops32.s -o x32_app
    RESULT=$?
else
    # Fallback to clang++ if g++ is unavailable.
    clang++ -m32 -std=c++20 -O0 x32.cpp string_ops32.s -o x32_app
    RESULT=$?
fi

if [ $RESULT -ne 0 ]; then
    echo "Native 32-bit build failed; trying Docker fallback..."
    if command -v docker &> /dev/null; then
        "$SCRIPT_DIR/build_docker_x32.sh"
        RESULT=$?
    else
        echo "✗ Build failed"
        echo "Note: 32-bit compilation requires multilib support or Docker."
        exit 1
    fi
fi

if [ $RESULT -eq 0 ]; then
    if [ -x ./x32_app ]; then
        echo "✓ Build successful: x32_app"
        echo "Running tests..."
        ./x32_app.exe
    else
        echo "✗ Build succeeded, but x32_app was not created"
        exit 1
    fi
else
    echo "✗ Build failed"
    exit 1
fi
