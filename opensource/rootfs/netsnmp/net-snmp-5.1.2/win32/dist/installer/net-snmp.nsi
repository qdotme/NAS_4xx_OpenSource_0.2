; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Net-SNMP"
!define PRODUCT_MAJ_VERSION "5"
!define PRODUCT_MIN_VERSION "1"
!define PRODUCT_REVISION "2"
!define PRODUCT_EXE_VERSION "pre1"
!define PRODUCT_WEB_SITE "http://www.net-snmp.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\encode_keychange.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "Net-SNMP:StartMenuDir"

; For environment variables
!define ALL_USERS
!include "SetEnVar.nsi"
!include "Add2Path.nsi"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
!define MUI_HEADERIMAGE_BITMAP "net-snmp-header1.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "net-snmp-header1.bmp"
; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "docs\COPYING"
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Net-SNMP"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README.txt"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_MAJ_VERSION}.${PRODUCT_MIN_VERSION}.${PRODUCT_REVISION}"
OutFile "..\\net-snmp-${PRODUCT_MAJ_VERSION}.\
                  ${PRODUCT_MIN_VERSION}.\
                  ${PRODUCT_REVISION}\
                  -${PRODUCT_EXE_VERSION}\
                  .win32.exe"
InstallDir "C:\usr"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "Base Components" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "README.txt"
  SetOutPath "$INSTDIR\bin"
  File "bin\netsnmp.dll"
  File "bin\encode_keychange.exe"
  File "bin\snmpvacm.exe"
  File "bin\snmpusm.exe"
  File "bin\snmptrap.exe"
  File "bin\snmptranslate.exe"
  File "bin\snmptest.exe"
  File "bin\snmptable.exe"
  File "bin\snmpstatus.exe"
  File "bin\snmpset.exe"
  File "bin\snmpnetstat.exe"
  File "bin\snmpgetnext.exe"
  File "bin\snmpget.exe"
  File "bin\snmpdf.exe"
  File "bin\snmpdelta.exe"
  File "bin\snmpbulkwalk.exe"
  File "bin\snmpbulkget.exe"
  File "bin\snmpwalk.exe"
  File "bin\mib2c"
  File "bin\mib2c.bat"
  Call CreateMib2cBat
  
  File "bin\snmpconf"
  File "bin\snmpconf.bat"
  Call CreateSnmpconfBat
  
  File "bin\traptoemail"
  File "bin\traptoemail.bat"
  Call CreatTraptoemailBat
  
  SetOutPath "$INSTDIR\share\snmp\mibs"
  File "share\snmp\mibs\AGENTX-MIB.txt"
  File "share\snmp\mibs\DISMAN-EVENT-MIB.txt"
  File "share\snmp\mibs\DISMAN-SCHEDULE-MIB.txt"
  File "share\snmp\mibs\DISMAN-SCRIPT-MIB.txt"
  File "share\snmp\mibs\EtherLike-MIB.txt"
  File "share\snmp\mibs\HCNUM-TC.txt"
  File "share\snmp\mibs\HOST-RESOURCES-MIB.txt"
  File "share\snmp\mibs\HOST-RESOURCES-TYPES.txt"
  File "share\snmp\mibs\IANA-ADDRESS-FAMILY-NUMBERS-MIB.txt"
  File "share\snmp\mibs\IANAifType-MIB.txt"
  File "share\snmp\mibs\IANA-LANGUAGE-MIB.txt"
  File "share\snmp\mibs\IF-INVERTED-STACK-MIB.txt"
  File "share\snmp\mibs\IF-MIB.txt"
  File "share\snmp\mibs\INET-ADDRESS-MIB.txt"
  File "share\snmp\mibs\IP-FORWARD-MIB.txt"
  File "share\snmp\mibs\IP-MIB.txt"
  File "share\snmp\mibs\IPV6-ICMP-MIB.txt"
  File "share\snmp\mibs\IPV6-MIB.txt"
  File "share\snmp\mibs\IPV6-TC.txt"
  File "share\snmp\mibs\IPV6-TCP-MIB.txt"
  File "share\snmp\mibs\IPV6-UDP-MIB.txt"
  File "share\snmp\mibs\LM-SENSORS-MIB.txt"
  File "share\snmp\mibs\MTA-MIB.txt"
  File "share\snmp\mibs\NET-SNMP-AGENT-MIB.txt"
  File "share\snmp\mibs\NET-SNMP-EXAMPLES-MIB.txt"
  File "share\snmp\mibs\NET-SNMP-MIB.txt"
  File "share\snmp\mibs\NET-SNMP-MONITOR-MIB.txt"
  File "share\snmp\mibs\NET-SNMP-SYSTEM-MIB.txt"
  File "share\snmp\mibs\NET-SNMP-TC.txt"
  File "share\snmp\mibs\NETWORK-SERVICES-MIB.txt"
  File "share\snmp\mibs\NOTIFICATION-LOG-MIB.txt"
  File "share\snmp\mibs\RFC1155-SMI.txt"
  File "share\snmp\mibs\RFC1213-MIB.txt"
  File "share\snmp\mibs\RFC-1215.txt"
  File "share\snmp\mibs\RMON-MIB.txt"
  File "share\snmp\mibs\SMUX-MIB.txt"
  File "share\snmp\mibs\SNMP-COMMUNITY-MIB.txt"
  File "share\snmp\mibs\SNMP-FRAMEWORK-MIB.txt"
  File "share\snmp\mibs\SNMP-MPD-MIB.txt"
  File "share\snmp\mibs\SNMP-NOTIFICATION-MIB.txt"
  File "share\snmp\mibs\SNMP-PROXY-MIB.txt"
  File "share\snmp\mibs\SNMP-TARGET-MIB.txt"
  File "share\snmp\mibs\SNMP-USER-BASED-SM-MIB.txt"
  File "share\snmp\mibs\SNMPv2-CONF.txt"
  File "share\snmp\mibs\SNMPv2-MIB.txt"
  File "share\snmp\mibs\SNMPv2-SMI.txt"
  File "share\snmp\mibs\SNMPv2-TC.txt"
  File "share\snmp\mibs\SNMPv2-TM.txt"
  File "share\snmp\mibs\SNMP-VIEW-BASED-ACM-MIB.txt"
  File "share\snmp\mibs\TCP-MIB.txt"
  File "share\snmp\mibs\TUNNEL-MIB.txt"
  File "share\snmp\mibs\UCD-DEMO-MIB.txt"
  File "share\snmp\mibs\UCD-DISKIO-MIB.txt"
  File "share\snmp\mibs\UCD-DLMOD-MIB.txt"
  File "share\snmp\mibs\UCD-IPFILTER-MIB.txt"
  File "share\snmp\mibs\UCD-IPFWACC-MIB.txt"
  File "share\snmp\mibs\UCD-SNMP-MIB.txt"
  File "share\snmp\mibs\UCD-SNMP-MIB-OLD.txt"
  File "share\snmp\mibs\UDP-MIB.txt"
  SetOutPath "$INSTDIR\docs"
  File "docs\COPYING"
  File "docs\Net-SNMP.chm"
  SetOutPath "$INSTDIR\share\snmp\snmpconf-data\snmp-data"
  File "share\snmp\snmpconf-data\snmp-data\authopts"
  File "share\snmp\snmpconf-data\snmp-data\debugging"
  File "share\snmp\snmpconf-data\snmp-data\mibs"
  File "share\snmp\snmpconf-data\snmp-data\output"
  File "share\snmp\snmpconf-data\snmp-data\snmpconf-config"
  SetOutPath "$INSTDIR\etc\snmp"
  File "etc\snmp\snmp.conf"
  SetOutPath "$INSTDIR\include\net-snmp"
  File "include\net-snmp\net-snmp-config.h"
  SetOutPath "$INSTDIR\include\net-snmp\agent"
  File "include\net-snmp\agent\mib_module_config.h"
  CreateDirectory "$INSTDIR\temp"
  CreateDirectory "$INSTDIR\snmp"
  CreateDirectory "$INSTDIR\snmp\persist"
  
  ; Add bin directory to the search path
  Push "$INSTDIR\bin"
  Call AddToPath
  
  Call CreateSnmpConf
