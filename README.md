# VSynth - Simple Cross-Platform Synthesizer

A simple but feature-rich software synthesizer built with C++, Qt6, PortAudio, and FFTW.

## Features

- **Cross-platform desktop application** (Linux, Windows, macOS)
- **ADSR envelope control** with adjustable Attack, Decay, Sustain, and Release
- **Polyphonic keyboard input** - play multiple notes simultaneously
- **Multiple waveforms**: Sine, Square, Sawtooth, Triangle, and Noise
- **Multiple oscillators** (2-3 per voice) with slight detuning for richness
- **Vibrato modulation** with adjustable rate and depth
- **Audio effects**: Reverb and Delay
- **Recording and playback** of note events
- **Export capabilities**: WAV audio, MIDI files, and note event text files
- **Real-time FFT analysis** with frequency visualization
- **Interactive piano keyboard** with mouse and computer keyboard support

## Dependencies

### Required Libraries
- **Qt6** (Core and Widgets modules)
- **PortAudio** (cross-platform audio I/O)
- **FFTW3** (Fast Fourier Transform library)
- **CMake** (build system)
- **C++17 compatible compiler**

### Installation on Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake pkg-config
sudo apt install qt6-base-dev qt6-base-dev-tools
sudo apt install libportaudio2 libportaudio-ocaml-dev portaudio19-dev
sudo apt install libfftw3-dev libfftw3-doc
```

### Installation on Fedora/RHEL
```bash
sudo dnf install gcc-c++ cmake pkgconfig
sudo dnf install qt6-qtbase-devel
sudo dnf install portaudio-devel
sudo dnf install fftw-devel
```

### Installation on macOS (with Homebrew)
```bash
brew install cmake pkg-config
brew install qt6
brew install portaudio
brew install fftw
```

### Installation on Windows
- Install Qt6 from the official Qt installer
- Install vcpkg and use it to install PortAudio and FFTW:
```cmd
vcpkg install portaudio:x64-windows
vcpkg install fftw3:x64-windows
```

## Building

### Linux/macOS
```bash
# Clone or extract the project
cd vsynth

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make -j$(nproc)

# Run
./vsynth
```

### Windows (Visual Studio)
```cmd
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

## Usage

### Keyboard Controls
- **Computer keyboard**: Use keys `awsedftgyhujkolp;'` to play notes
- **Mouse**: Click on the piano keyboard to play notes
- **ADSR Controls**: Adjust Attack, Decay, Sustain, and Release parameters
- **Oscillators**: Choose waveform type and number of oscillators
- **Vibrato**: Control vibrato rate and depth
- **Effects**: Adjust reverb and delay amounts

### Recording and Playback
1. Click "Record" to start recording your performance
2. Play notes using keyboard or mouse
3. Click "Stop Recording" when finished
4. Click "Play" to playback your recorded performance
5. Use "Export..." to save as WAV, MIDI, or text file

### FFT Visualization
The frequency analysis display shows the real-time frequency content of the audio output, helping you visualize the harmonic content of different waveforms and effects.

## Architecture

The synthesizer is built with a modular architecture:

- **AudioEngine**: Manages PortAudio integration and audio callback
- **Synthesizer**: Handles voice management and polyphony
- **Voice**: Individual note instances with oscillators and envelope
- **Oscillator**: Generates different waveforms
- **ADSREnvelope**: Amplitude envelope for each voice
- **Effects**: Reverb and delay processing
- **Recorder**: Note event recording and audio export
- **FFTAnalyzer**: Real-time frequency analysis
- **KeyboardWidget**: Interactive piano keyboard GUI
- **MainWindow**: Main application interface

## Troubleshooting

### Audio Issues
- Make sure PortAudio can access your audio device
- Try running with different audio backends
- Check system audio settings and permissions

### Build Issues
- Ensure all dependencies are properly installed
- Check CMake can find Qt6, PortAudio, and FFTW
- Verify compiler supports C++17

### Qt Issues
If Qt6 is not available, you can modify the CMakeLists.txt to use Qt5:
```cmake
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
target_link_libraries(vsynth Qt5::Core Qt5::Widgets ...)
```

## License

This project is provided as-is for educational and personal use.

## Contributing

Feel free to submit issues and enhancement requests!
