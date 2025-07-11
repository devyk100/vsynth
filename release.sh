#!/bin/bash

# VSynth Release Script
# This script automates the GitHub release process

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Functions
print_header() {
    echo -e "${BLUE}================================${NC}"
    echo -e "${BLUE}    VSynth Release Script${NC}"
    echo -e "${BLUE}================================${NC}"
    echo
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ $1${NC}"
}

# Check if version is provided
if [ $# -eq 0 ]; then
    print_error "Usage: $0 <version>"
    print_info "Example: $0 1.0.0"
    print_info "Example: $0 1.1.0-beta.1"
    exit 1
fi

VERSION=$1
TAG="v$VERSION"

print_header

# Validate version format
if [[ ! $VERSION =~ ^[0-9]+\.[0-9]+\.[0-9]+(-[a-zA-Z0-9.-]+)?$ ]]; then
    print_error "Invalid version format. Use semantic versioning (e.g., 1.0.0, 1.1.0-beta.1)"
    exit 1
fi

print_info "Preparing release for version: $VERSION"
echo

# Check if we're in a git repository
if [ ! -d ".git" ]; then
    print_error "Not in a git repository"
    exit 1
fi

# Check if working directory is clean
if [ -n "$(git status --porcelain)" ]; then
    print_warning "Working directory is not clean. Uncommitted changes:"
    git status --short
    echo
    read -p "Do you want to continue? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        print_info "Release cancelled"
        exit 1
    fi
fi

# Update VERSION file
echo "$VERSION" > VERSION
print_success "Updated VERSION file"

# Update RELEASE_NOTES.md with current version
sed -i "s/# VSynth v[0-9.]*/# VSynth v$VERSION/" RELEASE_NOTES.md
print_success "Updated RELEASE_NOTES.md"

# Test build
print_info "Testing build..."
if ./build.sh > /dev/null 2>&1; then
    print_success "Build test passed"
else
    print_error "Build test failed. Please fix build issues before releasing."
    exit 1
fi

# Commit version changes
git add VERSION RELEASE_NOTES.md
git commit -m "Prepare v$VERSION release" || true
print_success "Committed version changes"

# Create and push tag
print_info "Creating git tag: $TAG"

# Create annotated tag with release notes
TAG_MESSAGE="VSynth $TAG - Professional Software Synthesizer

$(head -20 RELEASE_NOTES.md | tail -15)"

git tag -a "$TAG" -m "$TAG_MESSAGE"
print_success "Created git tag: $TAG"

# Push changes and tag
print_info "Pushing to GitHub..."
git push origin main
git push origin "$TAG"
print_success "Pushed changes and tag to GitHub"

echo
print_success "Release process completed!"
print_info "GitHub Actions will now:"
print_info "  • Build for Linux, macOS, and Windows"
print_info "  • Create release packages"
print_info "  • Publish GitHub release with binaries"
echo
print_info "Monitor the release at: https://github.com/$(git config --get remote.origin.url | sed 's/.*github.com[:/]\([^/]*\/[^/]*\).*/\1/' | sed 's/\.git$//')/actions"
print_info "Release will be available at: https://github.com/$(git config --get remote.origin.url | sed 's/.*github.com[:/]\([^/]*\/[^/]*\).*/\1/' | sed 's/\.git$//')/releases/tag/$TAG"

echo
print_warning "Next steps:"
print_info "  1. Monitor GitHub Actions for build completion"
print_info "  2. Test downloaded binaries on different platforms"
print_info "  3. Update release notes if needed"
print_info "  4. Announce the release"