SectionEnd

Section "Net-SNMP Agent Service" SEC02
  SetOutPath "$INSTDIR\bin"
  File "bin\snmpd.exe"
  SetOutPath "$INSTDIR\share\snmp\snmpconf-data\snmpd-data"
  File "share\snmp\snmpconf-data\snmpd-data\acl"
  File "share\snmp\snmpconf-data\snmpd-data\basic_setup"
  File "share\snmp\snmpconf-data\snmpd-data\extending"
  File "share\snmp\snmpconf-data\snmpd-data\monitor"
  File "share\snmp\snmpconf-data\snmpd-data\operation"
  File "share\snmp\snmpconf-data\snmpd-data\snmpconf-config"
  File "share\snmp\snmpconf-data\snmpd-data\system"
  File "share\snmp\snmpconf-data\snmpd-data\trapsinks"
  
  ; If we are on an NT system then install the service batch files.
  Call IsNT
  Pop $1
  StrCmp $1 0 NoService
  
  SetOutPath "$INSTDIR\"
  File "registeragent.bat"
  File "unregisteragent.bat"
  Call CreateAgentBats

  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP\Service"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Service\Register Agent Service.lnk" "$INSTDIR\registeragent.bat"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Service\Unregister Agent Service.lnk" "$INSTDIR\unregisteragent.bat"
  
  NoService:
