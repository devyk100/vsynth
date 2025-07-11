# GitHub Release Guide for VSynth

This guide explains how to create professional releases on GitHub for the VSynth synthesizer project.

## 🚀 **Quick Release Process**

### 1. **Prepare for Release**
```bash
# Ensure all changes are committed
git add .
git commit -m "Prepare for v1.0.0 release"
git push origin main

# Test build locally
./build.sh
cd build && ./vsynth  # Test the application
```

### 2. **Create and Push Version Tag**
```bash
# Create annotated tag
git tag -a v1.0.0 -m "VSynth v1.0.0 - Professional Software Synthesizer"

# Push tag to GitHub
git push origin v1.0.0
```

### 3. **Automatic Build & Release**
- GitHub Actions will automatically:
  - Build for Linux, macOS, and Windows
  - Create release packages
  - Publish GitHub release with binaries

## 📋 **Detailed Release Workflow**

### **Pre-Release Checklist**

- [ ] All features tested and working
- [ ] Documentation updated (README.md, INSTALL.md)
- [ ] Version number updated in relevant files
- [ ] RELEASE_NOTES.md updated with changes
- [ ] Build tested on all target platforms
- [ ] No critical bugs or issues

### **Version Numbering**

Use [Semantic Versioning](https://semver.org/):
- **v1.0.0** - Major release (breaking changes)
- **v1.1.0** - Minor release (new features)
- **v1.0.1** - Patch release (bug fixes)
- **v2.0.0-beta.1** - Pre-release versions

### **Release Types**

#### **Stable Release**
```bash
git tag -a v1.0.0 -m "VSynth v1.0.0 - Stable Release"
git push origin v1.0.0
```

#### **Beta Release**
```bash
git tag -a v1.1.0-beta.1 -m "VSynth v1.1.0-beta.1 - Beta Release"
git push origin v1.1.0-beta.1
```

#### **Release Candidate**
```bash
git tag -a v1.0.0-rc.1 -m "VSynth v1.0.0-rc.1 - Release Candidate"
git push origin v1.0.0-rc.1
```

## 🔧 **GitHub Actions Workflow**

The `.github/workflows/release.yml` file automatically:

### **Build Matrix**
- **Linux**: Ubuntu latest with Qt6, PortAudio, FFTW3
- **macOS**: macOS latest with Homebrew dependencies
- **Windows**: Windows latest with vcpkg dependencies

### **Artifacts Created**
- **Linux**: `vsynth-linux-x64.tar.gz`
- **macOS**: `VSynth-1.0.0.dmg`
- **Windows**: `VSynth-1.0.0-Setup.exe`

### **Release Assets**
All artifacts are automatically attached to the GitHub release.

## 📝 **Manual Release (Alternative)**

If you prefer manual releases:

### 1. **Build Locally**
```bash
# Linux
./build.sh

# macOS
cd package && ./build_macos.sh

# Windows
cd package && .\build_windows.bat
```

### 2. **Create GitHub Release**
1. Go to your GitHub repository
2. Click "Releases" → "Create a new release"
3. Choose your tag version
4. Fill in release title and description
5. Upload your built binaries
6. Publish release

## 🎯 **Release Best Practices**

### **Release Notes**
- Use the `RELEASE_NOTES.md` template
- Include what's new, changed, and fixed
- Provide installation instructions
- List system requirements
- Include screenshots or demos

### **Asset Naming Convention**
- **Linux**: `vsynth-linux-x64-v1.0.0.tar.gz`
- **macOS**: `VSynth-v1.0.0-macos.dmg`
- **Windows**: `VSynth-v1.0.0-windows-x64.exe`

### **Release Timing**
- Test thoroughly before release
- Release during business hours for support
- Announce on relevant platforms
- Monitor for issues after release

## 🔄 **Hotfix Release Process**

For critical bug fixes:

```bash
# Create hotfix branch
git checkout -b hotfix/v1.0.1

# Make fixes
# ... fix critical bugs ...

# Commit and merge
git add .
git commit -m "Fix critical audio initialization bug"
git checkout main
git merge hotfix/v1.0.1

# Tag and release
git tag -a v1.0.1 -m "VSynth v1.0.1 - Critical Bug Fixes"
git push origin v1.0.1
```

## 📊 **Release Metrics**

Track these metrics for each release:
- Download counts per platform
- Issue reports and resolution time
- User feedback and reviews
- Performance benchmarks

## 🛠️ **Troubleshooting Releases**

### **Build Failures**
- Check GitHub Actions logs
- Verify dependencies are available
- Test build scripts locally
- Update workflow if needed

### **Missing Assets**
- Check artifact upload steps
- Verify file paths in workflow
- Ensure build produces expected files

### **Release Not Created**
- Verify tag format (must start with 'v')
- Check GitHub token permissions
- Ensure workflow triggers correctly

## 📚 **Additional Resources**

### **GitHub Documentation**
- [Creating Releases](https://docs.github.com/en/repositories/releasing-projects-on-github/managing-releases-in-a-repository)
- [GitHub Actions](https://docs.github.com/en/actions)
- [Semantic Versioning](https://semver.org/)

### **VSynth Specific**
- `README.md` - User documentation
- `INSTALL.md` - Installation guide
- `PROJECT_STRUCTURE.md` - Technical overview
- `package/DISTRIBUTION_GUIDE.md` - Distribution details

## 🎉 **Example Release Commands**

### **Complete Release Process**
```bash
# 1. Prepare release
git add .
git commit -m "Prepare v1.0.0 release"
git push origin main

# 2. Create and push tag
git tag -a v1.0.0 -m "VSynth v1.0.0 - Professional Software Synthesizer

Features:
- Polyphonic synthesis with 16 voices
- Multiple waveforms and oscillators
- Professional effects (reverb, delay)
- Real-time FFT analysis
- Cross-platform compatibility
- C++20 modern codebase"

git push origin v1.0.0

# 3. GitHub Actions will automatically:
#    - Build for all platforms
#    - Create release packages
#    - Publish GitHub release
```

### **Quick Patch Release**
```bash
# Fix bugs, commit changes
git add .
git commit -m "Fix audio latency issue"
git push origin main

# Create patch release
git tag -a v1.0.1 -m "VSynth v1.0.1 - Bug Fixes"
git push origin v1.0.1
```

This automated release system ensures consistent, professional releases across all platforms while maintaining high quality and reliability.
