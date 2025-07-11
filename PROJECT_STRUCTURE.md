# VSynth Project Structure

This document describes the complete project structure and organization of the VSynth synthesizer.

## 📁 Directory Layout

```
vsynth/
├── 📁 include/vsynth/          # Header files (C++ interfaces)
│   ├── ADSREnvelope.h          # ADSR envelope generator
│   ├── AudioEngine.h           # Main audio processing engine
│   ├── Effects.h               # Audio effects (reverb, delay)
│   ├── FFTAnalyzer.h           # Real-time frequency analysis
│   ├── KeyboardWidget.h        # Virtual piano keyboard GUI
│   ├── MainWindow.h            # Main application window
│   ├── Oscillator.h            # Waveform generators
│   ├── Recorder.h              # Audio/MIDI recording
│   └── Synthesizer.h           # Voice management & synthesis
│
├── 📁 src/                     # Source files (C++ implementations)
│   ├── main.cpp                # Application entry point
│   ├── MainWindow.cpp          # Main window implementation
│   ├── AudioEngine.cpp         # Audio engine implementation
│   ├── Synthesizer.cpp         # Synthesizer core logic
│   ├── Oscillator.cpp          # Oscillator implementations
│   ├── ADSREnvelope.cpp        # Envelope generator logic
│   ├── Effects.cpp             # Effects processing
│   ├── Recorder.cpp            # Recording functionality
│   ├── FFTAnalyzer.cpp         # FFT analysis implementation
│   └── KeyboardWidget.cpp      # Keyboard widget implementation
│
├── 📁 package/                 # Distribution & packaging
│   ├── build_macos.sh          # macOS build & packaging script
│   ├── build_windows.bat       # Windows build & packaging script
│   ├── vsynth_installer.nsi    # NSIS installer script (Windows)
│   ├── DISTRIBUTION_GUIDE.md   # Distribution instructions
│   └── LICENSE.txt             # Software license & attributions
│
├── 📁 build/                   # Build artifacts (generated)
│   ├── vsynth                  # Compiled executable
│   ├── CMakeCache.txt          # CMake configuration cache
│   └── [other build files]    # Object files, makefiles, etc.
│
├── 📄 CMakeLists.txt           # CMake build configuration
├── 📄 build.sh                 # Linux build script
├── 📄 install.sh               # Linux system installer
├── 📄 README.md                # User documentation
├── 📄 INSTALL.md               # Installation instructions
├── 📄 PROJECT_SUMMARY.md       # Technical overview
└── 📄 PROJECT_STRUCTURE.md     # This file
```

## 🏗️ Architecture Overview

### Core Components

#### 1. **AudioEngine** (`AudioEngine.h/.cpp`)
- **Purpose**: Central audio processing hub
- **Responsibilities**:
  - PortAudio integration and stream management
  - Real-time audio callback processing
  - Thread-safe parameter updates
  - FFT data collection for analysis
- **Key Features**:
  - 44.1kHz sample rate, 256-sample buffer
  - Mutex-protected audio thread safety
  - Automatic audio device detection

#### 2. **Synthesizer** (`Synthesizer.h/.cpp`)
- **Purpose**: Voice management and synthesis coordination
- **Responsibilities**:
  - Polyphonic voice allocation (up to 16 voices)
  - Note on/off event handling
  - Voice parameter distribution
  - Audio mixing and output
- **Key Features**:
  - Dynamic voice creation/cleanup
  - Per-voice ADSR and oscillator management
  - Global vibrato and modulation

#### 3. **Voice** (defined in `Synthesizer.h/.cpp`)
- **Purpose**: Individual note synthesis
- **Responsibilities**:
  - Multiple oscillator management (1-3 per voice)
  - ADSR envelope application
  - Voice lifecycle management
- **Key Features**:
  - Automatic detuning for richness
  - Velocity-sensitive amplitude
  - Automatic cleanup when envelope completes

#### 4. **Oscillator** (`Oscillator.h/.cpp`)
- **Purpose**: Waveform generation
- **Responsibilities**:
  - Multiple waveform types (sine, square, sawtooth, triangle, noise)
  - Frequency and amplitude control
  - Phase management
- **Key Features**:
  - Band-limited waveforms
  - Real-time frequency modulation
  - Low-pass filtered noise

#### 5. **ADSREnvelope** (`ADSREnvelope.h/.cpp`)
- **Purpose**: Amplitude envelope shaping
- **Responsibilities**:
  - Attack, Decay, Sustain, Release processing
  - State machine management
  - Sample-accurate timing
- **Key Features**:
  - Configurable timing parameters
  - Smooth transitions between states
  - Automatic voice deactivation

#### 6. **Effects** (`Effects.h/.cpp`)
- **Purpose**: Audio effects processing
- **Responsibilities**:
  - Delay effect with feedback
  - Reverb using comb and allpass filters
  - Effect parameter control
- **Key Features**:
  - Professional-quality algorithms
  - Adjustable wet/dry mix
  - Real-time parameter updates

