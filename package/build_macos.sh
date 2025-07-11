#!/bin/bash

# macOS Distribution Build Script for VSynth
# This script creates a macOS .app bundle and .dmg installer

set -e

echo "Building VSynth for macOS Distribution"
echo "====================================="

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "Error: This script must be run on macOS"
    exit 1
fi

# Check for required tools
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake not found. Install with: brew install cmake"
    exit 1
fi

if ! command -v macdeployqt &> /dev/null; then
    echo "Error: macdeployqt not found. Make sure Qt is properly installed."
    exit 1
fi

# Set variables
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build-macos-release"
PACKAGE_DIR="$PROJECT_ROOT/package/macos"
APP_NAME="VSynth"
VERSION="1.0.0"

echo "Project root: $PROJECT_ROOT"
echo "Build directory: $BUILD_DIR"
echo "Package directory: $PACKAGE_DIR"

# Clean and create directories
rm -rf "$BUILD_DIR"
rm -rf "$PACKAGE_DIR"
mkdir -p "$BUILD_DIR"
mkdir -p "$PACKAGE_DIR"

# Build the application
cd "$BUILD_DIR"
echo "Configuring CMake..."
cmake "$PROJECT_ROOT" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15 \
    -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"

echo "Building application..."
make -j$(sysctl -n hw.ncpu)

# Check if build was successful
if [ ! -f "vsynth" ]; then
    echo "Error: Build failed - vsynth executable not found"
    exit 1
fi

echo "Build successful!"

# Create app bundle structure
APP_BUNDLE="$PACKAGE_DIR/$APP_NAME.app"
mkdir -p "$APP_BUNDLE/Contents/MacOS"
mkdir -p "$APP_BUNDLE/Contents/Resources"

# Copy executable
cp "vsynth" "$APP_BUNDLE/Contents/MacOS/$APP_NAME"

# Create Info.plist
cat > "$APP_BUNDLE/Contents/Info.plist" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>$APP_NAME</string>
    <key>CFBundleIdentifier</key>
    <string>com.vsynth.synthesizer</string>
    <key>CFBundleName</key>
    <string>$APP_NAME</string>
    <key>CFBundleDisplayName</key>
    <string>VSynth Synthesizer</string>
    <key>CFBundleVersion</key>
    <string>$VERSION</string>
    <key>CFBundleShortVersionString</key>
    <string>$VERSION</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleSignature</key>
    <string>VSYN</string>
    <key>LSMinimumSystemVersion</key>
    <string>10.15</string>
    <key>NSHighResolutionCapable</key>
    <true/>
    <key>NSMicrophoneUsageDescription</key>
    <string>VSynth needs microphone access for audio input.</string>
</dict>
</plist>
EOF

# Create app icon (if available)
if [ -f "$PROJECT_ROOT/resources/icon.icns" ]; then
    cp "$PROJECT_ROOT/resources/icon.icns" "$APP_BUNDLE/Contents/Resources/"
    echo "    <key>CFBundleIconFile</key>" >> "$APP_BUNDLE/Contents/Info.plist.tmp"
    echo "    <string>icon</string>" >> "$APP_BUNDLE/Contents/Info.plist.tmp"
fi

echo "App bundle created: $APP_BUNDLE"

# Deploy Qt frameworks and dependencies
echo "Deploying Qt frameworks..."
macdeployqt "$APP_BUNDLE" -verbose=2

# Make executable
chmod +x "$APP_BUNDLE/Contents/MacOS/$APP_NAME"

echo "Creating DMG installer..."

# Create temporary DMG directory
DMG_DIR="$PACKAGE_DIR/dmg_temp"
mkdir -p "$DMG_DIR"

# Copy app bundle to DMG directory
cp -R "$APP_BUNDLE" "$DMG_DIR/"

# Create Applications symlink
ln -s /Applications "$DMG_DIR/Applications"

# Create DMG
DMG_NAME="VSynth-$VERSION-macOS.dmg"
hdiutil create -volname "VSynth Installer" \
    -srcfolder "$DMG_DIR" \
    -ov -format UDZO \
    "$PACKAGE_DIR/$DMG_NAME"

# Clean up
rm -rf "$DMG_DIR"

echo ""
echo "✅ macOS distribution build complete!"
echo "📦 App Bundle: $APP_BUNDLE"
echo "💿 DMG Installer: $PACKAGE_DIR/$DMG_NAME"
echo ""
echo "To install:"
echo "1. Open the DMG file"
echo "2. Drag VSynth.app to Applications folder"
echo "3. Run VSynth from Applications"
