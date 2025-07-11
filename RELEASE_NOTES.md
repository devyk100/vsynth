# VSynth v1.0.0 - Professional Software Synthesizer

## 🎹 What's New

VSynth is a professional-grade software synthesizer built with modern C++20, featuring real-time audio synthesis, effects processing, and cross-platform compatibility.

## ✨ Key Features

### 🎵 **Core Synthesis**
- **Polyphonic synthesis** with up to 16 simultaneous voices
- **Multiple waveforms**: Sine, Square, Sawtooth, Triangle, White Noise
- **Multiple oscillators** per voice (1-3) with automatic detuning for richness
- **ADSR envelope** with real-time parameter control
- **Vibrato modulation** with adjustable rate and depth

### 🎛️ **Effects & Processing**
- **Professional reverb** using Schroeder algorithm
- **Digital delay** with feedback and mix controls
- **Real-time parameter adjustment** with smooth transitions
- **Low-latency audio processing** (~5.8ms at 44.1kHz)

### 🎤 **Recording & Export**
- **Note event recording** with timestamp accuracy
- **Audio recording** (32-bit float internal processing)
- **Multiple export formats**:
  - WAV audio files (16-bit PCM)
  - MIDI files for DAW integration
  - Text files for note event analysis
- **Playback functionality** with loop support

### 📊 **Real-time Analysis**
- **FFT frequency analysis** using FFTW3
- **32-band spectrum display** with logarithmic frequency mapping
- **Visual feedback** for all synthesis parameters

### 🖥️ **Professional Interface**
- **Interactive piano keyboard** (mouse + computer keyboard input)
- **Real-time parameter controls** with visual feedback
- **Cross-platform Qt6 GUI** with responsive design
- **Professional layout** optimized for music production

## 🔧 **Technical Specifications**

- **Language**: C++20 with modern features
- **GUI Framework**: Qt6
- **Audio I/O**: PortAudio (cross-platform)
- **FFT Library**: FFTW3 (optimized)
- **Sample Rate**: 44.1kHz
- **Bit Depth**: 32-bit float (internal), 16-bit (export)
- **Polyphony**: 16 voices
- **Latency**: ~5.8ms (professional grade)

## 📦 **Downloads**

### Linux (x64)
- **vsynth-linux-x64.tar.gz** - Portable executable with documentation
- **Requirements**: Qt6, PortAudio, FFTW3 (install via package manager)

### macOS (Universal)
- **VSynth-1.0.0.dmg** - Universal app bundle (Intel + Apple Silicon)
- **Requirements**: macOS 10.14+ (dependencies included)

### Windows (x64)
- **VSynth-1.0.0-Setup.exe** - Professional installer with all dependencies
- **Requirements**: Windows 10+ (all dependencies included)

## 🚀 **Installation**

### Linux
```bash
# Extract and run
tar -xzf vsynth-linux-x64.tar.gz
cd vsynth-linux-x64
./vsynth

# Or install system-wide
sudo ./install.sh
```

### macOS
```bash
# Mount DMG and drag to Applications
open VSynth-1.0.0.dmg
# Drag VSynth.app to Applications folder
```

### Windows
```bash
# Run installer
VSynth-1.0.0-Setup.exe
# Follow installation wizard
```

## 🎮 **Quick Start**

1. **Launch VSynth** from your applications menu
2. **Play notes** using:
   - Computer keyboard: `awsedftgyhujkolp;'`
   - Mouse clicks on the piano keyboard
3. **Adjust parameters**:
   - ADSR envelope sliders
   - Waveform selection
   - Effects controls
4. **Record and export**:
   - Click "Record" to capture your performance
   - Use "Export" to save as WAV, MIDI, or text

## 🔄 **What's Changed**

### New in v1.0.0
- Initial release with full feature set
- C++20 modern codebase
- Cross-platform compatibility
- Professional audio quality
- Real-time FFT analysis
- Multiple export formats

## 🐛 **Known Issues**

- Audio initialization warnings on systems without audio hardware (normal)
- Some Linux distributions may require additional Qt6 packages

## 🛠️ **Building from Source**

```bash
# Clone repository
git clone https://github.com/yourusername/vsynth.git
cd vsynth

# Install dependencies (Ubuntu/Debian)
sudo apt install qt6-base-dev portaudio19-dev libfftw3-dev cmake

# Build
./build.sh

# Run
cd build && ./vsynth
```

## 📄 **License**

VSynth is released under the MIT License. See LICENSE.txt for details.

## 🙏 **Acknowledgments**

- **Qt6** - Cross-platform GUI framework
- **PortAudio** - Cross-platform audio I/O
- **FFTW3** - Fast Fourier Transform library
- **C++20** - Modern language features

## 📞 **Support**

- **Issues**: Report bugs on GitHub Issues
- **Documentation**: See README.md and INSTALL.md
- **Build Guide**: See PROJECT_STRUCTURE.md

---

**Full Changelog**: https://github.com/yourusername/vsynth/commits/v1.0.0
