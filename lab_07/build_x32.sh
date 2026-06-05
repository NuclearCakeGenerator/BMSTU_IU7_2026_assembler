#!/bin/bash
# Build x32.cpp (32-bit assignment version)
# Note: Requires g++ with multilib support or Linux environment

echo "Building x32 (32-bit version)..."

# Try with g++ first (more likely to have multilib)
if command -v g++ &> /dev/null; then
    g++ -m32 -std=c++20 -O0 x32.cpp string_ops32.s -o x32_app
    RESULT=$?
else
    # Fallback to clang++
    clang++ -m32 -std=c++20 -O0 x32.cpp string_ops32.s -o x32_app
    RESULT=$?
fi

if [ $RESULT -eq 0 ]; then
    echo "✓ Build successful: x32_app"
    echo "Running tests..."
    ./x32_app
else
    echo "✗ Build failed"
    echo "Note: 32-bit compilation requires multilib support (Linux with g++-multilib or Docker)"
    exit 1
fi