### GUI Components

#### 7. **MainWindow** (`MainWindow.h/.cpp`)
- **Purpose**: Main application interface
- **Responsibilities**:
  - UI layout and widget management
  - Parameter control interfaces
  - Real-time FFT display
  - Recording controls
- **Key Features**:
  - Responsive parameter sliders
  - Visual feedback for all controls
  - Professional layout design

#### 8. **KeyboardWidget** (`KeyboardWidget.h/.cpp`)
- **Purpose**: Virtual piano keyboard
- **Responsibilities**:
  - Visual keyboard representation
  - Mouse and keyboard input handling
  - Note event generation
- **Key Features**:
  - 2-octave range (configurable)
  - Computer keyboard mapping
  - Visual press feedback
  - Polyphonic input support

### Utility Components

#### 9. **Recorder** (`Recorder.h/.cpp`)
- **Purpose**: Audio and MIDI recording
- **Responsibilities**:
  - Note event recording
  - Audio buffer recording
  - Multiple export formats (WAV, MIDI, text)
  - Playback functionality
- **Key Features**:
  - Timestamp-accurate recording
  - Multiple export formats
  - Loop playback capability

#### 10. **FFTAnalyzer** (`FFTAnalyzer.h/.cpp`)
- **Purpose**: Real-time frequency analysis
- **Responsibilities**:
  - FFTW integration
  - Magnitude spectrum calculation
  - Windowing and display preparation
- **Key Features**:
  - 1024-point FFT
  - Hann windowing
  - Logarithmic frequency mapping
  - Real-time visualization

## 🔧 Build System

### CMake Configuration (`CMakeLists.txt`)
- **C++20 standard** for cutting-edge language features
- **Qt6** for cross-platform GUI
- **PortAudio** for cross-platform audio I/O
- **FFTW3** for fast Fourier transforms
- **Automatic MOC/UIC** for Qt meta-object compilation

### Build Scripts
- **`build.sh`**: Linux development build script
- **`install.sh`**: Linux system-wide installer
- **`package/build_macos.sh`**: macOS distribution builder
- **`package/build_windows.bat`**: Windows distribution builder

## 📦 Distribution

### Linux
- **Development**: Use `build.sh` for local builds
- **Installation**: Use `install.sh` for system-wide installation
- **Package**: Creates desktop entry and uninstaller

### macOS
- **Bundle**: Creates `.app` bundle with proper Info.plist
- **DMG**: Distributable disk image with drag-to-install
- **Universal**: Supports both Intel and Apple Silicon

### Windows
- **Portable**: ZIP package with all dependencies
- **Installer**: Professional NSIS installer with registry integration
- **Shortcuts**: Start menu and desktop shortcuts

## 🎯 Design Principles

### 1. **Separation of Concerns**
- Audio processing separated from GUI
- Each component has a single responsibility
- Clear interfaces between modules

### 2. **Thread Safety**
- Mutex protection for audio thread access
- Lock-free audio processing where possible
- Safe parameter updates from GUI thread

### 3. **Cross-Platform Compatibility**
- Standard C++17 features only
- Platform-specific code isolated
- Consistent behavior across platforms

### 4. **Professional Quality**
- Industry-standard audio algorithms
- Proper sample rate handling
- Low-latency audio processing

### 5. **Extensibility**
- Modular component design
- Easy to add new waveforms
- Pluggable effects architecture

## 🚀 Development Workflow

### 1. **Local Development**
```bash
./build.sh          # Build for development
cd build && ./vsynth # Run locally
```

### 2. **System Installation**
```bash
./install.sh        # Install system-wide
vsynth              # Run from anywhere
```

### 3. **Distribution**
```bash
# macOS
cd package && ./build_macos.sh

# Windows
cd package && build_windows.bat
```

## 📊 Code Statistics

- **Total Files**: 25+ files
- **Source Code**: ~2,500 lines of C++
- **Header Files**: 9 interface definitions
- **Implementation Files**: 10 source files
- **Build Scripts**: 4 platform-specific scripts
- **Documentation**: 5 comprehensive guides

## 🎵 Features Implemented

### Core Synthesis
- ✅ Polyphonic synthesis (16 voices)
- ✅ Multiple oscillators per voice (1-3)
- ✅ 5 waveform types
- ✅ ADSR envelope shaping
- ✅ Vibrato and pitch modulation

### Effects
- ✅ Professional reverb
- ✅ Delay with feedback
- ✅ Real-time parameter control

### Recording & Export
- ✅ Note event recording
- ✅ Audio recording
- ✅ WAV export
- ✅ MIDI export
- ✅ Text export

### Analysis
- ✅ Real-time FFT analysis
- ✅ Visual frequency display
- ✅ 32-band spectrum analyzer

### Interface
- ✅ Professional GUI design
- ✅ Virtual piano keyboard
- ✅ Real-time parameter controls
- ✅ Visual feedback

This structure provides a solid foundation for a professional software synthesizer with room for future enhancements and extensions.