SectionEnd

Section "Net-SNMP Trap Service" SEC03
  SetOutPath "$INSTDIR\bin"
  File "bin\snmptrapd.exe"
  SetOutPath "$INSTDIR\share\snmp\snmpconf-data\snmptrapd-data"
  File "share\snmp\snmpconf-data\snmptrapd-data\formatting"
  File "share\snmp\snmpconf-data\snmptrapd-data\snmpconf-config"
  File "share\snmp\snmpconf-data\snmptrapd-data\traphandle"
SectionEnd

Section "Perl SNMP Modules" SEC04
  SetOutPath "$INSTDIR\perl\x86"
  File "perl\x86\Net-SNMP.tar.gz"
  SetOutPath "$INSTDIR\perl"
  File "perl\Net-SNMP.ppd"
  SetOutPath "$INSTDIR\bin"
  File "bin\net-snmp-perl-test.pl"
SectionEnd

Section -AdditionalIcons
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Net-SNMP Help.lnk" "$INSTDIR\docs\Net-SNMP.chm"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\README.lnk" "$INSTDIR\README.txt"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bin\encode_keychange.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bin\encode_keychange.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_MAJ_VERSION}.${PRODUCT_MIN_VERSION}.${PRODUCT_REVISION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "${PRODUCT_STARTMENU_REGVAL}" "$ICONS_GROUP"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} \
               "The Base Components provide basic means for interrogating SNMP devices. These \
               include the command-line client applications, a short list of Management \
               Information Base MIB files, and a user-friendly Help subsystem"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} \
               "The Net-SNMP Agent Service provides information to a remote management system."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC03} \
               "The Net-SNMP Trap Service receives SNMP notifications traps and informs) \
               from other SNMP-enabled devices."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC04} \
               "The Perl SNMP Modules can be used if this computer will be used to \
               run or develop Perl-based SNMP programs (e.g. 'mib2c')"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Function CreateSnmpConf
  SetOutPath "$INSTDIR\etc\snmp"
  
  ; Slash it
  Push $INSTDIR
  Push "\"
  Push "/"
  Call StrRep
  Pop $R0 
  
  ClearErrors
  FileOpen $0 "snmp.conf" "w"
  IfErrors cleanup
  FileWrite $0 "mibdirs $R0/share/snmp/mibs$\r$\n"
  FileWrite $0 "persistentDir $R0/snmp/persist$\r$\n"
  FileWrite $0 "tempFilePattern $R0/temp/snmpdXXXXXX$\r$\n"
  
  cleanup:
  FileClose $0
  
  ; For environment variables
  ;!define ALL_USERS
  
  ; Set the conf path
  Push "SNMPCONFPATH"
  Push "$R0/etc/snmp"
  Call WriteEnvStr
  
  Push "SNMPSHAREPATH"
  Push "$R0/share/snmp"
  Call WriteEnvStr
FunctionEnd

Function CreateAgentBats
  SetOutPath "$INSTDIR\"
  ClearErrors
  FileOpen $0 "registeragent.bat" "w"
  IfErrors cleanup
  FileWrite $0 "$\"$INSTDIR\bin\snmpd.exe$\" -register$\r$\n"
  FileWrite $0 "pause"
  
  ClearErrors
  FileOpen $1 "unregisteragent.bat" "w"
  IfErrors cleanup
  FileWrite $1 "$\"$INSTDIR\bin\snmpd.exe$\" -unregister$\r$\n"
  FileWrite $1 "pause"

  cleanup:
  FileClose $0
  FileClose $1
