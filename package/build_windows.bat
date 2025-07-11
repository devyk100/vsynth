@echo off
REM Windows Distribution Build Script for VSynth
REM This script creates a Windows installer using NSIS

echo Building VSynth for Windows Distribution
echo ========================================

REM Check for required tools
where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Error: CMake not found. Please install CMake and add it to PATH.
    exit /b 1
)

where windeployqt >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Error: windeployqt not found. Make sure Qt is properly installed and in PATH.
    exit /b 1
)

REM Set variables
set PROJECT_ROOT=%~dp0..
set BUILD_DIR=%PROJECT_ROOT%\build-windows-release
set PACKAGE_DIR=%PROJECT_ROOT%\package\windows
set APP_NAME=VSynth
set VERSION=1.0.0

echo Project root: %PROJECT_ROOT%
echo Build directory: %BUILD_DIR%
echo Package directory: %PACKAGE_DIR%

REM Clean and create directories
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
if exist "%PACKAGE_DIR%" rmdir /s /q "%PACKAGE_DIR%"
mkdir "%BUILD_DIR%"
mkdir "%PACKAGE_DIR%"

REM Build the application
cd /d "%BUILD_DIR%"
echo Configuring CMake...
cmake "%PROJECT_ROOT%" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_PREFIX_PATH="%Qt6_DIR%" ^
    -G "Visual Studio 17 2022" ^
    -A x64

if %ERRORLEVEL% neq 0 (
    echo Error: CMake configuration failed
    exit /b 1
)

echo Building application...
cmake --build . --config Release --parallel

REM Check if build was successful
if not exist "Release\vsynth.exe" (
    echo Error: Build failed - vsynth.exe not found
    exit /b 1
)

echo Build successful!

REM Create distribution directory
set DIST_DIR=%PACKAGE_DIR%\VSynth
mkdir "%DIST_DIR%"

REM Copy executable
copy "Release\vsynth.exe" "%DIST_DIR%\"

REM Deploy Qt dependencies
echo Deploying Qt dependencies...
cd /d "%DIST_DIR%"
windeployqt --release --no-translations --no-system-d3d-compiler --no-opengl-sw vsynth.exe

REM Copy additional files
copy "%PROJECT_ROOT%\README.md" "%DIST_DIR%\"
copy "%PROJECT_ROOT%\INSTALL.md" "%DIST_DIR%\"

REM Create batch file to run the application
echo @echo off > "%DIST_DIR%\VSynth.bat"
echo cd /d "%%~dp0" >> "%DIST_DIR%\VSynth.bat"
echo start vsynth.exe >> "%DIST_DIR%\VSynth.bat"

echo Distribution directory created: %DIST_DIR%

REM Create ZIP archive
echo Creating ZIP archive...
cd /d "%PACKAGE_DIR%"
powershell -command "Compress-Archive -Path 'VSynth' -DestinationPath 'VSynth-%VERSION%-Windows-x64.zip' -Force"

echo.
echo ✅ Windows distribution build complete!
echo 📁 Distribution folder: %DIST_DIR%
echo 📦 ZIP Archive: %PACKAGE_DIR%\VSynth-%VERSION%-Windows-x64.zip
echo.
echo To install:
echo 1. Extract the ZIP file
echo 2. Run VSynth.bat or vsynth.exe
echo.
echo Note: You may need to install Visual C++ Redistributable if not already installed.

pause
