#!/bin/bash

# VSynth Installation Script
# This script installs VSynth system-wide on Linux

set -e

APP_NAME="VSynth"
VERSION="1.0.0"
INSTALL_PREFIX="/usr/local"
DESKTOP_FILE_DIR="/usr/share/applications"
ICON_DIR="/usr/share/pixmaps"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

check_root() {
    if [[ $EUID -eq 0 ]]; then
        print_error "This script should not be run as root for safety reasons."
        print_status "Please run as a regular user. The script will ask for sudo when needed."
        exit 1
    fi
}

check_dependencies() {
    print_status "Checking dependencies..."
    
    local missing_deps=()
    
    # Check for build tools
    if ! command -v cmake &> /dev/null; then
        missing_deps+=("cmake")
    fi
    
    if ! command -v make &> /dev/null; then
        missing_deps+=("make")
    fi
    
    if ! command -v g++ &> /dev/null; then
        missing_deps+=("g++")
    fi
    
    # Check for Qt6
    if ! pkg-config --exists Qt6Core; then
        missing_deps+=("qt6-base-dev")
    fi
    
    # Check for PortAudio
    if ! pkg-config --exists portaudio-2.0; then
        missing_deps+=("portaudio19-dev")
    fi
    
    # Check for FFTW
    if ! pkg-config --exists fftw3f; then
        missing_deps+=("libfftw3-dev")
    fi
    
    if [ ${#missing_deps[@]} -ne 0 ]; then
        print_error "Missing dependencies: ${missing_deps[*]}"
        print_status "Please install them first:"
        
        # Detect package manager and suggest installation command
        if command -v apt &> /dev/null; then
            echo "sudo apt install ${missing_deps[*]}"
        elif command -v dnf &> /dev/null; then
            echo "sudo dnf install cmake make gcc-c++ qt6-qtbase-devel portaudio-devel fftw-devel"
        elif command -v pacman &> /dev/null; then
            echo "sudo pacman -S cmake make gcc qt6-base portaudio fftw"
        else
            echo "Please install the missing dependencies using your package manager."
        fi
        
        exit 1
    fi
    
    print_success "All dependencies are satisfied"
}

build_application() {
    print_status "Building VSynth..."
    
    # Clean previous build
    if [ -d "build" ]; then
        rm -rf build
    fi
    
    mkdir build
    cd build
    
    # Configure with CMake
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX"
    
    # Build
    make -j$(nproc)
    
    # Check if build was successful
    if [ ! -f "vsynth" ]; then
        print_error "Build failed - executable not found"
        exit 1
    fi
    
    cd ..
    print_success "Build completed successfully"
}

install_application() {
    print_status "Installing VSynth..."
    
    # Install binary
    sudo install -Dm755 build/vsynth "$INSTALL_PREFIX/bin/vsynth"
    print_success "Installed binary to $INSTALL_PREFIX/bin/vsynth"
    
    # Install headers (for development)
    sudo mkdir -p "$INSTALL_PREFIX/include"
    sudo cp -r include/vsynth "$INSTALL_PREFIX/include/"
    print_success "Installed headers to $INSTALL_PREFIX/include/vsynth"
    
    # Install documentation
    sudo mkdir -p "$INSTALL_PREFIX/share/doc/vsynth"
    sudo cp README.md INSTALL.md PROJECT_SUMMARY.md "$INSTALL_PREFIX/share/doc/vsynth/"
    if [ -f "package/LICENSE.txt" ]; then
        sudo cp package/LICENSE.txt "$INSTALL_PREFIX/share/doc/vsynth/"
    fi
    print_success "Installed documentation to $INSTALL_PREFIX/share/doc/vsynth"
}

create_desktop_entry() {
    print_status "Creating desktop entry..."
    
    # Create desktop file
    cat > /tmp/vsynth.desktop << EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=VSynth
Comment=Cross-platform Software Synthesizer
Exec=$INSTALL_PREFIX/bin/vsynth
Icon=vsynth
Terminal=false
Categories=AudioVideo;Audio;Music;
Keywords=synthesizer;music;audio;midi;
StartupNotify=true
EOF
    
    sudo install -Dm644 /tmp/vsynth.desktop "$DESKTOP_FILE_DIR/vsynth.desktop"
    rm /tmp/vsynth.desktop
    
    print_success "Created desktop entry"
}

create_uninstaller() {
    print_status "Creating uninstaller..."
    
    cat > /tmp/uninstall-vsynth.sh << 'EOF'
#!/bin/bash

# VSynth Uninstaller

set -e

INSTALL_PREFIX="/usr/local"
DESKTOP_FILE_DIR="/usr/share/applications"

echo "Uninstalling VSynth..."

# Remove binary
sudo rm -f "$INSTALL_PREFIX/bin/vsynth"

# Remove headers
sudo rm -rf "$INSTALL_PREFIX/include/vsynth"

# Remove documentation
sudo rm -rf "$INSTALL_PREFIX/share/doc/vsynth"

# Remove desktop entry
sudo rm -f "$DESKTOP_FILE_DIR/vsynth.desktop"

# Remove this uninstaller
sudo rm -f "$INSTALL_PREFIX/bin/uninstall-vsynth"

echo "VSynth has been uninstalled successfully."
EOF
    
    sudo install -Dm755 /tmp/uninstall-vsynth.sh "$INSTALL_PREFIX/bin/uninstall-vsynth"
    rm /tmp/uninstall-vsynth.sh
    
    print_success "Created uninstaller at $INSTALL_PREFIX/bin/uninstall-vsynth"
}

update_desktop_database() {
    print_status "Updating desktop database..."
    
    if command -v update-desktop-database &> /dev/null; then
        sudo update-desktop-database "$DESKTOP_FILE_DIR" 2>/dev/null || true
        print_success "Desktop database updated"
    else
        print_warning "update-desktop-database not found, skipping"
    fi
}

main() {
    echo "======================================"
    echo "       VSynth Installer v$VERSION"
    echo "======================================"
    echo
    
    check_root
    check_dependencies
    build_application
    install_application
    create_desktop_entry
    create_uninstaller
    update_desktop_database
    
    echo
    print_success "VSynth has been installed successfully!"
    echo
    print_status "You can now:"
    echo "  • Run 'vsynth' from the command line"
    echo "  • Launch VSynth from your application menu"
    echo "  • Uninstall with 'sudo uninstall-vsynth'"
    echo
    print_status "Installation locations:"
    echo "  • Binary: $INSTALL_PREFIX/bin/vsynth"
    echo "  • Headers: $INSTALL_PREFIX/include/vsynth/"
    echo "  • Documentation: $INSTALL_PREFIX/share/doc/vsynth/"
    echo "  • Desktop entry: $DESKTOP_FILE_DIR/vsynth.desktop"
    echo
}

# Run main function
main "$@"
