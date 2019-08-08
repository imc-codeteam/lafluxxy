; NSIS Installer script
;
; This script constructs the Veevee installer for Windows
 
!define setup "lafluxxy_VERSION_installer_win64.exe"
 
; change this to wherever the files to be packaged reside
!define srcdir "."
 
!define company "Inorganic Materials & Catalysis"
 
!define prodname "lafluxxy"
!define exec "lafluxxy.exe"
 
; optional stuff
 
; Set the text which prompts the user to enter the installation directory
; DirText "My Description Here."
 
; icons must be Microsoft .ICO files
!define icon "lafluxxy_logo.ico"
 
; registry stuff
 
!define regkey "Software\${company}\${prodname}"
!define uninstkey "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}"
 
!define startmenu "$SMPROGRAMS\${company}\${prodname}"
!define uninstaller "uninstall.exe"

;!define licensefile <path to license file>
 
;--------------------------------
 
XPStyle off
ShowInstDetails hide
ShowUninstDetails hide
 
Name "${prodname}"
Caption "${prodname}"
 
!ifdef icon
Icon "bin/${icon}"
!endif
 
OutFile "${setup}"
SetCompressor /SOLID LZMA
RequestExecutionLevel admin
 
SetDateSave on
SetDatablockOptimize on
CRCCheck on
SilentInstall normal
 
InstallDir "$PROGRAMFILES\${company}\${prodname}"
InstallDirRegKey HKLM "${regkey}" ""
 
!ifdef licensefile
LicenseText "License"
LicenseData "${srcdir}\${licensefile}"
!endif
 
; pages
; we keep it simple - leave out selectable installation types
 
!ifdef licensefile
Page license
!endif
 
; Page components
Page directory
Page instfiles
 
UninstPage uninstConfirm
UninstPage instfiles
 
;--------------------------------
 
AutoCloseWindow false
ShowInstDetails show
 
; beginning (invisible) section
Section
 
  WriteRegStr HKLM "${regkey}" "Install_Dir" "$INSTDIR"
  ; write uninstall strings
  WriteRegStr HKLM "${uninstkey}" "DisplayName" "${prodname} (remove only)"
  WriteRegStr HKLM "${uninstkey}" "UninstallString" '"$INSTDIR\${uninstaller}"'
 
!ifdef filetype
  WriteRegStr HKCR "${filetype}" "" "${prodname}"
!endif
 
  WriteRegStr HKCR "${prodname}\Shell\open\command\" "" '"$INSTDIR\bin\${exec} "%1"'
 
!ifdef icon
  WriteRegStr HKCR "${prodname}\DefaultIcon" "" "$INSTDIR\bingui\${icon}"
!endif
 
  SetOutPath $INSTDIR
 
 
; package all files, recursively, preserving attributes
; assume files are in the correct places
File /a /r "${srcdir}\bin"
 
  WriteUninstaller "${uninstaller}"
 
SectionEnd
 
; create shortcuts
Section
 
  CreateDirectory "${startmenu}"
  SetOutPath $INSTDIR\bin ; for working directory
  CreateShortCut "${startmenu}\${prodname}.lnk" "$INSTDIR\bin\${exec}" "" "$INSTDIR\bin\${icon}"

 
!ifdef website
WriteINIStr "${startmenu}\web site.url" "InternetShortcut" "URL" ${website}
 ; CreateShortCut "${startmenu}\Web Site.lnk "${website}" "URL"
!endif
 
SectionEnd
 
; Uninstaller
; All section names prefixed by "Un" will be in the uninstaller
 
UninstallText "This will uninstall ${prodname}."
 
!ifdef icon
UninstallIcon "bin\${icon}"
!endif
 
Section "Uninstall"
 
  DeleteRegKey HKLM "${uninstkey}"
  DeleteRegKey HKLM "${regkey}"
 
  Delete "${startmenu}\*.*"
  Delete "${startmenu}"
 
  RMDir /r $INSTDIR
 
SectionEnd