FunctionEnd

; The trap receiver has not been tested
; completely as a Windows service.
Function CreateTrapdBats
  SetOutPath "$INSTDIR\"
  ClearErrors
  FileOpen $0 "registertrapd.bat" "w"
  IfErrors cleanup
  FileWrite $0 "$\"$INSTDIR\bin\snmptrapd.exe$\" -register$\r$\n"
  FileWrite $0 "pause"

  ClearErrors
  FileOpen $1 "unregistertrapd.bat" "w"
  IfErrors cleanup
  FileWrite $1 "$\"$INSTDIR\bin\snmptrapd.exe$\" -unregister$\r$\n"
  FileWrite $1 "pause"

  cleanup:
  FileClose $0
  FileClose $1
FunctionEnd

Function CreateSnmpconfBat
  SetOutPath "$INSTDIR\bin\"
  ClearErrors
  FileOpen $0 "snmpconf.bat" "r"
  GetTempFileName $R0
  FileOpen $1 $R0 "w"
  snmpconfloop:
    FileRead $0 $2
    IfErrors done
    StrCmp $2 "set MYPERLPROGRAM=c:\usr\bin\snmpconf$\n" 0 +3
      FileWrite $1 "set MYPERLPROGRAM=$INSTDIR\bin\snmpconf$\n"
      Goto snmpconfloop
    StrCmp $2 "set MYPERLPROGRAM=c:\usr\bin\snmpconf$\r$\n" 0 +3
      FileWrite $1 "set MYPERLPROGRAM=$INSTDIR\bin\snmpconf$\r$\n"
      Goto snmpconfloop
    FileWrite $1 $2
    Goto snmpconfloop

  done:
    FileClose $0
    FileClose $1
    Delete "snmpconf.bat"
    CopyFiles /SILENT $R0 "snmpconf.bat"
    Delete $R0
FunctionEnd

Function CreateMib2cBat
  SetOutPath "$INSTDIR\bin\"
  ClearErrors
  FileOpen $0 "mib2c.bat" "r"
  GetTempFileName $R0
  FileOpen $1 $R0 "w"
  mib2cloop:
    FileRead $0 $2
    IfErrors done
    StrCmp $2 "set MYPERLPROGRAM=c:\usr\bin\mib2c$\n" 0 +3
      FileWrite $1 "set MYPERLPROGRAM=$INSTDIR\bin\mib2c$\n"
      Goto mib2cloop
    StrCmp $2 "set MYPERLPROGRAM=c:\usr\bin\mib2c$\r$\n" 0 +3
      FileWrite $1 "set MYPERLPROGRAM=$INSTDIR\bin\mib2c$\r$\n"
      Goto mib2cloop
    FileWrite $1 $2
    Goto mib2cloop

  done:
    FileClose $0
    FileClose $1
    Delete "mib2c.bat"
    CopyFiles /SILENT $R0 "mib2c.bat"
    Delete $R0
FunctionEnd

Function CreatTraptoemailBat
  SetOutPath "$INSTDIR\bin\"
  ClearErrors
  FileOpen $0 "traptoemail.bat" "r"
  GetTempFileName $R0
  FileOpen $1 $R0 "w"
  traptoemailloop:
    FileRead $0 $2
    IfErrors done
    StrCmp $2 "set MYPERLPROGRAM=c:\usr\bin\traptoemail$\n" 0 +3
      FileWrite $1 "set MYPERLPROGRAM=$INSTDIR\bin\traptoemail$\n"
      Goto traptoemailloop
    StrCmp $2 "set MYPERLPROGRAM=c:\usr\bin\traptoemail$\r$\n" 0 +3
      FileWrite $1 "set MYPERLPROGRAM=$INSTDIR\bin\traptoemail$\r$\n"
      Goto traptoemailloop
    FileWrite $1 $2
    Goto traptoemailloop

  done:
    FileClose $0
    FileClose $1
    Delete "traptoemail.bat"
    CopyFiles /SILENT $R0 "traptoemail.bat"
    Delete $R0
FunctionEnd

