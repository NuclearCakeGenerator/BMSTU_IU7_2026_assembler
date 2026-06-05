#!/bin/bash
# Build x32 version inside Docker (i386/ubuntu)
# For Linux systems where native 32-bit compilation isn't available

echo "Building x32 inside Docker container..."

docker run --rm -v "$PWD":/work -w /work i386/ubuntu:24.04 bash -c '
    apt-get update -qq
    apt-get install -y -qq build-essential > /dev/null 2>&1
    
    echo "Compiling x32 version inside Docker..."
    g++ -m32 -std=c++20 -O0 x32.cpp string_ops32.s -o x32_app
    
    if [ $? -eq 0 ]; then
        echo "✓ x32_app built successfully inside Docker"
        ./x32_app
    else
        echo "✗ Build failed"
        exit 1
    fi
'
