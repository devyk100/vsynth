# VSynth Distribution Guide

This guide explains how to create distributable packages for VSynth on different platforms.

## 📦 **Available Distribution Methods**

### **Linux (Current Platform)**
- ✅ **Native Build**: Already working with `./build.sh`
- 📦 **AppImage**: Portable single-file executable
- 📦 **Flatpak**: Universal Linux package
- 📦 **Snap**: Ubuntu/Canonical package format

### **macOS**
- 🍎 **App Bundle**: Native .app package
- 💿 **DMG Installer**: Disk image with drag-to-install
- 🏪 **Mac App Store**: (requires Apple Developer account)

### **Windows**
- 📁 **Portable ZIP**: Extract and run
- 🔧 **NSIS Installer**: Professional Windows installer
- 🏪 **Microsoft Store**: (requires Microsoft Developer account)

## 🚀 **Quick Start**

### **For macOS Distribution**
```bash
# On macOS machine with Qt6 and dependencies installed:
cd package
./build_macos.sh

# Output:
# - package/macos/VSynth.app (App Bundle)
# - package/macos/VSynth-1.0.0-macOS.dmg (DMG Installer)
```

### **For Windows Distribution**
```batch
REM On Windows machine with Qt6, Visual Studio, and dependencies:
cd package
build_windows.bat

REM Output:
REM - package/windows/VSynth/ (Portable folder)
REM - package/windows/VSynth-1.0.0-Windows-x64.zip (ZIP archive)
```

### **For Windows Installer (NSIS)**
```batch
REM After running build_windows.bat:
REM Install NSIS (https://nsis.sourceforge.io/)
cd package
makensis vsynth_installer.nsi

REM Output:
REM - VSynth-1.0.0-Windows-Installer.exe (Professional installer)
```

## 🛠️ **Prerequisites by Platform**

### **macOS Build Requirements**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake qt6 portaudio fftw

# Verify Qt tools are in PATH
which macdeployqt  # Should show Qt installation path
```

### **Windows Build Requirements**
```batch
REM 1. Install Visual Studio 2022 Community (free)
REM    - Include "Desktop development with C++" workload
REM    - Include CMake tools

REM 2. Install Qt6 for Windows
REM    - Download from https://www.qt.io/download-qt-installer
REM    - Install Qt 6.5+ with MSVC 2022 64-bit

REM 3. Install vcpkg (for PortAudio and FFTW)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install portaudio:x64-windows fftw3:x64-windows

REM 4. Add Qt to PATH
set PATH=%PATH%;C:\Qt\6.5.0\msvc2022_64\bin

REM 5. For NSIS installer, install NSIS
REM    - Download from https://nsis.sourceforge.io/
```

### **Linux Build Requirements (Already Met)**
```bash
# Ubuntu/Debian
sudo apt install qt6-base-dev portaudio19-dev libfftw3-dev cmake

# Fedora/RHEL
sudo dnf install qt6-qtbase-devel portaudio-devel fftw-devel cmake

# Arch Linux
sudo pacman -S qt6-base portaudio fftw cmake
```

## 📋 **Distribution Checklist**

### **Before Building**
- [ ] Update version number in CMakeLists.txt
- [ ] Update version in build scripts
- [ ] Test build on target platform
- [ ] Verify all dependencies are available
- [ ] Update documentation if needed

### **macOS Distribution**
- [ ] Build on macOS 10.15+ for compatibility
- [ ] Test on both Intel and Apple Silicon Macs
- [ ] Verify app bundle launches correctly
- [ ] Test DMG installer process
- [ ] Check code signing (optional but recommended)

### **Windows Distribution**
- [ ] Build with Visual Studio 2022 for compatibility
- [ ] Test on Windows 10 and Windows 11
- [ ] Verify all DLLs are included
- [ ] Test installer and uninstaller
- [ ] Check for missing Visual C++ Redistributables

### **Linux Distribution**
- [ ] Test on multiple distributions (Ubuntu, Fedora, etc.)
- [ ] Verify dependency versions
- [ ] Test with different Qt versions
- [ ] Consider creating AppImage for portability

## 🔧 **Advanced Distribution Options**

### **Cross-Compilation**
```bash
# Build Windows executable from Linux (advanced)
# Requires MinGW-w64 and Qt for MinGW
sudo apt install mingw-w64 qt6-tools-dev-tools

# Build macOS executable from Linux (very advanced)
# Requires OSXCross toolchain - complex setup
```

### **Automated CI/CD**
```yaml
# GitHub Actions example (.github/workflows/build.yml)
name: Build VSynth
on: [push, pull_request]
jobs:
  build-linux:
    runs-on: ubuntu-latest
    # ... build steps for Linux
  
  build-macos:
    runs-on: macos-latest
    # ... build steps for macOS
  
  build-windows:
    runs-on: windows-latest
    # ... build steps for Windows
```

## 📊 **Distribution Sizes (Estimated)**

| Platform | Format | Size | Notes |
|----------|--------|------|-------|
| Linux | Executable | ~150KB | Requires system Qt/PortAudio |
| Linux | AppImage | ~50MB | Self-contained |
| macOS | App Bundle | ~30MB | Includes Qt frameworks |
| macOS | DMG | ~25MB | Compressed app bundle |
| Windows | Portable | ~40MB | Includes Qt DLLs |
| Windows | Installer | ~35MB | Compressed with NSIS |

## 🚨 **Common Issues & Solutions**

### **macOS**
- **"App is damaged"**: Code signing issue - use `codesign` or allow in Security preferences
- **Qt not found**: Ensure Qt is installed via Homebrew or Qt installer
- **Permission denied**: Run `chmod +x build_macos.sh`

### **Windows**
- **Missing DLLs**: Run `windeployqt` to copy Qt dependencies
- **MSVC not found**: Install Visual Studio with C++ workload
- **PortAudio errors**: Install PortAudio via vcpkg

### **Linux**
- **Qt version conflicts**: Use specific Qt version packages
- **Audio permissions**: Add user to audio group
- **Missing libraries**: Install development packages (-dev/-devel)

## 🎯 **Distribution Strategy**

### **Recommended Approach**
1. **Start with ZIP/portable** versions for quick testing
2. **Create proper installers** for production releases
3. **Consider app stores** for wider distribution
4. **Provide multiple formats** to accommodate user preferences

### **Release Channels**
- **Stable**: Tested, stable releases with installers
- **Beta**: Pre-release versions for testing
- **Nightly**: Automated builds from latest code

## 📝 **Legal Considerations**

- **Qt License**: VSynth uses Qt under LGPL - commercial use allowed
- **PortAudio License**: MIT-style license - very permissive
- **FFTW License**: GPL - ensure compliance for commercial use
- **Code Signing**: Recommended for macOS/Windows distribution
- **Notarization**: Required for macOS Gatekeeper (macOS 10.15+)

## 🎉 **Success!**

With these distribution scripts and guides, you can create professional installers for VSynth on all major desktop platforms!

**Next Steps:**
1. Choose your target platform(s)
2. Set up the build environment
3. Run the appropriate build script
4. Test the generated installer
5. Distribute to users!