Function StrRep
  Exch $R4 ; $R4 = Replacement String
  Exch
  Exch $R3 ; $R3 = String to replace (needle)
  Exch 2
  Exch $R1 ; $R1 = String to do replacement in (haystack)
  Push $R2 ; Replaced haystack
  Push $R5 ; Len (needle)
  Push $R6 ; len (haystack)
  Push $R7 ; Scratch reg
  StrCpy $R2 ""
  StrLen $R5 $R3
  StrLen $R6 $R1
loop:
  StrCpy $R7 $R1 $R5
  StrCmp $R7 $R3 found
  StrCpy $R7 $R1 1 ; - optimization can be removed if U know len needle=1
  StrCpy $R2 "$R2$R7"
  StrCpy $R1 $R1 $R6 1
  StrCmp $R1 "" done loop
found:
  StrCpy $R2 "$R2$R4"
  StrCpy $R1 $R1 $R6 $R5
  StrCmp $R1 "" done loop
done:
  StrCpy $R3 $R2
  Pop $R7
  Pop $R6
  Pop $R5
  Pop $R2
  Pop $R1
  Pop $R4
  Exch $R3
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  ReadRegStr $ICONS_GROUP ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "${PRODUCT_STARTMENU_REGVAL}"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\README.txt"
  Delete "$INSTDIR\perl\Net-SNMP.ppd"
  Delete "$INSTDIR\perl\x86\Net-SNMP.tar.gz"
  Delete "$INSTDIR\include\net-snmp\net-snmp-config.h"
  Delete "$INSTDIR\include\net-snmp\agent\mib_module_config.h"
  Delete "$INSTDIR\docs\COPYING"
  Delete "$INSTDIR\docs\Net-SNMP.chm"
  Delete "$INSTDIR\bin\net-snmp-perl-test.pl"
  Delete "$INSTDIR\bin\snmptrapd.exe"
  Delete "$INSTDIR\bin\snmpd.exe"
  Delete "$INSTDIR\bin\snmpwalk.exe"
  Delete "$INSTDIR\bin\snmpbulkget.exe"
  Delete "$INSTDIR\bin\snmpbulkwalk.exe"
  Delete "$INSTDIR\bin\snmpconf.pl"
  Delete "$INSTDIR\bin\snmpdelta.exe"
  Delete "$INSTDIR\bin\snmpdf.exe"
  Delete "$INSTDIR\bin\snmpget.exe"
  Delete "$INSTDIR\bin\snmpgetnext.exe"
  Delete "$INSTDIR\bin\snmpnetstat.exe"
  Delete "$INSTDIR\bin\snmpset.exe"
  Delete "$INSTDIR\bin\snmpstatus.exe"
  Delete "$INSTDIR\bin\snmptable.exe"
  Delete "$INSTDIR\bin\snmptest.exe"
  Delete "$INSTDIR\bin\snmptranslate.exe"
  Delete "$INSTDIR\bin\snmptrap.exe"
  Delete "$INSTDIR\bin\snmpusm.exe"
  Delete "$INSTDIR\bin\snmpvacm.exe"
  Delete "$INSTDIR\bin\encode_keychange.exe"
  Delete "$INSTDIR\bin\netsnmp.dll"
  Delete "$INSTDIR\bin\mib2c"
  Delete "$INSTDIR\bin\mib2c.bat"
  Delete "$INSTDIR\bin\snmpconf"
  Delete "$INSTDIR\bin\snmpconf.bat"
  Delete "$INSTDIR\bin\traptoemail"
  Delete "$INSTDIR\bin\traptoemail.bat"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmp-data\authopts"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmp-data\debugging"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmp-data\mibs"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmp-data\output"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmp-data\snmpconf-config"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmpd-data\acl"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmpd-data\basic_setup"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmpd-data\extending"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmpd-data\monitor"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmpd-data\operation"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmpd-data\snmpconf-config"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmpd-data\system"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmpd-data\trapsinks"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmptrapd-data\formatting"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmptrapd-data\snmpconf-config"
  Delete "$INSTDIR\share\snmp\snmpconf-data\snmptrapd-data\traphandle"
  Delete "$INSTDIR\share\snmp\snmp.conf"
  Delete "$INSTDIR\share\snmp\mibs\AGENTX-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\DISMAN-EVENT-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\DISMAN-SCHEDULE-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\DISMAN-SCRIPT-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\EtherLike-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\HCNUM-TC.txt"
  Delete "$INSTDIR\share\snmp\mibs\HOST-RESOURCES-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\HOST-RESOURCES-TYPES.txt"
  Delete "$INSTDIR\share\snmp\mibs\IANA-ADDRESS-FAMILY-NUMBERS-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\IANAifType-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\IANA-LANGUAGE-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\IF-INVERTED-STACK-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\IF-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\INET-ADDRESS-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\IP-FORWARD-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\IP-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\IPV6-ICMP-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\IPV6-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\IPV6-TC.txt"
  Delete "$INSTDIR\share\snmp\mibs\IPV6-TCP-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\IPV6-UDP-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\LM-SENSORS-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\MTA-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\NET-SNMP-AGENT-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\NET-SNMP-EXAMPLES-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\NET-SNMP-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\NET-SNMP-MONITOR-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\NET-SNMP-SYSTEM-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\NET-SNMP-TC.txt"
  Delete "$INSTDIR\share\snmp\mibs\NETWORK-SERVICES-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\NOTIFICATION-LOG-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\RFC1155-SMI.txt"
  Delete "$INSTDIR\share\snmp\mibs\RFC1213-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\RFC-1215.txt"
  Delete "$INSTDIR\share\snmp\mibs\RMON-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\SMUX-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMP-COMMUNITY-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMP-FRAMEWORK-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMP-MPD-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMP-NOTIFICATION-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMP-PROXY-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMP-TARGET-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMP-USER-BASED-SM-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMPv2-CONF.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMPv2-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMPv2-SMI.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMPv2-TC.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMPv2-TM.txt"
  Delete "$INSTDIR\share\snmp\mibs\SNMP-VIEW-BASED-ACM-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\TCP-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\TUNNEL-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\UCD-DEMO-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\UCD-DISKIO-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\UCD-DLMOD-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\UCD-IPFILTER-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\UCD-IPFWACC-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\UCD-SNMP-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\UCD-SNMP-MIB-OLD.txt"
  Delete "$INSTDIR\share\snmp\mibs\UDP-MIB.txt"
  Delete "$INSTDIR\share\snmp\mibs\.index"
  Delete "$INSTDIR\share\snmp\snmpd.conf"
  Delete "$INSTDIR\etc\snmp\snmp.conf"
  Delete "$INSTDIR\etc\snmp\snmpd.conf"
  Delete "$INSTDIR\etc\snmp\snmptrapd.conf"
  Delete "$INSTDIR\registeragent.bat"
  Delete "$INSTDIR\unregisteragent.bat"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Net-SNMP Help.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\README.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Service\Register Agent Service.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Service\Unregister Agent Service.lnk"

  RMDir "$SMPROGRAMS\$ICONS_GROUP\Service"
  RMDir "$SMPROGRAMS\$ICONS_GROUP"
  RMDir "$INSTDIR\perl\x86"
  RMDir "$INSTDIR\perl"
  RMDir "$INSTDIR\lib"
  RMDir "$INSTDIR\include\ucd-snmp"
  RMDir "$INSTDIR\include"
  RMDir "$INSTDIR\docs"
  RMDir "$INSTDIR\bin"
  RMDir "$INSTDIR\share\snmp\snmpconf-data\snmptrapd-data"
  RMDir "$INSTDIR\share\snmp\snmpconf-data\snmpd-data"
  RMDir "$INSTDIR\share\snmp\snmpconf-data\snmp-data"
  RMDir "$INSTDIR\share\snmp\snmpconf-data"

  RMDir "$INSTDIR\temp"
  RMDir "$INSTDIR\share\snmp\mibs"
  RMDir "$INSTDIR\snmp\persist"
  RMDir "$INSTDIR\snmp"
  RMDir "$INSTDIR\share\snmp"
  RMDir "$INSTDIR\share"
  RMDir "$INSTDIR\etc\snmp"
  RMDir "$INSTDIR\etc"
  RMDir "$INSTDIR\include\net-snmp\agent"
  RMDir "$INSTDIR\include\net-snmp"
  RMDir "$INSTDIR\include"
  RMDir "$INSTDIR"
  ; Delete the environment variables
  Push "SNMPCONFPATH"
  Call un.DeleteEnvStr
  
  Push "SNMPSHAREPATH"
  Call un.DeleteEnvStr

  Push "$INSTDIR\bin"
  Call un.RemoveFromPath
  
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
