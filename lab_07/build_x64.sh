#!/bin/bash
# Build x64.cpp (64-bit assignment version)

echo "Building x64 (64-bit version)..."
clang++ -std=c++20 -O0 x64.cpp string_ops64.s -o x64_app.exe

if [ $? -eq 0 ]; then
    echo "✓ Build successful: x64_app"
    echo "Running tests..."
    ./x64_app.exe
else
    echo "✗ Build failed"
    exit 1
fi

