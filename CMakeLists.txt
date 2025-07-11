cmake_minimum_required(VERSION 3.16)
project(VSynth VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(PkgConfig REQUIRED)

# Find PortAudio
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)

# Find FFTW for FFT analysis
pkg_check_modules(FFTW REQUIRED fftw3f)

# Include directories
include_directories(${PORTAUDIO_INCLUDE_DIRS})
include_directories(${FFTW_INCLUDE_DIRS})
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/AudioEngine.cpp
    src/Synthesizer.cpp
    src/Oscillator.cpp
    src/ADSREnvelope.cpp
    src/Effects.cpp
    src/Recorder.cpp
    src/FFTAnalyzer.cpp
    src/KeyboardWidget.cpp
)

set(HEADERS
    include/vsynth/MainWindow.h
    include/vsynth/AudioEngine.h
    include/vsynth/Synthesizer.h
    include/vsynth/Oscillator.h
    include/vsynth/ADSREnvelope.h
    include/vsynth/Effects.h
    include/vsynth/Recorder.h
    include/vsynth/FFTAnalyzer.h
    include/vsynth/KeyboardWidget.h
)

# Create executable
add_executable(vsynth ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(vsynth 
    Qt6::Core 
    Qt6::Widgets
    ${PORTAUDIO_LIBRARIES}
    ${FFTW_LIBRARIES}
    m
)

# Compiler flags
target_compile_options(vsynth PRIVATE ${PORTAUDIO_CFLAGS_OTHER})
target_compile_options(vsynth PRIVATE ${FFTW_CFLAGS_OTHER})

# Set Qt MOC
set_target_properties(vsynth PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)
