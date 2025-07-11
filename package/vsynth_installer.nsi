; VSynth NSIS Installer Script
; Creates a Windows installer for VSynth Synthesizer

!define APP_NAME "VSynth"
!define APP_VERSION "1.0.0"
!define APP_PUBLISHER "VSynth Team"
!define APP_URL "https://github.com/vsynth/vsynth"
!define APP_DESCRIPTION "Cross-platform Software Synthesizer"

; Include Modern UI
!include "MUI2.nsh"
!include "FileFunc.nsh"

; General
Name "${APP_NAME} ${APP_VERSION}"
OutFile "VSynth-${APP_VERSION}-Windows-Installer.exe"
Unicode True

; Default installation folder
InstallDir "$PROGRAMFILES64\${APP_NAME}"

; Get installation folder from registry if available
InstallDirRegKey HKCU "Software\${APP_NAME}" ""

; Request application privileges
RequestExecutionLevel admin

; Variables
Var StartMenuFolder

; Interface Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "LICENSE.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY

; Start Menu Folder Page Configuration
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\${APP_NAME}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Languages
!insertmacro MUI_LANGUAGE "English"

; License file
LicenseData "LICENSE.txt"

; Version Information
VIProductVersion "${APP_VERSION}.0"
VIAddVersionKey "ProductName" "${APP_NAME}"
VIAddVersionKey "Comments" "${APP_DESCRIPTION}"
VIAddVersionKey "CompanyName" "${APP_PUBLISHER}"
VIAddVersionKey "LegalCopyright" "© 2024 ${APP_PUBLISHER}"
VIAddVersionKey "FileDescription" "${APP_NAME} Installer"
VIAddVersionKey "FileVersion" "${APP_VERSION}.0"
VIAddVersionKey "ProductVersion" "${APP_VERSION}.0"
VIAddVersionKey "InternalName" "${APP_NAME}"
VIAddVersionKey "OriginalFilename" "VSynth-${APP_VERSION}-Windows-Installer.exe"

; Installer Sections
Section "VSynth Application" SecApp
  SectionIn RO
  
  ; Set output path to the installation directory
  SetOutPath $INSTDIR
  
  ; Copy main executable
  File "windows\VSynth\vsynth.exe"
  
  ; Copy Qt libraries and dependencies
  File /r "windows\VSynth\*.dll"
  File /r "windows\VSynth\platforms"
  File /r "windows\VSynth\styles"
  File /r "windows\VSynth\imageformats"
  
  ; Copy documentation
  File "..\README.md"
  File "..\INSTALL.md"
  File "..\PROJECT_SUMMARY.md"
  
  ; Store installation folder
  WriteRegStr HKCU "Software\${APP_NAME}" "" $INSTDIR
  
  ; Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  ; Add to Add/Remove Programs
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                   "DisplayName" "${APP_NAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                   "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                   "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                   "DisplayIcon" "$INSTDIR\vsynth.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                   "Publisher" "${APP_PUBLISHER}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                   "URLInfoAbout" "${APP_URL}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                   "DisplayVersion" "${APP_VERSION}"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                     "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                     "NoRepair" 1
  
  ; Get installed size
  ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
  IntFmt $0 "0x%08X" $0
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" \
                     "EstimatedSize" "$0"
SectionEnd

Section "Start Menu Shortcuts" SecStartMenu
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ; Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\${APP_NAME}.lnk" "$INSTDIR\vsynth.exe"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
    
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section "Desktop Shortcut" SecDesktop
  CreateShortcut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\vsynth.exe"
SectionEnd

Section "File Associations" SecFileAssoc
  ; Associate .vsynth files with the application
  WriteRegStr HKCR ".vsynth" "" "VSynth.Project"
  WriteRegStr HKCR "VSynth.Project" "" "VSynth Project File"
  WriteRegStr HKCR "VSynth.Project\DefaultIcon" "" "$INSTDIR\vsynth.exe,0"
  WriteRegStr HKCR "VSynth.Project\shell\open\command" "" '"$INSTDIR\vsynth.exe" "%1"'
SectionEnd

; Section Descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecApp} "The main VSynth application and required files."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecStartMenu} "Add shortcuts to the Start Menu."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktop} "Add a shortcut to the Desktop."
  !insertmacro MUI_DESCRIPTION_TEXT ${SecFileAssoc} "Associate .vsynth files with VSynth."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

; Uninstaller Section
Section "Uninstall"
  ; Remove files and uninstaller
  Delete "$INSTDIR\vsynth.exe"
  Delete "$INSTDIR\*.dll"
  RMDir /r "$INSTDIR\platforms"
  RMDir /r "$INSTDIR\styles"
  RMDir /r "$INSTDIR\imageformats"
  Delete "$INSTDIR\README.md"
  Delete "$INSTDIR\INSTALL.md"
  Delete "$INSTDIR\PROJECT_SUMMARY.md"
  Delete "$INSTDIR\Uninstall.exe"
  
  ; Remove directories
  RMDir "$INSTDIR"
  
  ; Remove Start Menu shortcuts
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  Delete "$SMPROGRAMS\$StartMenuFolder\${APP_NAME}.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  ; Remove Desktop shortcut
  Delete "$DESKTOP\${APP_NAME}.lnk"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"
  DeleteRegKey HKCU "Software\${APP_NAME}"
  
  ; Remove file associations
  DeleteRegKey HKCR ".vsynth"
  DeleteRegKey HKCR "VSynth.Project"
SectionEnd

; Functions
Function .onInit
  ; Check if already installed
  ReadRegStr $R0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}" "UninstallString"
  StrCmp $R0 "" done
  
  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "${APP_NAME} is already installed. $\n$\nClick `OK` to remove the \
  previous version or `Cancel` to cancel this upgrade." \
  IDOK uninst
  Abort
  
  uninst:
    ClearErrors
    ExecWait '$R0 _?=$INSTDIR'
    
    IfErrors no_remove_uninstaller done
      Delete $R0
      RMDir $INSTDIR
    
  no_remove_uninstaller:
  done:
FunctionEnd
