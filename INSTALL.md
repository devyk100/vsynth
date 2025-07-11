# VSynth Installation Guide

## Quick Start

### Ubuntu/Debian
```bash
# Install dependencies
sudo apt update
sudo apt install build-essential cmake pkg-config
sudo apt install qt6-base-dev qt6-base-dev-tools
sudo apt install libportaudio2 libportaudio-ocaml-dev portaudio19-dev
sudo apt install libfftw3-dev libfftw3-doc

# Build and run
./build.sh
cd build && ./vsynth
```

### Fedora/RHEL/CentOS
```bash
# Install dependencies
sudo dnf install gcc-c++ cmake pkgconfig
sudo dnf install qt6-qtbase-devel
sudo dnf install portaudio-devel
sudo dnf install fftw-devel

# Build and run
./build.sh
cd build && ./vsynth
```

### macOS (with Homebrew)
```bash
# Install dependencies
brew install cmake pkg-config qt6 portaudio fftw

# Build and run
./build.sh
cd build && ./vsynth
```

## Manual Build

If the build script doesn't work for your system:

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
./vsynth
```

## Troubleshooting

### Qt6 Not Available
If Qt6 is not available on your system, you can use Qt5 instead. The build script will automatically detect this and modify the CMakeLists.txt temporarily.

### Missing Dependencies
Make sure all dependencies are installed:
- CMake (3.16 or later)
- Qt6 or Qt5 (Core and Widgets)
- PortAudio
- FFTW3
- C++17 compatible compiler

### Audio Issues
- Check that your system has audio output devices
- Verify PortAudio can access your audio hardware
- Try running with different audio backends

### Permission Issues
On some systems, you might need to add your user to the audio group:
```bash
sudo usermod -a -G audio $USER
```
Then log out and log back in.

## System Requirements

- **OS**: Linux, macOS, or Windows
- **RAM**: 512MB minimum, 1GB recommended
- **CPU**: Any modern x86_64 or ARM64 processor
- **Audio**: Any audio output device supported by PortAudio

## Performance Notes

- The synthesizer uses real-time audio processing
- Lower buffer sizes provide lower latency but require more CPU
- FFT analysis can be disabled if CPU usage is too high
- Multiple oscillators per voice increase CPU usage
