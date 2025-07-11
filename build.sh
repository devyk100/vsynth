#!/bin/bash

# VSynth Build Script
# This script helps build the VSynth synthesizer project

set -e  # Exit on any error

echo "VSynth Build Script"
echo "==================="

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: CMakeLists.txt not found. Please run this script from the project root directory."
    exit 1
fi

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check for required tools
echo "Checking for required tools..."

if ! command_exists cmake; then
    echo "Error: CMake is not installed. Please install CMake first."
    exit 1
fi

if ! command_exists make; then
    echo "Error: Make is not installed. Please install build tools first."
    exit 1
fi

if ! command_exists pkg-config; then
    echo "Error: pkg-config is not installed. Please install pkg-config first."
    exit 1
fi

echo "✓ Required build tools found"

# Check for dependencies
echo "Checking for dependencies..."

# Check for Qt6
if ! pkg-config --exists Qt6Core Qt6Widgets 2>/dev/null; then
    echo "Warning: Qt6 not found via pkg-config. Trying Qt5..."
    if ! pkg-config --exists Qt5Core Qt5Widgets 2>/dev/null; then
        echo "Error: Neither Qt6 nor Qt5 found. Please install Qt development packages."
        echo "Ubuntu/Debian: sudo apt install qt6-base-dev"
        echo "Fedora: sudo dnf install qt6-qtbase-devel"
        echo "macOS: brew install qt6"
        exit 1
    else
        echo "✓ Qt5 found (will need to modify CMakeLists.txt for Qt5)"
        USE_QT5=1
    fi
else
    echo "✓ Qt6 found"
    USE_QT5=0
fi

# Check for PortAudio
if ! pkg-config --exists portaudio-2.0; then
    echo "Error: PortAudio not found. Please install PortAudio development packages."
    echo "Ubuntu/Debian: sudo apt install portaudio19-dev"
    echo "Fedora: sudo dnf install portaudio-devel"
    echo "macOS: brew install portaudio"
    exit 1
fi
echo "✓ PortAudio found"

# Check for FFTW
if ! pkg-config --exists fftw3; then
    echo "Error: FFTW3 not found. Please install FFTW development packages."
    echo "Ubuntu/Debian: sudo apt install libfftw3-dev"
    echo "Fedora: sudo dnf install fftw-devel"
    echo "macOS: brew install fftw"
    exit 1
fi
echo "✓ FFTW3 found"

# Modify CMakeLists.txt for Qt5 if needed
if [ $USE_QT5 -eq 1 ]; then
    echo "Modifying CMakeLists.txt for Qt5..."
    cp CMakeLists.txt CMakeLists.txt.backup
    sed -i 's/find_package(Qt6 REQUIRED COMPONENTS Core Widgets)/find_package(Qt5 REQUIRED COMPONENTS Core Widgets)/' CMakeLists.txt
    sed -i 's/Qt6::Core/Qt5::Core/g' CMakeLists.txt
    sed -i 's/Qt6::Widgets/Qt5::Widgets/g' CMakeLists.txt
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo "Building VSynth..."
CPU_COUNT=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
make -j$CPU_COUNT

# Check if build was successful
if [ -f "vsynth" ]; then
    echo ""
    echo "✓ Build successful!"
    echo "Executable: $(pwd)/vsynth"
    echo ""
    echo "To run VSynth:"
    echo "  cd build && ./vsynth"
    echo ""
    echo "Or install system-wide:"
    echo "  sudo make install"
else
    echo "✗ Build failed!"
    exit 1
fi

# Restore CMakeLists.txt if we modified it
if [ $USE_QT5 -eq 1 ]; then
    cd ..
    mv CMakeLists.txt.backup CMakeLists.txt
    echo "Note: CMakeLists.txt was temporarily modified for Qt5 and has been restored."
fi
