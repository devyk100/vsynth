cmake_minimum_required(VERSION 3.16)
project(VSynth VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Platform-specific dependency handling
if(APPLE)
    # macOS with Homebrew
    find_library(PORTAUDIO_LIBRARIES portaudio PATHS /opt/homebrew/lib /usr/local/lib)
    find_path(PORTAUDIO_INCLUDE_DIRS portaudio.h PATHS /opt/homebrew/include /usr/local/include)
    find_library(FFTW_LIBRARIES fftw3f PATHS /opt/homebrew/lib /usr/local/lib)
    find_path(FFTW_INCLUDE_DIRS fftw3.h PATHS /opt/homebrew/include /usr/local/include)
    
    if(NOT PORTAUDIO_LIBRARIES)
        message(FATAL_ERROR "PortAudio not found. Install with: brew install portaudio")
    endif()
    if(NOT FFTW_LIBRARIES)
        message(FATAL_ERROR "FFTW3 not found. Install with: brew install fftw")
    endif()
elseif(WIN32)
    # Windows with vcpkg for PortAudio and manual FFTW3
    find_package(portaudio REQUIRED)
    set(PORTAUDIO_LIBRARIES portaudio)
    set(PORTAUDIO_INCLUDE_DIRS "")
    
    # Manual FFTW3 setup for Windows
    if(DEFINED ENV{FFTW_ROOT})
        set(FFTW_ROOT $ENV{FFTW_ROOT})
        find_library(FFTW_LIBRARIES NAMES libfftw3f-3 fftw3f PATHS ${FFTW_ROOT} NO_DEFAULT_PATH)
        find_path(FFTW_INCLUDE_DIRS fftw3.h PATHS ${FFTW_ROOT} NO_DEFAULT_PATH)
        
        if(NOT FFTW_LIBRARIES)
            message(FATAL_ERROR "FFTW3 not found in FFTW_ROOT: ${FFTW_ROOT}")
        endif()
    else()
        message(FATAL_ERROR "FFTW_ROOT environment variable not set for Windows build")
    endif()
else()
    # Linux and other platforms
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)
    pkg_check_modules(FFTW REQUIRED fftw3f)
endif()

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

# Compiler flags (only for Linux where pkg-config is used)
if(NOT APPLE AND NOT WIN32)
    target_compile_options(vsynth PRIVATE ${PORTAUDIO_CFLAGS_OTHER})
    target_compile_options(vsynth PRIVATE ${FFTW_CFLAGS_OTHER})
endif()

# Set Qt MOC
set_target_properties(vsynth PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)
