;7Z�򿪿հ�
!system '>blank set/p=MSCF<nul'
!packhdr temp.dat 'cmd /c Copy /b temp.dat /b +blank&&del blank'

#========================================================================================================================
#
#         PS: ��ǰ�ű�û������Uninstall,��װ��Ҫ���������ɵ�Unistall.exe      ������ WriteUninstaller "$INSTDIR\Uninstall.exe" ���ж��Ƿ�����
#
#========================================================================================================================

Var MSG     ;MSG�������붨�壬��������ǰ�棬����WndProc::onCallback���������������Ҫ�����Ϣ����,���ڼ�¼��Ϣ��Ϣ
Var Dialog  ;Dialog����Ҳ��Ҫ���壬��������NSISĬ�ϵĶԻ���������ڱ��洰���пؼ�����Ϣ
Var DialogWidth
Var DialogHight

Var IconImage ;ͼ��ͼƬ
Var BGImage  ;������ͼ
Var ImageHandle
Var IconHandle
Var DefaultPage

Var WarningForm

Var Btn_QuickInstall
Var Btn_CustomInstall
Var Btn_Install
Var Btn_Done
Var Btn_Confirm
Var Btn_Back

Var CurrentPage ;��ǰҳ���˳�ҳ�ļ��ҳ��

Var Txt_License
Var Bool_License ;��װЭ��״̬
Var Bool_LicenseShow
Var Chk_License
Var Btn_Agreement
Var Lbl_License

Var Lbl_Sumary
Var PPercent
Var PB_ProgressBar
Var SubProgress

Var Bool_UnInstall

Var Btn_No
Var Btn_Yes

Var Lbl_AppName
Var Lbl_Title
Var Lbl_Version

Var Txt_Browser
Var Btn_TxtBrowser
Var btn_Browser
var Txt_SpaceValue

Var Bool_Desktop ;��װ������
Var Bool_QuickRun  ;��ӿ�������
Var Chk_Desktop
Var Chk_QuickRun


;---------------------------ȫ�ֱ���ű�Ԥ����ĳ���-----------------------------------------------------
; -- ��װ�ļ�����Ŀ¼ Ϊ ��PROGRAM_NAME + PRODUCT_VERSION�� ����Ϊ  101����PPT0.0.0.27
!define PRODUCT_NAME "101����PPT"
!define PRODUCT_DIR "101����PPT"
!define PROGRAM_NAME "101PPT"
!define MUST_PACKAGE_FOLDER_NAME "ImportantFile"

!define CACHE_DIR "Cache"
!define TEMP_DIR "Temp"

!define BIN_DIR "bin"
!define FLASH_OCX "flash.ocx"
!define KAITI "kaiti��.ttf"

!define PPTSHELL_EXE_NAME "${PROGRAM_NAME}.exe"
!define OLD_PPTSHELL_EXE_NAME "PPTShell.exe"
!define PPT_EXE_NAME "POWERPNT.exe"
!define UPDATE_EXE_NAME "update.exe"
!define COURSE_EXE_NAME "CoursePlayer.exe"
!define NODE_EXE_NAME "node.exe"

;!define MyMutex_Install     "PPTShell_Install"
;!define MyMutex_UnInstall   "PPTShell_UnInstall"


;!define ADBMGR_NAME "���ֹ���"
;!define ADBMGR_DIR "AssistantMgr"
;!define ADBMGR_PROGRAM_NAME "AssistantManager"

!define PRODUCT_VERSION "1.1.3.8"
!define PRODUCT_PUBLISHER "��������Ƽ����޹�˾"
!define PRODUCT_WEB_SITE "http://ppt.101.com"

!define PRODUCT_WEB_LINK_NAME "101����PPT����"

!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${PROGRAM_NAME}.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

ShowInstDetails nevershow ;�����Ƿ���ʾ��װ��ϸ��Ϣ��
ShowUnInstDetails nevershow ;�����Ƿ���ʾɾ����ϸ��Ϣ��


;Ӧ�ó�����ʾ����
Name "${PRODUCT_NAME}"
;Ӧ�ó�������ļ���
OutFile "${PRODUCT_NAME}${PRODUCT_VERSION}.exe"
;Ĭ�ϰ�װĿ¼
InstallDir "$PROGRAMFILES\${PRODUCT_DIR}"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"

; MUI Ԥ���峣��
;!define MUI_ABORTWARNING ;�˳���ʾ
;��װͼ���·������
!define MUI_ICON "Icon\Install.ico"
;ж��ͼ���·������
!define MUI_UNICON "Icon\UnInstall.ico"
;ʹ�õ�UI
!define MUI_UI "UI\mod.exe" ;��UI���޸Ĺ���

;---------------------------�������ѹ�����ͣ�Ҳ����ͨ���������ű����ƣ�------------------------------------
RequestExecutionLevel		admin

; ����ѹ��ѡ��
SetCompress auto
; ѡ��ѹ����ʽ
SetCompressor lzma
;�������ݿ��Ż�
SetDatablockOptimize on
;XPStyle on
; ------ MUI �ִ����涨�� (1.67 �汾���ϼ���) ------
!include "MUI2.nsh"
!include "WinCore.nsh"
!include "nsWindows.nsh"
!include "LogicLib.nsh"
!include "FileFunc.nsh"
!include "nsDialogs.nsh"
!include "LoadRTF.nsh"
!include "WinVer.nsh"
!include "VersionCheckNew.nsh"
!include "WordFunc.nsh"

;!include "WndSubclass.nsh"  ;���ȿ���ʹ��

!define /math PBM_SETRANGE32 ${WM_USER} + 6 #����nsisδ�����Ľ�������س���
;!define WS_BORDER 0x800000

!define MUI_CUSTOMFUNCTION_GUIINIT onGUIInit

;�Զ���ҳ��
Page custom Page.1

Page custom Page.2

; ���Э��ҳ��
;!define MUI_LICENSEPAGE_CHECKBOX
; ��װĿ¼ѡ��ҳ��
;!insertmacro MUI_PAGE_DIRECTORY
; ��װ����ҳ��
;!define MUI_PAGE_CUSTOMFUNCTION_PRO InstFilesPagePRO
!define MUI_PAGE_CUSTOMFUNCTION_SHOW InstFilesPageShow
!define MUI_PAGE_CUSTOMFUNCTION_LEAVE InstFileLeave
!insertmacro MUI_PAGE_INSTFILES

Page custom Page.3

Page custom Page.4

Page custom Page.Exit
; ��װ���ҳ��
;!insertmacro MUI_PAGE_FINISH
; ��װж�ع���ҳ��
UninstPage custom un.Page.5

UninstPage instfiles un.InstFiles.PRO un.InstFiles.Show un.InstFiles.Leave

UninstPage custom un.Page.6

UninstPage custom un.Page.7


; ��װ�����������������
!insertmacro MUI_LANGUAGE "SimpChinese"

;------------------------------------------------------MUI �ִ����涨���Լ���������------------------------

Function .onInit
  SetOverwrite on
  InitPluginsDir
  ;Call MutexCreate
  ;��ֹ�����װ����ʵ��
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "PPTShell_Install") i .r1 ?e'
  Pop $R0
  StrCmp $R0 0 +3
  MessageBox MB_OK|MB_ICONEXCLAMATION "��װ����ж�س�����������"
  Abort
  
  ;InstallDir "$PROGRAMFILES\${PRODUCT_DIR}"
  
  ;Call UnInstallDirver  ;��Ҫ��ʱ�����ж������  zcs
  FindProcDLL::FindProc  "${PPTSHELL_EXE_NAME}" ;"${PROGRAM_NAME}.exe"
  Pop $R0
  IntCmp $R0 1 kill_app no_run
  kill_app:
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 "${PRODUCT_NAME}�������У��㡰�ǡ����رռ�����װ���㡰���˳���װ����" IDYES kill IDNO quit
  kill:
    ;Call KillPPT
    KillProcDLL::KillProc "${PPT_EXE_NAME}"
    KillProcDLL::KillProc "${PPTSHELL_EXE_NAME}"
    KillProcDLL::KillProc "${PPTSHELL_EXE_NAME}"
    Sleep 500
    goto install
  quit:
    Quit
  no_run:
    ;Call UninstallSoft ;����ж���ж�
  install:
    ;Call KillUpdate
    ;Call KillCoursePlayer
    KillProcDll::KillProc "${OLD_PPTSHELL_EXE_NAME}"
    KillProcDLL::KillProc "${UPDATE_EXE_NAME}"
    KillProcDLL::KillProc "${COURSE_EXE_NAME}"
    KillProcDLL::KillProc "${NODE_EXE_NAME}"
    Call CheckOldProc
  
  ;Kill Adb

  ;FindProcDLL::FindProc "adb.exe"
  ;Pop $R0
  ;IntCmp $R0 1 kill_adb adb_no_run
  ;kill_adb:
  ;  KillProcDLL::KillProc "adb.exe"
  ;  Sleep 100
  ;  goto install
  ;adb_no_run:

    StrCpy $Bool_License 1
    StrCpy $Bool_Desktop 1
    StrCpy $Bool_QuickRun 1
    
    StrCpy $DialogWidth 602 ;588
    StrCpy $DialogHight 428 ;291
    
    #===================================================================
    #           un.onInit Ҳ�ǵ�Ҫ�ͷ�ͼƬ
    #===================================================================
    
    File `/oname=$PLUGINSDIR\license.rtf` `license\license.rtf`
    
    File `/ONAME=$PLUGINSDIR\bg.bmp` `images\StPad.bmp`
    File `/ONAME=$PLUGINSDIR\icon.bmp` `images\icon.bmp`
    File `/ONAME=$PLUGINSDIR\IconP.bmp` `images\IconP.bmp`
    
    File `/ONAME=$PLUGINSDIR\QuickInstall.bmp` `images\QuickInstall.bmp`
    File `/ONAME=$PLUGINSDIR\CustomInstall.bmp` `images\CustomInstall.bmp`
    File `/ONAME=$PLUGINSDIR\License_Checked.bmp` `images\License_Checked.bmp`
    File `/ONAME=$PLUGINSDIR\License_NoChecked.bmp` `images\License_NoChecked.bmp`
    File `/ONAME=$PLUGINSDIR\Agreement.bmp` `images\Agreement.bmp`
    File `/ONAME=$PLUGINSDIR\Confirm.bmp` `images\Confirm.bmp`
    File `/ONAME=$PLUGINSDIR\Back.bmp` `images\Back.bmp`
    
    File `/ONAME=$PLUGINSDIR\Install.bmp` `images\Install.bmp`
    File `/ONAME=$PLUGINSDIR\InstallDir.bmp` `images\InstallDir.bmp`
    File `/ONAME=$PLUGINSDIR\Browse.bmp` `images\Browse.bmp`
    
    File `/ONAME=$PLUGINSDIR\Yes.bmp` `images\Yes.bmp`
    File `/ONAME=$PLUGINSDIR\No.bmp` `images\No.bmp`
    File `/ONAME=$PLUGINSDIR\Done.bmp` `images\Done.bmp`

    File `/oname=$PLUGINSDIR\btnClose.bmp` `images\close.bmp`
    File `/oname=$PLUGINSDIR\btnmini.bmp` `images\mini.bmp`
    
    File `/oname=$PLUGINSDIR\VersionBgLeft.bmp` `images\VersionBgLeft.bmp`
    File `/oname=$PLUGINSDIR\VersionBgRight.bmp` `images\VersionBgRight.bmp`
    ;��װҳ��
    File `/oname=$PLUGINSDIR\Intro.bmp` `images\Install_Intro.bmp`
    File `/oname=$PLUGINSDIR\IconDone.bmp` `images\IconDone.bmp`
    ;ж��ҳ��
    File `/oname=$PLUGINSDIR\IconUnInstall.bmp` `images\IconUnInstall.bmp`
    
    ;������Ƥ��
    File `/oname=$PLUGINSDIR\Progress.bmp` `images\Progress.bmp`
    File `/oname=$PLUGINSDIR\ProgressBar.bmp` `images\ProgressBar.bmp`
    ;File `/ONAME=$PLUGINSDIR\PPercent.bmp` `images\PPercent.bmp`
    
    ;�Զ��尲װ CheckBox
    File `/ONAME=$PLUGINSDIR\Desktop_Checked.bmp` `images\Desktop_Checked.bmp`
    File `/ONAME=$PLUGINSDIR\Desktop_NoChecked.bmp` `images\Desktop_NoChecked.bmp`
    
    File `/ONAME=$PLUGINSDIR\QuickRun_Checked.bmp` `images\QuickRun_Checked.bmp`
    File `/ONAME=$PLUGINSDIR\QuickRun_NoChecked.bmp` `images\QuickRun_NoChecked.bmp`
    
    SkinBtn::Init "$PLUGINSDIR\QuickInstall.bmp"
    SkinBtn::Init "$PLUGINSDIR\close.bmp"
FunctionEnd

Function KillPPT
  FindProcDLL::FindProc "${PPT_NAME}.EXE"
  Pop $R0
  IntCmp $R0 1 kill_ppt ppt_no_run
  kill_ppt:
    KillProcDLL::KillProc "${PPT_NAME}.EXE"
    Sleep 500
  ppt_no_run:
FunctionEnd

Function KillUpdate
  FindProcDLL::FindProc "${UPDATE_NAME}.EXE"
  Pop $R0
  IntCmp $R0 1 kill_update update_no_run
  kill_update:
    KillProcDLL::KillProc "${UPDATE_NAME}.EXE"
    Sleep 500
  update_no_run:
FunctionEnd

Function KillCoursePlayer
  FindProcDLL::FindProc "${COURSE_NAME}.EXE"
  Pop $R0
  IntCmp $R0 1 kill_course course_no_run
  kill_course:
    KillProcDLL::KillProc "${COURSE_NAME}.EXE"
    Sleep 500
  course_no_run:
FunctionEnd

Function KillAllApp
    KillProcDLL::KillProc "${PPT_EXE_NAME}"
    KillProcDLL::KillProc "${PPTSHELL_EXE_NAME}"
    KillProcDll::KillProc "${OLD_PPTSHELL_EXE_NAME}"
    KillProcDLL::KillProc "${UPDATE_EXE_NAME}"
    KillProcDLL::KillProc "${COURSE_EXE_NAME}"
    KillProcDLL::KillProc "${NODE_EXE_NAME}"
FunctionEnd

;-------------��������ϰ�װ����������----------
;Function MutexCreate
;  System::Call 'kernel32::CreateMutexA(i 0, i 0, t"${MyMutex_Install}") i.R3 ?e'  ;#��鰲װ���⣺#
;  Pop $R2
;  System::Call 'kernel32::CloseHandle(i R1) i.s'

;  IntCmp $R2 0 CheckUnMutex MutexMsg   ;#�жϰ�װ����Ĵ���#
;MutexMsg:
;  MessageBox MB_OK|MB_ICONEXCLAMATION "��װ�����Ѿ����У�"
;  Quit
;CheckUnMutex:
;  System::Call 'kernel32::CreateMutexA(i 0, i 0, t"${MyMutex_UnInstall}") i.R3 ?e'    ;#���ж�ػ��⣺#
;  Pop $R2
;  System::Call 'kernel32::CloseHandle(i R3) i.s'

;  IntCmp $R2 0 ExitCheckMutex UnMutexMsg      ;#�ж�ж�ػ���Ĵ���#
;UnMutexMsg:
;  MessageBox MB_OK|MB_ICONEXCLAMATION "ж�س����Ѿ����У�"
;  Quit
;ExitCheckMutex:
;FunctionEnd

;Section Install
;  SetOutPath $EXEDIR
;  WriteUninstaller "$EXEDIR\Uninstall.exe"
;SectionEnd

;-------------����������Ƿ��Ѱ�װ�˳��򼰰汾���----------

Function CheckOldProc

  ClearErrors

  ReadRegStr $0 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion"
  ReadRegStr $1 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName"
  ReadRegStr $2 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher"
  ReadRegStr $6 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString"
  ${GetParent} "$6" $R6

  IfErrors ExitCheckOldProc
  
  StrCmp $1 ${PRODUCT_NAME} 0 ExitCheckOldProc    ;�����1.0.1.1���汾�Ľ�����������

  ${VersionCheckNew} $0 ${PRODUCT_VERSION} "$R0"
  StrCmp $R0 0 0 ExistProcCmp ;��ǰ������ϰ�װ�汾�밲װ�̰汾һ��
  MessageBox MB_OKCANCEL|MB_DEFBUTTON2|MB_TOPMOST "���Ѱ�װ��$1�汾���뵱ǰ�汾һ�¡���ȷ��Ҫ���Ǵ˳�����" IDCANCEL DoCancel
  IfFileExists "$R6" SetInstDir ExitCheckOldProc ;��� $R1���λ�� �����ļ�����ת�� DIR: ɾ������������ļ�
SetInstDir:
   StrCpy $INSTDIR $R6
   Goto ExitCheckOldProc

ExistProcCmp:
  StrCmp $R0 1 0 ExitCheckOldProc ;��ǰ�����ϵİ汾�Ȱ�װ�̰汾��       ;ExistProcL
  ;MessageBox MB_OK "���Ѱ�װ��$1�汾�����ڵ�ǰ�汾${PRODUCT_VERSION}�����ܰ�װ�˳�����ж�غ��ٰ�װ��"
  ;Quit
  ;����ж�س���
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_TOPMOST "���Ѱ�װ��$1�汾�����ڵ�ǰ�汾${PRODUCT_VERSION}�����ܼ�����װ���Ƿ�ж�غ������װ��" IDNo DoCancel
  ;ReadRegStr $R0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString"
  ;${GetParent} "$6" $R6
  ;StrCmp $R0 "" ExitCheckOldProc
  ;IfFileExists $R0 UnInstallApp
	;Goto ExitCheckOldProc
;UnInstallApp:
  ;ExecWait "$R0 /S _?=$R1" ;����$R0Ϊ��ȡ����ж�س������ƣ� /S�Ǿ�Ĭж�ز���ʹ��NSIS���ɵ�ж�س������Ҫ���� _? ���ܵȴ�ж�ء�$R1�����λ��
  IfFileExists "$R6" DelFile ;��� $R1���λ�� �����ļ�����ת�� DIR: ɾ������������ļ�
  Goto ExitCheckOldProc
DelFile: ;����ļ��д���
	;Delete "$R1\*.*" ;��ɾ�����������ļ�,�����ʹ��
  StrCpy $INSTDIR $R6
  Delete "$INSTDIR\*.exe"
  Delete "$INSTDIR\*.dll"
  Delete "$INSTDIR\*.dat"
  Delete "$INSTDIR\*.log"

  RMDir /r /REBOOTOK "$INSTDIR\bin"
  RMDir /r /REBOOTOK "$INSTDIR\Skins"
  RMDir /r /REBOOTOK "$INSTDIR\template"
  RMDir /r /REBOOTOK "$INSTDIR\Cache"
  RMDir /r /REBOOTOK "$INSTDIR\log"
  RMDir /r /REBOOTOK "$INSTDIR\temp"
  
  Call InstallDelContent
  
  Goto ExitCheckOldProc
ExistProcL:
  MessageBox MB_OKCANCEL|MB_DEFBUTTON2|MB_TOPMOST "���Ѱ�װ�� $1 �ĽϵͰ汾 $0 ����ǰ��װ�汾Ϊ${PRODUCT_VERSION}����ȷ��Ҫ���°���򸲸Ǿɰ������" IDOK ExitCheckOldProc IDCANCEL DoCancel
DoCancel:
  Quit
ExitCheckOldProc:
FunctionEnd

Function InstallDelContent
  SetShellVarContext all
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}.lnk"
  Delete "$DESKTOP\${PRODUCT_WEB_LINK_NAME}.lnk"
  RMDir /r /REBOOTOK "$SMPROGRAMS\${PRODUCT_NAME}"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"

  ;�ָ�ע���
 #------------------ Win 8 -------------------------------------
  DeleteRegKey HKCR ".ppt\PPTShell"
  DeleteRegKey HKCR ".pptx\PPTShells"

  WriteRegStr HKCR ".ppt" "" "PowerPoint.Show.8"
  WriteRegStr HKCR ".ppt\PowerPoint.Show.8\ShellNew" "" ""

  WriteRegStr HKCR ".pptx" "" "PowerPoint.Show.12"
  WriteRegStr HKCR ".pptx\PowerPoint.Show.12\ShellNew" "" ""

 #------------------ ���� -------------------------------------

 DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.pptx\UserChoice"
 DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.ppt\UserChoice"

 DeleteRegKey HKCR "PPTShell\\Shell\\Open\\Command"
 DeleteRegKey HKCU "Software\\Classes\\Applications\\PPTShell\\shell\\open\\command"

 WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.pptx\UserChoice" "Progid" "PowerPoint.Show.12"
 WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.ppt\UserChoice" "Progid" "PowerPoint.Show.8"
FunctionEnd

Function onGUIInit

    ;�����߿�
    System::Call `user32::SetWindowLong(i$HWNDPARENT,i${GWL_STYLE},0x9480084C)i.R0`
    ;������Ӱ
    System::Call `user32::SetClassLong(i$HWNDPARENT,i${GCL_STYLE},i${CS_DROPSHADOW})`
    ;����һЩ���пؼ�
    GetDlgItem $0 $HWNDPARENT 1034
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1035
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1036
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1037
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1038
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1039
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1256
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1028
    ShowWindow $0 ${SW_HIDE}

    ${NSW_SetWindowSize} $HWNDPARENT $DialogWidth $DialogHight ;�ı��������С
    System::Call User32::GetDesktopWindow()i.R0
    ;Բ��
    System::Alloc 16
    System::Call user32::GetWindowRect(i$HWNDPARENT,isR0)
    System::Call *$R0(i.R1,i.R2,i.R3,i.R4)
    IntOp $R3 $R3 - $R1
    IntOp $R4 $R4 - $R2
    System::Call gdi32::CreateRoundRectRgn(i0,i0,iR3,iR4,i4,i4)i.r0
    System::Call user32::SetWindowRgn(i$HWNDPARENT,ir0,i1)
    System::Free $R0
    
    
FunctionEnd

Function .onInstFailed
  Call OnExit
FunctionEnd

;�����ޱ߿��ƶ�
Function onGUICallback
  ${If} $MSG = ${WM_LBUTTONDOWN}
        SendMessage $HWNDPARENT ${WM_NCLBUTTONDOWN} ${HTCAPTION} $0
  ${EndIf}
FunctionEnd

;�����Ի����ƶ�
Function onWarningGUICallback
  ${If} $MSG = ${WM_LBUTTONDOWN}
    SendMessage $WarningForm ${WM_NCLBUTTONDOWN} ${HTCAPTION} $0
  ${EndIf}
FunctionEnd

#=======================================================================#
#                             ��װ�� -- ��ҳ                            #
#=======================================================================#
Function Page.1

    GetDlgItem $0 $HWNDPARENT 1
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 2
    ShowWindow $0 ${SW_HIDE}
    EnableWindow $0 0       ;��ֹȡ����ť���ﵽ����ESC��Ч��
    GetDlgItem $0 $HWNDPARENT 3
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1990
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1991
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1992
    ShowWindow $0 ${SW_HIDE}
    
    StrCpy $Bool_LicenseShow 0
    ;���õ�ǰҳ��λ�����˳�ҳ��������
    StrCpy $CurrentPage 6
    
    nsDialogs::Create 1044
    Pop $0
    ${If} $0 == error
        Abort
    ${EndIf}
    SetCtlColors $0 ""  transparent ;�������͸��
    ${NSW_SetWindowSize} $0 601 427 ;�ı�Page��С   588 291

    ;Ӧ����     zcs
;    ;${NSD_CreateLabel} 270 70 300 60 "${PRODUCT_NAME}"        ;--ԭλ�� �����ͼ��ʱ
;    ${NSD_CreateLabel} 200 70 300 60 "${PRODUCT_NAME}"         ;--��ͼ�� ����
;    Pop $Lbl_AppName
;    SetCtlColors $Lbl_AppName "FFFFFF" transparent ;�������͸��
;    CreateFont $1 "΢���ź�" "28" "400"
;    SendMessage $Lbl_AppName ${WM_SETFONT} $1 0
;    ;;${NSD_AddStyle} $Lbl_AppName ${ES_CENTER}
;    GetFunctionAddress $0 onGUICallback
;    WndProc::onCallback $Lbl_AppName $0 ;�����ޱ߿����ƶ�

    ;��������
    ${NSD_CreateLabel} 5u 5u 115u 12u "${PRODUCT_NAME}" ;��װ��"
    Pop $Lbl_Title
    ;SetCtlColors $Lbl_Title "" 0xFFFFFF ;��ɫ
    SetCtlColors $Lbl_Title "FFFFFF"  transparent ;�������͸��
    CreateFont $1 "΢���ź�" "11" "800"
    SendMessage $Lbl_Title ${WM_SETFONT} $1 0
    ;${NSD_AddStyle} $Lbl_Title ${ES_CENTER}
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $Lbl_Title $0 ;�����ޱ߿����ƶ�

    ;����汾
    ${NSD_CreateLabel} 305 180 115u 12u "��ǰ�汾" ;  ${PRODUCT_VERSION}"
    Pop $Lbl_Version
    ;SetCtlColors $Lbl_Title "" 0xFFFFFF ;��ɫ
    SetCtlColors $Lbl_Version "FFFFFF"  transparent ;�������͸��
    CreateFont $1 "����" "10" "120"
    SendMessage $Lbl_Version ${WM_SETFONT} $1 0
    ;${NSD_AddStyle} $Lbl_Title ${ES_CENTER}
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $Lbl_Version $0 ;�����ޱ߿����ƶ�
    
    ;���ٰ�װ��ť
    ${NSD_CreateButton} 204 249 191 54 ""
    Pop $Btn_QuickInstall
    SkinBtn::Set /IMGID=$PLUGINSDIR\QuickInstall.bmp $Btn_QuickInstall
    GetFunctionAddress $3 OnQucikInstall
    SkinBtn::onClick $Btn_QuickInstall $3
    ${NSD_SetFocus} $Btn_QuickInstall

    ;�Զ��尲װ��ť
    ${NSD_CreateButton} 505 390 85 16 ""
    Pop $Btn_CustomInstall
    SkinBtn::Set /IMGID=$PLUGINSDIR\CustomInstall.bmp $Btn_CustomInstall
    GetFunctionAddress $3 OnNext
    SkinBtn::onClick $Btn_CustomInstall $3
    
    ;ͬ��Э��CheckBox
    ${NSD_CreateButton} 16 390 17 17 ""
    Pop $Chk_License
    SkinBtn::Set /IMGID=$PLUGINSDIR\License_Checked.bmp $Chk_License
    GetFunctionAddress $3 SetLicense
    SkinBtn::onClick $Chk_License $3
    
    Call InitLicense
    
    ;ͬ��ѧ��������
    ${NSD_CreateLabel} 42 388 205 20 "�����Ķ���ͬ��${PRODUCT_NAME}��"
    Pop $Lbl_License
    ;SetCtlColors $Lbl_Title "" 0xFFFFFF ;��ɫ
    SetCtlColors $Lbl_License "676666"  transparent ;�������͸��
    CreateFont $1 "΢���ź�" "11" "200"
    SendMessage $Lbl_License ${WM_SETFONT} $1 0
    ;${NSD_AddStyle} $Lbl_License ${ES_CENTER}
    
    ;��ȡRTF���ı���
		nsDialogs::CreateControl "RichEdit20A" \
    ${ES_READONLY}|${WS_VISIBLE}|${WS_CHILD}|${WS_TABSTOP}|${WS_VSCROLL}|${ES_MULTILINE}|${ES_WANTRETURN} \
		${WS_EX_STATICEDGE} 38 45 525 300 ''
    Pop $Txt_License
		${LoadRTF} '$PLUGINSDIR\license.rtf' $Txt_License
    ShowWindow $Txt_License ${SW_HIDE}
    
    ;�û�Э�鰴ť
    ${NSD_CreateButton} 247 390 85 16 ""
    Pop $Btn_Agreement
    SkinBtn::Set /IMGID=$PLUGINSDIR\Agreement.bmp $Btn_Agreement
    GetFunctionAddress $3 OnClickAgreement
    SkinBtn::onClick $Btn_Agreement $3

    ;Э��ȷ����ť
    ;${NSD_CreateButton} 470 375 122 47 ""
    ${NSD_CreateButton} 470 379 105 38 ""
    Pop $Btn_Confirm
    SkinBtn::Set /IMGID=$PLUGINSDIR\Confirm.bmp $Btn_Confirm
    GetFunctionAddress $3 OnClickAgreement
    SkinBtn::onClick $Btn_Confirm $3
    ShowWindow $Btn_Confirm ${SW_HIDE}

    ;��С����ť
    ${NSD_CreateButton} 546 1 27 27 ""
    Pop $0
    SkinBtn::Set /IMGID=$PLUGINSDIR\btnmini.bmp $0
    GetFunctionAddress $3 OnMini
    SkinBtn::onClick $0 $3

    ;�رհ�ť
    ${NSD_CreateButton} 573 1 27 27 ""
    Pop $0
    SkinBtn::Set /IMGID=$PLUGINSDIR\btnClose.bmp $0
    GetFunctionAddress $3 OnClickClose
    SkinBtn::onClick $0 $3
    
    ;���汾ͼ���󲿷�
    ${NSD_CreateBitmap} 364 176 9 22 ""
    Pop $IconImage
    ${NSD_SetImage} $IconImage $PLUGINSDIR\VersionBgLeft.bmp $IconHandle
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $IconImage $0 ;�����ޱ߿����ƶ�
    
    ;���浽��ͼ���Ҳ���
;    ${NSD_CreateBitmap} 373 176 10 22 ""
;    Pop $IconImage
;    ${NSD_SetImage} $IconImage $PLUGINSDIR\VersionBgRight.bmp $IconHandle
;    GetFunctionAddress $0 onGUICallback
;    WndProc::onCallback $IconImage $0 ;�����ޱ߿����ƶ�
    
    ${NSD_CreateButton} 365 176 70 22 "${PRODUCT_VERSION}"
    Pop $0
    SetCtlColors $0 "FFFFFF" transparent ;͸��
    CreateFont $1 "����" "10" "120"
    SendMessage $0 ${WM_SETFONT} $1 0
    ${NSD_AddStyle} $0 ${ES_LEFT}
    SkinBtn::Set /IMGID=$PLUGINSDIR\VersionBgRight.bmp $0
    GetFunctionAddress $3 onGUICallback
    WndProc::onCallback $0 $3 ;�����ޱ߿����ƶ�
    

    ;������ͼ��
    ${NSD_CreateBitmap} 255 80 101 116 ""
    Pop $IconImage
    ${NSD_SetImage} $IconImage $PLUGINSDIR\IconP.bmp $IconHandle
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $IconImage $0 ;�����ޱ߿����ƶ�

    ;��������ͼ
    ${NSD_CreateBitmap} 0 0 100% 100% ""
    Pop $BGImage
    ${NSD_SetImage} $BGImage $PLUGINSDIR\bg.bmp $ImageHandle
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $BGImage $0 ;�����ޱ߿����ƶ�
    nsDialogs::Show
    ${NSD_FreeImage} $IconHandle
    ${NSD_FreeImage} $ImageHandle
    
FunctionEnd

;CheckBox����¼�
Function SetLicense
         ${If} $Bool_License == 1
               Call License_NoChecked
               Strcpy $Bool_License 0
        ${Else}
               Call License_Checked
               Strcpy $Bool_License 1
        ${EndIf}
FunctionEnd

Function InitLicense
         ${If} $Bool_License == 1
               Call License_Checked
        ${Else}
               Call License_NoChecked
        ${EndIf}
FunctionEnd

;����ѡ��״̬ʱ�ı���ͼƬ
Function License_Checked
  SkinBtn::Set /IMGID=$PLUGINSDIR\License_Checked.bmp $Chk_License
  EnableWindow $Btn_CustomInstall 1
  EnableWindow $Btn_QuickInstall 1
FunctionEnd

;����δѡ��״̬ʱ�ı���ͼƬ
Function License_NoChecked
  SkinBtn::Set /IMGID=$PLUGINSDIR\License_NoChecked.bmp $Chk_License
  EnableWindow $Btn_CustomInstall 0
  EnableWindow $Btn_QuickInstall 0
FunctionEnd

;Э�鰴ť�¼�
Function OnClickAgreement
	${IF} $Bool_LicenseShow == 1
		ShowWindow $Btn_QuickInstall ${SW_SHOW}
		ShowWindow $Btn_CustomInstall ${SW_SHOW}
		ShowWindow $Btn_Agreement ${SW_SHOW}
		ShowWindow $Chk_License ${SW_SHOW}
		ShowWindow $Lbl_AppName ${SW_SHOW}
		ShowWindow $Lbl_License ${SW_SHOW}
    ShowWindow $Lbl_Version ${SW_SHOW}
		
    ShowWindow $Btn_Confirm ${SW_HIDE}
    ShowWindow $Txt_License ${SW_HIDE}
		IntOp $Bool_LicenseShow $Bool_LicenseShow - 1
	${ELSE}
	  ShowWindow $Btn_QuickInstall ${SW_HIDE}
	  ShowWindow $Btn_CustomInstall ${SW_HIDE}
		ShowWindow $Btn_Agreement ${SW_HIDE}
		ShowWindow $Chk_License ${SW_HIDE}
	  ShowWindow $Lbl_AppName ${SW_HIDE}
		ShowWindow $Lbl_License ${SW_HIDE}
		ShowWindow $Lbl_Version ${SW_HIDE}
		
		ShowWindow $Btn_Confirm ${SW_SHOW}
	  ShowWindow $Txt_License ${SW_SHOW}
		IntOp $Bool_LicenseShow $Bool_LicenseShow + 1
	${EndIf}
FunctionEnd


#=======================================================================#
#                        ��װ�� -- �Զ��尲װҳ                         #
#=======================================================================#
Function Page.2
    GetDlgItem $0 $HWNDPARENT 1
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 2
    ShowWindow $0 ${SW_HIDE}
    EnableWindow $0 0       ;��ֹȡ����ť���ﵽ����ESC��Ч��
    GetDlgItem $0 $HWNDPARENT 3
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1990
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1991
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1992
    ShowWindow $0 ${SW_HIDE}


    ;���õ�ǰҳ��λ�����˳�ҳ��������
    StrCpy $CurrentPage 5

    nsDialogs::Create 1044
    Pop $0
    ${If} $0 == error
        Abort
    ${EndIf}
    SetCtlColors $0 ""  transparent ;�������͸��

    ;${NSW_SetWindowSize} $0 $DialogWidth $DialogHight ;�ı�Page��С   588 291
    ${NSW_SetWindowSize} $0 601 427 ;�ı�Page��С   588 291

    ;Ӧ����
;    ${NSD_CreateLabel} 270 70 300 60 "${PRODUCT_NAME}"
;    Pop $Lbl_AppName
;    SetCtlColors $Lbl_AppName "FFFFFF" transparent ;�������͸��
;    CreateFont $1 "΢���ź�" "28" "400"
;    SendMessage $Lbl_AppName ${WM_SETFONT} $1 0
    ;;${NSD_AddStyle} $Lbl_AppName ${ES_CENTER}
;    GetFunctionAddress $0 onGUICallback
;    WndProc::onCallback $Lbl_AppName $0 ;�����ޱ߿����ƶ�
;    GetFunctionAddress $3 HideBrowserEdit
;    SkinBtn::onClick $Lbl_AppName $3

    ;��������
    ${NSD_CreateLabel} 5u 5u 115u 12u "${PRODUCT_NAME}" ; ��װ��"
    Pop $Lbl_Title
    ;SetCtlColors $Lbl_Title "" 0xFFFFFF ;��ɫ
    SetCtlColors $Lbl_Title "FFFFFF"  transparent ;�������͸��
    CreateFont $1 "΢���ź�" "11" "800"
    SendMessage $Lbl_Title ${WM_SETFONT} $1 0
    ;${NSD_AddStyle} $Lbl_Title ${ES_CENTER}
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $Lbl_Title $0 ;�����ޱ߿����ƶ�
    GetFunctionAddress $3 HideBrowserEdit
    SkinBtn::onClick $Lbl_Title $3

    ;������װ��ť
    ${NSD_CreateButton} 470 379 105 38 ""
    Pop $Btn_Install
    SkinBtn::Set /IMGID=$PLUGINSDIR\Install.bmp $Btn_Install
    GetFunctionAddress $3 OnInstall
    SkinBtn::onClick $Btn_Install $3
    ${NSD_SetFocus} $Btn_Install
    
    ;���ؿ��ٰ�װ��ť
    ${NSD_CreateButton} 365 379 84 38 ""
    Pop $Btn_Back
    SkinBtn::Set /IMGID=$PLUGINSDIR\Back.bmp $Btn_Back
    GetFunctionAddress $3 OnPre
    SkinBtn::onClick $Btn_Back $3

    ;��װλ������
    ${NSD_CreateLabel} 45 65 80 20 "��װλ�ã�"
    Pop $2
    SetCtlColors $2 "FFFFFF" transparent ;�������͸��
    CreateFont $1 "΢���ź�" "12" "800"
    SendMessage $2 ${WM_SETFONT} $1 0
    ;;${NSD_AddStyle} $Lbl_AppName ${ES_CENTER}
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $2 $0 ;�����ޱ߿����ƶ�
    
    ;���ÿռ�  Txt_SpaceValue
    ${NSD_CreateLabel} 125 66 160 20 ""
    Pop $Txt_SpaceValue
    SetCtlColors $Txt_SpaceValue "FFFFFF" "54BBC2" ;�������͸��
    CreateFont $1 "΢���ź�" "10" "100"
    SendMessage $Txt_SpaceValue ${WM_SETFONT} $1 0
    ${NSD_AddStyle} $Txt_SpaceValue ${ES_LEFT}
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $2 $0 ;�����ޱ߿����ƶ�
    Call GetDriveSpace
    
    
    ;������װĿ¼�����ı���
    ;${NSD_CreateText} 24 150 350 24 $INSTDIR
    ${NSD_CreateText} 45 95 440 38 $INSTDIR
    Pop $Txt_Browser
    ${NSD_SetTextLimit} $Txt_Browser 255
    ;SkinBtn::Set /IMGID=$PLUGINSDIR\InstallDir.bmp $Txt_Browser
    SetCtlColors $Txt_Browser "FFFFFF" "49AAAF"  ;�������͸��   "096FA9" ; transparent;
    CreateFont $1 "΢���ź�" "14" "150"
    SendMessage $Txt_Browser ${WM_SETFONT} $1 0
    ShowWindow $Txt_Browser ${SW_HIDE}
    ${NSD_OnChange} $Txt_Browser EditTextChanged
    
    ;������װĿ¼��ť
    ;${NSD_CreateText} 24 150 350 24 $INSTDIR
    ${NSD_CreateButton} 45 95 440 40 $INSTDIR
    Pop $Btn_TxtBrowser
    SkinBtn::Set /IMGID=$PLUGINSDIR\InstallDir.bmp $Btn_TxtBrowser
    SetCtlColors $Btn_TxtBrowser "FFFFFF" transparent ;�������͸��   "096FA9" ; transparent;
    CreateFont $1 "΢���ź�" "14" "150"
    SendMessage $Btn_TxtBrowser ${WM_SETFONT} $1 0
    GetFunctionAddress $3 ShowBrowserEdit
    SkinBtn::onClick $Btn_TxtBrowser $3
    
    ;;Բ��
;    System::Alloc 16
;    System::Call user32::GetWindowRect(i$Txt_Browser,isR0)
;    System::Call *$R0(i.R1,i.R2,i.R3,i.R4)
;    IntOp $R3 $R3 - $R1
;    IntOp $R4 $R4 - $R2
;    System::Call gdi32::CreateRoundRectRgn(i0,i0,iR3,iR4,i8,i8)i.r0
;    System::Call user32::SetWindowRgn($Txt_Browser, ir0, i1)
;    System::Free $R0

    ;�����ť
    ${NSD_CreateButton} 484 95 71 38  ""
    Pop $btn_Browser
    SkinBtn::Set /IMGID=$PLUGINSDIR\Browse.bmp $btn_Browser
    GetFunctionAddress $3 onButtonClickSelectPath
    SkinBtn::onClick $btn_Browser $3
    
    ;���������ݷ�ʽCheckBox
    ${NSD_CreateButton} 45 150 17 17 ""
    Pop $Chk_Desktop
    SkinBtn::Set /IMGID=$PLUGINSDIR\Desktop_Checked.bmp $Chk_Desktop
    GetFunctionAddress $3 SetDesktop
    SkinBtn::onClick $Chk_Desktop $3
    
    ${NSD_CreateLabel} 70 149 160 20 "����������ݷ�ʽ"
    Pop $2
    SetCtlColors $2 "FFFFFF" transparent ;�������͸��
    CreateFont $1 "΢���ź�" "10" "100"
    SendMessage $2 ${WM_SETFONT} $1 0
    ${NSD_AddStyle} $2 ${ES_LEFT}
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $2 $0 ;�����ޱ߿����ƶ�
    
    Call InitDesktop
    
    ;��������������CheckBox
    ${NSD_CreateButton} 45 180 17 17 ""
    Pop $Chk_QuickRun
    SkinBtn::Set /IMGID=$PLUGINSDIR\QuickRun_Checked.bmp $Chk_QuickRun
    GetFunctionAddress $3 SetQuickRun
    SkinBtn::onClick $Chk_QuickRun $3
    
    ${NSD_CreateLabel} 70 179 160 20 "���������������"
    Pop $2
    SetCtlColors $2 "FFFFFF" transparent ;�������͸��
    CreateFont $1 "΢���ź�" "10" "100"
    SendMessage $2 ${WM_SETFONT} $1 0
    ${NSD_AddStyle} $2 ${ES_LEFT}
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $2 $0 ;�����ޱ߿����ƶ�
    
    Call InitQuickRun

    ;��С����ť
    ${NSD_CreateButton} 546 1 27 27 ""
    Pop $0
    SkinBtn::Set /IMGID=$PLUGINSDIR\btnmini.bmp $0
    GetFunctionAddress $3 OnMini
    SkinBtn::onClick $0 $3
    
    ;�رհ�ť
    ${NSD_CreateButton} 573 1 27 27 ""
    Pop $0
    SkinBtn::Set /IMGID=$PLUGINSDIR\btnClose.bmp $0
    GetFunctionAddress $3 OnClickClose
    SkinBtn::onClick $0 $3

    ;������ͼ��
;    ${NSD_CreateBitmap} 195 64 55 57 ""
;    Pop $IconImage
;    ${NSD_SetImage} $IconImage $PLUGINSDIR\icon.bmp $IconHandle
;    GetFunctionAddress $0 onGUICallback
;    WndProc::onCallback $IconImage $0 ;�����ޱ߿����ƶ�
;    GetFunctionAddress $3 HideBrowserEdit
;    SkinBtn::onClick $IconImage $3

    ;��������ͼ
    ${NSD_CreateBitmap} 0 0 100% 100% ""
    Pop $BGImage
    ${NSD_SetImage} $BGImage $PLUGINSDIR\bg.bmp $ImageHandle
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $BGImage $0 ;�����ޱ߿����ƶ�
    GetFunctionAddress $3 HideBrowserEdit
    SkinBtn::onClick $BGImage $3
    
    nsDialogs::Show
    ${NSD_FreeImage} $IconHandle
    ${NSD_FreeImage} $ImageHandle
FunctionEnd

Function ShowBrowserEdit
  ShowWindow $Btn_TxtBrowser ${SW_HIDE}
  ShowWindow $Txt_Browser ${SW_SHOW}
FunctionEnd

Function HideBrowserEdit
  ShowWindow $Btn_TxtBrowser ${SW_SHOW}
  ShowWindow $Txt_Browser ${SW_HIDE}
FunctionEnd

Function EditTextChanged
  ${NSD_GetText} $Txt_Browser $0
  StrCpy $INSTDIR $0
  ${NSD_SetText} $Btn_TxtBrowser $0

  ${GetRoot} "$INSTDIR" $R3   ;��ȡ��װ��Ŀ¼
  StrCpy $R0 "$R3\"
  StrCpy $R1 "invalid"
  ${GetDrives} "ALL" "HDDDetection"            ;��ȡ��Ҫ��װ�ĸ�Ŀ¼��������
  ${If} $R1 == HDD              ;��Ӳ��
     EnableWindow $Btn_Install 1
     Call GetDriveSpace
  ${Else}
     EnableWindow $Btn_Install 0
  ${Endif}
  ;system::Call `user32::SetWindowText(i $Btn_TxtBrowser, t "$INSTDIR")`
FunctionEnd

Function HDDDetection
  ${if} $R0 == $9    ;$9���̷�;$8������
    StrCpy $R1 $8
  ${endif}
  Push $0
FunctionEnd

Function GetDriveSpace
  ${GetRoot} "$INSTDIR" $R3   ;��ȡ��װ��Ŀ¼
  StrCpy $R0 "$R3\"
  ${DriveSpace}  "$R0" "/D=F /S=M" $R5
  ${If} $R5 < 1024
    ${NSD_SetText} $Txt_SpaceValue "($R3 �̿��ÿռ�:$R5M)"
  ${Else}
    IntOp $R6 $R5 / 1024
    ${NSD_SetText} $Txt_SpaceValue "($R3 �̿��ÿռ�:$R6G)"
  ${Endif}
FunctionEnd

;��������ݷ�ʽ����¼�
Function SetDesktop
         ${If} $Bool_Desktop == 1
               Call Desktop_NoChecked
               Strcpy $Bool_Desktop 0
        ${Else}
               Call Desktop_Checked
               Strcpy $Bool_Desktop 1
        ${EndIf}
FunctionEnd

Function InitDesktop
         ${If} $Bool_Desktop == 1
               Call Desktop_Checked
        ${Else}
               Call Desktop_NoChecked
        ${EndIf}
FunctionEnd

;����ѡ��״̬ʱ�ı���ͼƬ
Function Desktop_Checked
  SkinBtn::Set /IMGID=$PLUGINSDIR\Desktop_Checked.bmp $Chk_Desktop
FunctionEnd

;����δѡ��״̬ʱ�ı���ͼƬ
Function Desktop_NoChecked
  SkinBtn::Set /IMGID=$PLUGINSDIR\Desktop_NoChecked.bmp $Chk_Desktop

FunctionEnd

;��ӿ����������¼�
Function SetQuickRun
         ${If} $Bool_QuickRun == 1
               Call QuickRun_NoChecked
               Strcpy $Bool_QuickRun 0
        ${Else}
               Call QuickRun_Checked
               Strcpy $Bool_QuickRun 1
        ${EndIf}
FunctionEnd

Function InitQuickRun
         ${If} $Bool_QuickRun == 1
               Call QuickRun_Checked
        ${Else}
               Call QuickRun_NoChecked
        ${EndIf}
FunctionEnd

;����ѡ��״̬ʱ�ı���ͼƬ
Function QuickRun_Checked
  SkinBtn::Set /IMGID=$PLUGINSDIR\QuickRun_Checked.bmp $Chk_QuickRun
FunctionEnd

;����δѡ��״̬ʱ�ı���ͼƬ
Function QuickRun_NoChecked
  SkinBtn::Set /IMGID=$PLUGINSDIR\QuickRun_NoChecked.bmp $Chk_QuickRun
FunctionEnd

#----------------------------------------------
#��2��ҳ�����  -- ��װ����ҳ��
#----------------------------------------------

Function  InstFilesPageShow
    FindWindow $R2 "#32770" "" $HWNDPARENT
    StrCpy $DefaultPage $R2
    GetDlgItem $0 $HWNDPARENT 1
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 2
    ShowWindow $0 ${SW_HIDE}
    EnableWindow $0 0       ;��ֹȡ����ť���ﵽ����ESC��Ч��
    GetDlgItem $0 $HWNDPARENT 3
    ShowWindow $0 ${SW_HIDE}
    
    GetDlgItem $1 $R2 1027
    ShowWindow $1 ${SW_HIDE}
    
    GetDlgItem $1 $R2 1990  ;��С��
    ShowWindow $1 ${SW_HIDE}
    GetDlgItem $1 $R2 1991 ;�ر�
    ShowWindow $1 ${SW_HIDE}
    EnableWindow $1 0
    GetDlgItem $1 $R2 1994  ;��װ
    ShowWindow $1 ${SW_HIDE}
    ;EnableWindow $1 0
    
    ;w7tbp::Start

    StrCpy $R0 $R2 ;�ı�ҳ���С,��Ȼ��ͼ����ȫҳ
    System::Call "user32::MoveWindow(i R0, i 0, i 0, i 601, i 427) i R2"        ;�޸Ĵ�С   588 291
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $R0 $0 ;�����ޱ߿����ƶ�

    GetDlgItem $R0 $R2 1004  ;���ý�����λ��
    System::Call "user32::MoveWindow(i R0, i 15, i 400, i 562, i 6) i R2"
    Strcpy $PB_ProgressBar $R0
    SkinProgress::Set $PB_ProgressBar "$PLUGINSDIR\Progress.bmp" "$PLUGINSDIR\ProgressBar.bmp"
    
    ;��������
    GetDlgItem $R0 $R2 1993  
    System::Call "user32::MoveWindow(i R0, i 8, i 8, i 160, i 18) i R2"    ;ע�⣺����ϵͳAPI�������� R0 ~ R9, ����Ч��
    StrCpy $Lbl_Title $R0
    SetCtlColors $Lbl_Title "FFFFFF"  transparent ;�������͸��
    ${NSD_SetText} $Lbl_Title "${PRODUCT_NAME}" ; ��װ��"
    CreateFont $1 "΢���ź�" "11" "800"
    SendMessage $Lbl_Title ${WM_SETFONT} $1 0
    
    ;Ӧ����
;    ;${NSD_CreateLabel} 270 70 300 60 "${PRODUCT_NAME}"
;    GetDlgItem $R0 $R2 1997  ;����Ӧ����λ��
;    System::Call "user32::MoveWindow(i R0, i 250, i 70, i 300, i 60) i R2"
;    StrCpy $Lbl_AppName $R0
;    SetCtlColors $Lbl_AppName "FFFFFF" transparent ;�������͸��
;    ${NSD_SetText} $Lbl_AppName "${PRODUCT_NAME}"
;    CreateFont $1 "΢���ź�" "28" "400"
;    SendMessage $Lbl_AppName ${WM_SETFONT} $1 0

    ;DetailPrint ���� -- "���ڰ�װ"
    GetDlgItem $R1 $R2 1006  ;��ȡ1006�ؼ�������ɫ���ı�λ��
    System::Call "user32::MoveWindow(i R1, i 15, i 376, i 200, i 40) i R2"
    SetCtlColors $R1 "888888"  transparent ;�������F6F6F6,ע����ɫ������Ϊ͸���������ص�
    CreateFont $1 "΢���ź�" "10" "50"
    SendMessage $R1 ${WM_SETFONT} $1 0
    
    GetDlgItem $R0 $R2 1992  ;��ȡ1992�ؼ�������ɫ���ı�λ��   [�ð�ť����ʾ]      1997 ���ı�
    ShowWindow $R0 ${SW_HIDE}
    ;��������
    GetDlgItem $R0 $R2 1997  ;��ȡ1992�ؼ�������ɫ���ı�λ��   [�ð�ť����ʾ]      1997 ���ı�
    ShowWindow $R0 ${SW_HIDE}
    System::Call "user32::MoveWindow(i R0, i 90, i 376, i 40, i 20, i 1) i R2"
    SetCtlColors $R0 "888888" "FFFFFF" ;�������F6F6F6,ע����ɫ������Ϊ͸���������ص�
    CreateFont $1 "΢���ź�" "10" "50"
    SendMessage $R0 ${WM_SETFONT} $1 0
    Strcpy $PPercent $R0
    ${NSD_SetText} $PPercent "0%"

    ;����ͼ��
    GetDlgItem $R0 $R2 1996
    System::Call "user32::MoveWindow(i R0, i 72, i 49, i 311, i 306) i R2"
    ${NSD_SetImage} $R0 $PLUGINSDIR\Intro.bmp $IconHandle
    ;��������ͼ
    GetDlgItem $R0 $R2 1995
    System::Call "user32::MoveWindow(i R0, i 0, i 0, i 601, i 427) i R2"      ;�޸Ĵ�С   588 291
    ${NSD_SetImage} $R0 $PLUGINSDIR\bg.bmp $ImageHandle

    #����WndSubclass����趨timer������ַ�����Թ�ֱ��Ϊsettimer�趨������ַ����û�в��֧�ֺ����ѣ�
    ;${WndSubclass_Subclass} $HWNDPARENT GetProgressPos $SubProgress $SubProgress
    ;System::Call user32::SetTimer(i $DefaultPage, i 10, i 20, i 0)
    ;GetFunctionAddress $2 GetProgressPos
    ;ThreadTimer::Start 200 0 $2
FunctionEnd

Function GetProgressPos
  SendMessage $PB_ProgressBar 0x0407 0 0 $1
  SendMessage $PB_ProgressBar 0x0408 0 0 $2
  ${If} $2 > 0
  ${If} $1 > 0
    IntOp $2 $2 * 100
    IntOp $1 $2 / $1
    ;SendMessage $PB_ProgressBar ${PBM_GETPOS} 0 0 $1
   ${NSD_SetText} $PPercent "$1%"
    ${if} $1 >= 99
      ThreadTimer::Stop
      ;System::Call user32::KillTimer(i $DefaultPage, i 10)
      ${NSD_SetText} $PPercent "100%"
    ${Endif}
  ${Endif}
  ${Endif}
FunctionEnd

Function InstFileLeave
  ${IfThen} ${Abort} ${|} Quit ${|}
FunctionEnd


#=======================================================================#
#                          ��װ�� -- ���ҳ��                           #
#=======================================================================#
Function Page.3
    GetDlgItem $0 $HWNDPARENT 1
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 2
    ShowWindow $0 ${SW_HIDE}
    EnableWindow $0 0       ;��ֹȡ����ť���ﵽ����ESC��Ч��
    GetDlgItem $0 $HWNDPARENT 3
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1990
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1991
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1992
    ShowWindow $0 ${SW_HIDE}

    ;���õ�ǰҳ��λ�����˳�ҳ��������
    StrCpy $CurrentPage 2


    nsDialogs::Create 1044
    Pop $0
    ${If} $0 == error
        Abort
    ${EndIf}
    SetCtlColors $0 ""  transparent ;�������͸��
    ;${NSW_SetWindowSize} $0 $DialogWidth $DialogHight ;�ı�Page��С   588 291
    ${NSW_SetWindowSize} $0 601 427 ;�ı�Page��С   588 291

    ;��װ���
    ${NSD_CreateLabel} 195 205 300 60 "��װ���"
    Pop $Lbl_AppName
    SetCtlColors $Lbl_AppName "FFFFFF" transparent ;�������͸��
    CreateFont $1 "΢���ź�" "38" "400"
    SendMessage $Lbl_AppName ${WM_SETFONT} $1 0
    ;;${NSD_AddStyle} $Lbl_AppName ${ES_CENTER}
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $Lbl_AppName $0 ;�����ޱ߿����ƶ�

    ;��������
    ${NSD_CreateLabel} 5u 5u 115u 12u "${PRODUCT_NAME}" ; ��װ��"
    Pop $Lbl_Title
    ;SetCtlColors $Lbl_Title "" 0xFFFFFF ;��ɫ
    SetCtlColors $Lbl_Title "FFFFFF"  transparent ;�������͸��
    CreateFont $1 "΢���ź�" "11" "800"
    SendMessage $Lbl_Title ${WM_SETFONT} $1 0
    ;${NSD_AddStyle} $Lbl_Title ${ES_CENTER}
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $Lbl_Title $0 ;�����ޱ߿����ƶ�
    
    ;��С����ť
    ${NSD_CreateButton} 546 1 27 27 ""
    Pop $0
    SkinBtn::Set /IMGID=$PLUGINSDIR\btnmini.bmp $0
    GetFunctionAddress $3 OnMini
    SkinBtn::onClick $0 $3

    ;�رհ�ť
    ${NSD_CreateButton} 573 1 27 27 ""
    Pop $0
    SkinBtn::Set /IMGID=$PLUGINSDIR\btnClose.bmp $0
    GetFunctionAddress $3 OnClickClose
    SkinBtn::onClick $0 $3
    ;EnableWindow $0 0
    
        ;��ɰ�ť    ;${NSD_CreateButton} 232 158 122 47 ""
    ${NSD_CreateButton} 470 379 105 38 ""
    Pop $Btn_Done
    ${NSD_SetFocus} $Btn_Done
    SkinBtn::Set /IMGID=$PLUGINSDIR\Done.bmp $Btn_Done
    GetFunctionAddress $5 OnInstallDoneRun
    SkinBtn::onClick $Btn_Done $5


    ;������ͼ��
    ${NSD_CreateBitmap} 254 90 93 96 ""
    Pop $BGImage
    ${NSD_SetImage} $BGImage $PLUGINSDIR\IconDone.bmp $ImageHandle
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $BGImage $0 ;�����ޱ߿����ƶ�

    ;��������ͼ
    ${NSD_CreateBitmap} 0 0 100% 100% ""
    Pop $BGImage
    ${NSD_SetImage} $BGImage $PLUGINSDIR\bg.bmp $ImageHandle
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $BGImage $0 ;�����ޱ߿����ƶ�
    
    nsDialogs::Show
    ${NSD_FreeImage} $ImageHandle
FunctionEnd


Function Page.4

FunctionEnd

#=======================================================================#
#                     ��װ�� -- ��ʾ�˳�ҳ��                            #
#=======================================================================#
Function Page.Exit
    GetDlgItem $0 $HWNDPARENT 1
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 2
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 3
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1990
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1991
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1992
    ShowWindow $0 ${SW_HIDE}

    nsDialogs::Create 1044
    Pop $0
    ${If} $0 == error
        Abort
    ${EndIf}
    SetCtlColors $0 ""  transparent ;�������͸��

    ;${NSW_SetWindowSize} $0 $DialogWidth $DialogHight ;�ı�Page��С   588 291
    ${NSW_SetWindowSize} $0 601 427 ;�ı�Page��С   588 291

    ;�˳���ʾ����
    ${NSD_CreateLabel} 55 249 493U 20u "��ȷ��Ҫ�˳�${PRODUCT_NAME}�İ�װ������"
    Pop $Lbl_AppName
    SetCtlColors $Lbl_AppName "FFFFFF" transparent ;�������͸��
    CreateFont $1 "΢���ź�" "16" "220"
    SendMessage $Lbl_AppName ${WM_SETFONT} $1 0
    ${NSD_AddStyle} $Lbl_AppName ${ES_CENTER}
    
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $Lbl_AppName $0 ;�����ޱ߿����ƶ�

    ;��������
    ${NSD_CreateLabel} 5u 5u 115u 12u "${PRODUCT_NAME}" ; ��װ��"
    Pop $Lbl_Title
    ;SetCtlColors $Lbl_Title "" 0xFFFFFF ;��ɫ
    SetCtlColors $Lbl_Title "FFFFFF"  transparent ;�������͸��
    CreateFont $1 "΢���ź�" "11" "800"
    SendMessage $Lbl_Title ${WM_SETFONT} $1 0
    ;${NSD_AddStyle} $Lbl_Title ${ES_CENTER}
    
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $Lbl_Title $0 ;�����ޱ߿����ƶ�

    ;��(�˳�)��ť ${NSD_CreateButton} 207 163 77 28 ""
    ${NSD_CreateButton} 470 379 105 38 ""
    Pop $Btn_Yes
    SkinBtn::Set /IMGID=$PLUGINSDIR\Yes.bmp $Btn_Yes
    SetCtlColors $Btn_Yes "808080"  transparent ;�������͸��
    CreateFont $1 "΢���ź�" "16" "500"
    SendMessage $Btn_Yes ${WM_SETFONT} $1 0
    ${NSD_AddStyle} $Btn_Yes ${ES_CENTER}

    GetFunctionAddress $3 OnExit
    SkinBtn::onClick $Btn_Yes $3

    ;��(ȡ��)��ť ${NSD_CreateButton} 303 163 77 28 ""
    ${NSD_CreateButton} 365 379 84 38 ""
    Pop $Btn_No
    SkinBtn::Set /IMGID=$PLUGINSDIR\No.bmp $Btn_No
    SetCtlColors $Btn_No "0d6190"  transparent ;�������͸��
    CreateFont $1 "΢���ź�" "16" "500" ;
    SendMessage $Btn_No ${WM_SETFONT} $1 0
    ${NSD_AddStyle} $Btn_No ${ES_CENTER}

    GetFunctionAddress $3 OnClickCancel
    SkinBtn::onClick $Btn_No $3
    
    ;��С����ť
    ${NSD_CreateButton} 546 1 27 27 ""
    Pop $0
    SkinBtn::Set /IMGID=$PLUGINSDIR\btnmini.bmp $0
    GetFunctionAddress $3 OnMini
    SkinBtn::onClick $0 $3

    ;�رհ�ť
    ${NSD_CreateButton} 573 1 27 27 ""
    Pop $0
    SkinBtn::Set /IMGID=$PLUGINSDIR\btnClose.bmp $0
    GetFunctionAddress $3 OnClickClose
    SkinBtn::onClick $0 $3
    EnableWindow $0 0

    ;������ͼ��
    ${NSD_CreateBitmap} 227 72 146 146 ""
    Pop $BGImage
    ${NSD_SetImage} $BGImage $PLUGINSDIR\Icon.bmp $ImageHandle
    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $BGImage $0 ;�����ޱ߿����ƶ�

    ;��������ͼ
    ${NSD_CreateBitmap} 0 0 100% 100% ""
    Pop $BGImage
    ${NSD_SetImage} $BGImage $PLUGINSDIR\bg.bmp $ImageHandle

    GetFunctionAddress $0 onGUICallback
    WndProc::onCallback $BGImage $0 ;�����ޱ߿����ƶ�
    nsDialogs::Show
    ${NSD_FreeImage} $ImageHandle
FunctionEnd

#----------------------------------------------
#ж������
#----------------------------------------------
Function UnInstallDirver
  ReadRegStr $R0 HKLM \
  "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
  "UninstallString"
  ${GetParent} "$R0" $R1
  StrCmp $R0 "" undone
  IfFileExists $R0 uninstdirver
	Goto undone
;����ж�س���
uninstdirver:
  ;MessageBox MB_ICONINFORMATION|MB_OK "$R1\${ADBMGR_DIR}\${ADBMGR_PROGRAM_NAME}.exe -uninstall "
  ExecWait "$R1\${ADBMGR_DIR}\${ADBMGR_PROGRAM_NAME}.exe -uninstall " ;����$R0Ϊ��ȡ����ж�س������ƣ� /S�Ǿ�Ĭж�ز���ʹ��NSIS���ɵ�ж�س������Ҫ���� _? ���ܵȴ�ж�ء�$R1�����λ��
undone:
FunctionEnd

#----------------------------------------------
#��װ����������һ����
#----------------------------------------------
Function CloseInstallUI
         Sleep 100
         Call OnNext
         ;SetAutoClose true
FunctionEnd

;Function OnAutoClose
;  GetFunctionAddress $0 OnAutoClose
;  nsDialogs::KillTimer $0
;  GetFunctionAddress $0 CloseInstallUI
;  BgWorker::CallAndWait #����һ�������ȼ��ĺ�̨�߳�
;FunctionEnd

#----------------------------------------------
#��װ��ɽ��水ť
#----------------------------------------------
Function OnInstallDoneRun
  ExecShell "open" "$INSTDIR\${PROGRAM_NAME}.exe"
  ;Exec "$INSTDIR\${PROGRAM_NAME}.exe"
  ;System::Alloc 4*18			;// $1 = struct STARTUPINFO
  ;Pop $1
  ;System::Alloc 4*4			;// $2 = struct PROCESS_INFORMATION
  ;Pop $2
  ;System::Call "*$1(i 4*18)"		;// StartUp.cb=sizeof(StartUp);
  ;System::Call \
  ;  'kernel32::CreateProcess(i 0, t "$INSTDIR\${PROGRAM_NAME}.exe", i 0, i 0, i 0, \
	;			i 0, i 0, i 0, i r1, i.r2)i.r0'
  ;System::Free $1
  ;System::Free $2
  Call OnExit
FunctionEnd


#----------------------------------------------
#ִ�а�װ�¼�
#----------------------------------------------
Section MainSetup
  ;Call KillAllApp
  Sleep 50
  SetDetailsPrint textonly
  DetailPrint "���ڰ�װ..."
  SetDetailsPrint none  ;����ʾ��Ϣ
  SetOverwrite on
  ;nsisSlideshow::Show /NOUNLOAD /auto=$PLUGINSDIR\Slides.dat
  ;CopyFiles "ImportantFile\*.*" "${PRODUCT_NAME}${PRODUCT_VERSION}"
  SetOutPath $INSTDIR
  File /r "${PRODUCT_NAME}${PRODUCT_VERSION}\*"
  File /r "${MUST_PACKAGE_FOLDER_NAME}\*"

  ;CopyFiles $INSTDIR\${BIN_DIR}\*.ttf $INSTDIR\${BIN_DIR}

  ;GetFunctionAddress $0 CloseInstallUI
  ;BgWorker::CallAndWait #����һ�������ȼ��ĺ�̨�߳�
  ;nsisSlideshow::Stop
  Call ReportInstall
  Sleep 100
  SetAutoClose true
SectionEnd

#----------------------------------------------
#�����������ж�س�����Ϣ ,����ľ����÷����鿴����  D.2 ���ж����Ϣ�����/ɾ���������  �����ڰ����������ؼ��ʣ��磺DisplayName
#----------------------------------------------

#----------------------------------------------------------------------------------------------------------------------------------
#
#             ;����ж���ļ�  PS�������ж���ļ������������ɺ����ǩ�ź�������Ŀ¼����ע�͵���仰 zcs
#
#----------------------------------------------------------------------------------------------------------------------------------

Section -Post
  ;WriteUninstaller "$INSTDIR\Uninstall.exe"     ;����ж���ļ�  PS�������ж���ļ������������ɺ����ǩ�ź�������Ŀ¼����ע�͵���仰 zcs

  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\${PROGRAM_NAME}.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\${PROGRAM_NAME}.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

Section -AdditionalIcons
  SetOverwrite on
  SetShellVarContext all   ;����Ϊ�����û� Win7����Ӱ��      
  
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"     ;��������Ŀ¼

  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"   ;����������ݴ�

  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\ж��${PRODUCT_NAME}.lnk" "$INSTDIR\Uninstall.exe"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}.lnk" "$INSTDIR\${PROGRAM_NAME}.exe"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\${PROGRAM_NAME}.exe"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_WEB_LINK_NAME}.lnk" "$INSTDIR\${PRODUCT_NAME}.url" "${PRODUCT_WEB_LINK_NAME}" "$INSTDIR\Browser.ico" ;"C:\Program Files\Internet Explorer\iexplore.exe"
  
  
  ;�����ݷ�ʽ
  ${If} $Bool_Desktop == 1
    CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\${PROGRAM_NAME}.exe"
    CreateShortCut "$DESKTOP\${PRODUCT_WEB_LINK_NAME}.lnk" "$INSTDIR\${PRODUCT_NAME}.url" "${PRODUCT_WEB_LINK_NAME}" "$INSTDIR\Browser.ico" ;"C:\Program Files\Internet Explorer\iexplore.exe"
  ${Endif}
  
  ;��������������
  
  ${If} $Bool_QuickRun == 1
    ;CreateShortCut "$STARTMENU\${PRODUCT_NAME}.lnk" "$INSTDIR\${PROGRAM_NAME}.exe"
    CreateShortCut "$QUICKLAUNCH\${PRODUCT_NAME}.lnk" "$INSTDIR\${PROGRAM_NAME}.exe"
    ;CreateShortCut "$QUICKLAUNCH\User Pinned\TaskBar\${PRODUCT_NAME}.lnk" "$INSTDIR\${PROGRAM_NAME}.exe"  ;���ô���, taskbarpin ʱ���Զ�����
;    GetVersion::WindowsVersion
;    Pop $R0
;    StrCpy $R2 $R0
;    GetVersion::WindowsServicePackBuild
;    Pop $R0
;    ${If} ${AtMostWin8.1}
    ${GetFileVersion} "$SYSDIR\kernel32.dll" $R1        ; x.x.x.x         6.2.7601.12345
    ${WordFind} "$R1" "." +3 $R0   ; �õ���ǰ�汾���������� ���ܻ��ǰ��ģ�ĳЩϵͳ������ȥ�Ļ������
    
    ${If} $R0 < 10000
      ;������ʼ�˵���ݷ�ʽ
      ExecShell "startpin" "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"
      ;������������ݷ�ʽ
      ExecShell "taskbarpin" "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"
    ${EndIf}
  ${EndIf}
  System::Call 'shell32::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'
  ;SendMessage $R0 ${WM_ERASEBKGND} $0 $0
  ;System::Call User32::RedrawWindow(i 0, i 0, i 0, i 133)
  ;CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\ʹ��˵��.lnk" "$INSTDIR\${PRODUCT_HELPFILE}"
SectionEnd

;����ҳ����ת������
Function RelGotoPage
  IntCmp $R9 0 0 Move Move
    StrCmp $R9 "X" 0 Move
      StrCpy $R9 "120"
  Move:
  SendMessage $HWNDPARENT "0x408" "$R9" ""
FunctionEnd

Function OnNext
  StrCpy $R9 1 ;Goto the next page
  Call RelGotoPage
  Abort
FunctionEnd

Function OnPre
  StrCpy $R9 -1 ;Goto the Pre page
  Call RelGotoPage
  Abort
FunctionEnd

#----------------------------------------------
#��ת���ر�ҳ��
#----------------------------------------------
Function OnClickClose
  StrCpy $R9 $CurrentPage ;Goto the next page
  Call RelGotoPage
  Abort
FunctionEnd

#----------------------------------------------
#�ر�ҳ��ȡ��
#----------------------------------------------
Function OnClickCancel
  StrCpy $R9 -$CurrentPage ;Goto the pre page
  Call RelGotoPage
  Abort
FunctionEnd

Function OnQucikInstall
  StrCpy $R9 2 ;Goto the next page
  Call RelGotoPage
  Abort
FunctionEnd

Function OnInstall
  ${NSD_GetText} $Txt_Browser  $R0  ;������õİ�װ·��
  ;�ж�Ŀ¼�Ƿ���ȷ
	ClearErrors
	CreateDirectory "$R0"
	IfErrors 0 +3
  MessageBox MB_ICONINFORMATION|MB_OK "'$R0' ��װĿ¼�����ڣ����������á�"
  Return

	StrCpy $INSTDIR  $R0  ;���氲װ·��

	;������һҳ�� $R9��NavigationGotoPage ������Ҫ����ת��������
  StrCpy $R9 1
  call RelGotoPage
FunctionEnd

#------------------------------------------
#��С������
#------------------------------------------
Function OnMini
System::Call user32::CloseWindow(i$HWNDPARENT) ;��С��
FunctionEnd

#------------------------------------------
#�رմ���
#------------------------------------------
Function OnExit
;SendMessage $hwndparent ${WM_CLOSE} 0 0 ;�ر�
  RMDir /r /REBOOTOK "$PLUGINSDIR"
  ;DeleteRegKey  ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  System::Call kernel32::ExitProcess(0) ;�ر�
FunctionEnd

#--------------------------------------------------------
# ·��ѡ��ť�¼�����Windowsϵͳ�Դ���Ŀ¼ѡ��Ի���
#--------------------------------------------------------
Function onButtonClickSelectPath
  Pop $0
  Push $INSTDIR ; input string "C:\Program Files\ProgramName"
  Call GetParent
  Pop $R0 ; first part "C:\Program Files"

  Push $INSTDIR ; input string "C:\Program Files\ProgramName"
  Push "\" ; input chop char
  Call GetLastPart
  Pop $R1 ; last part "ProgramName"

  nsDialogs::SelectFolderDialog "��ѡ�� $R1 ��װ���ļ���:" "$R0"
  Pop $0
  ${If} $0 == "error" # returns 'error' if 'cancel' was pressed?
    Return
  ${EndIf}
  ${If} $0 != ""
    StrLen $1 $0 ;�ж��Ƿ��Ǹ�Ŀ¼
    ${If} $1 == 3
    StrCpy $INSTDIR "$0$R1"
    ${Else}
    StrCpy $INSTDIR "$0\$R1"
    ${EndIf}
    ;${NSD_SetText} $Txt_Browser ""
    ;SetCtlColors $Txt_Browser "FFFFFF" transparent ;�������͸��
    ;CreateFont $1 "΢���ź�" "12" "100"
    ;SendMessage $Txt_Browser ${WM_SETFONT} $1 1
    ;system::Call `user32::SetWindowText(i $Txt_Browser, t "$INSTDIR")`
    ;system::Call `user32::SetWindowText(i $Btn_TxtBrowser, t "$INSTDIR")`
    ${NSD_SetText} $Txt_Browser $INSTDIR
    ${NSD_SetText} $Btn_TxtBrowser $INSTDIR
    ;SetCtlColors $Txt_Browser "FFFFFF" transparent ;�������͸��
    ;CreateFont $1 "΢���ź�" "12" "100"
    ;SendMessage $Txt_Browser ${WM_SETFONT} $1 1
  ${EndIf}

   ;${NSD_GetText} $Txt_Browser  $0
   ;nsDialogs::SelectFolderDialog  "��ѡ�� ${PRODUCT_NAME} ��װĿ¼��"  "$0"
   ;Pop $0
   ;${IfNot} $0 == error
   ;	${NSD_SetText} $Txt_Browser  $0
   ;${EndIf}
FunctionEnd

; Usage:
; Push "C:\Program Files\Directory\Whatever"
; Call GetParent
; Pop $R0 ; $R0 equal "C:\Program Files\Directory"
;�õ�ѡ��Ŀ¼����ƴ�Ӱ�װ��������
Function GetParent
  Exch $R0 ; input string
  Push $R1
  Push $R2
  Push $R3
  StrCpy $R1 0
  StrLen $R2 $R0
  loop:
    IntOp $R1 $R1 + 1
    IntCmp $R1 $R2 get 0 get
    StrCpy $R3 $R0 1 -$R1
    StrCmp $R3 "\" get
    Goto loop
  get:
    StrCpy $R0 $R0 -$R1
    Pop $R3
    Pop $R2
    Pop $R1
    Exch $R0 ; output string
FunctionEnd

; Usage:
; Push $INSTDIR ; input string "C:\Program Files\ProgramName"
; Push "\" ; input chop char
; Call GetLastPart
; Pop $R1 ; last part "ProgramName"
;��ȡѡ��Ŀ¼
Function GetLastPart
  Exch $0 ; chop char
  Exch
  Exch $1 ; input string
  Push $2
  Push $3
  StrCpy $2 0
  loop:
    IntOp $2 $2 - 1
    StrCpy $3 $1 1 $2
    StrCmp $3 "" 0 +3
      StrCpy $0 ""
      Goto exit2
    StrCmp $3 $0 exit1
    Goto loop
  exit1:
    IntOp $2 $2 + 1
    StrCpy $0 $1 "" $2
  exit2:
    Pop $3
    Pop $2
    Pop $1
    Exch $0 ; output string
FunctionEnd

/******************************
 *  �����ǰ�װ���������  *
 ******************************/
;Section "Fonts"
  ;StrCpy $FONT_DIR $FONTS
;  Call InstallTTFFont
;  SendMessage ${HWND_BROADCAST} ${WM_FONTCHANGE} 0 0 /TIMEOUT=5000
;SectionEnd

Function InstallTTFFont
  CopyFiles "$INSTDIR\${BIN_DIR}\*.ttf" "$INSTDIR\${BIN_DIR}"
  StrCpy $0 "$INSTDIR\${BIN_DIR}\${KAITI}"
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts" "��׼�����(TrueType)" "$0"
  System::Call "GDI32::AddFontResource(t) i ('$0') .s"
FunctionEnd

;Section "InstallReport"
;  Call ReportInstall
;SectionEnd

 /******************************
 *  ͳ�ư�װ�������� *
 ******************************/
Function ReportInstall
  StrCpy $R8 "data={$\"eventId$\":1005,$\"eventTime$\":0,$\"params$\":[],$\"userId$\":0,$\"versionCode$\":$\"${PRODUCT_VERSION}$\",$\"ver$\":0}"
  inetc::post "$R8" /SILENT "http://p.101.com/101ppt/pptEvent.php" "$INSTDIR\post.dat"
  Sleep 100
  Delete "$INSTDIR\post.dat"
FunctionEnd

/******************************
 *  �����ǰ�װ�����ж�ز���  *
 ******************************/
Function un.onInit
  SetOverwrite on
  InitPluginsDir
  ;Call un.CreateMutex
  ;��ֹ�����װ����ʵ��
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "PPTShell_Install") i .r1 ?e'
  Pop $R0
  StrCmp $R0 0 +3
  MessageBox MB_OK|MB_ICONEXCLAMATION "��װ����ж�س�����������"
  Abort

  FindProcDLL::FindProc "${PPTSHELL_EXE_NAME}"
  Pop $R0
  IntCmp $R0 1 kill_app no_run
  kill_app:
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1 "${PRODUCT_NAME}�������У��㡰�ǡ����رռ���ж�أ��㡰���˳�ж�س���" IDYES kill IDNO quit
  kill:
    KillProcDLL::KillProc "${PPT_EXE_NAME}"
    KillProcDLL::KillProc "${PPTSHELL_EXE_NAME}"
    Sleep 500
    goto no_run
  quit:
    Quit
  no_run:
  
  ;Call un.KillUpdate
  ;Call un.KillCourse
  KillProcDLL::KillProc "${UPDATE_EXE_NAME}"
  KillProcDLL::KillProc "${COURSE_EXE_NAME}"
  KillProcDLL::KillProc "${NODE_EXE_NAME}"
  
  
  ;kill Adb
;  FindProcDLL::FindProc "adb.exe"
;  Pop $R0
;  IntCmp $R0 1 kill_adb adb_no_run
;  kill_adb:
;    KillProcDLL::KillProc "adb.exe"
;    Sleep 100
;    goto no_run
;  adb_no_run:
  
    
    File `/ONAME=$PLUGINSDIR\bg.bmp` `images\StPad.bmp`
    File `/ONAME=$PLUGINSDIR\icon.bmp` `images\icon.bmp`

    File `/ONAME=$PLUGINSDIR\Yes.bmp` `images\Yes.bmp`
    File `/ONAME=$PLUGINSDIR\No.bmp` `images\No.bmp`
    File `/ONAME=$PLUGINSDIR\Done.bmp` `images\Done.bmp`
    
    File `/oname=$PLUGINSDIR\btnClose.bmp` `images\close.bmp`
    File `/oname=$PLUGINSDIR\btnmini.bmp` `images\mini.bmp`

    ;������Ƥ��
    File `/oname=$PLUGINSDIR\Progress.bmp` `images\Progress.bmp`
    File `/oname=$PLUGINSDIR\ProgressBar.bmp` `images\ProgressBar.bmp`
    File `/oname=$PLUGINSDIR\IconDone.bmp` `images\IconDone.bmp`
    File `/oname=$PLUGINSDIR\IconUnInstall.bmp` `images\IconUnInstall.bmp`
    ;File `/ONAME=$PLUGINSDIR\PPercent.bmp` `images\PPercent.bmp`

    SkinBtn::Init "$PLUGINSDIR\Yes.bmp"
    SkinBtn::Init "$PLUGINSDIR\No.bmp"
FunctionEnd

;-------------��������ϰ�װ����������----------
Function un.CreateMutex
#��鰲װ���⣺#
ReCheck:
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t"${MyMutex_Install}") i.R1 ?e'
Pop $R0
  System::Call 'kernel32::CloseHandle(i R1) i.s'
#���ж�ػ��⣺#
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t"${MyMutex_UnInstall}") i.R3 ?e'
Pop $R2
  System::Call 'kernel32::CloseHandle(i R3) i.s'
#�жϰ�װ/ж�ػ���Ĵ���#
${If} $R0 != 0
  MessageBox MB_RetryCancel|MB_ICONEXCLAMATION "��װ�����Ѿ����У�" IdRetry ReCheck
Quit
${ElseIf} $R2 != 0
  MessageBox MB_RetryCancel|MB_ICONEXCLAMATION "ж�س����Ѿ����У�" IdRetry ReCheck
Quit
${Else}
#������װ���⣺#
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t"${MyMutex_Install}") i.R1 ?e'
Pop $R0
  StrCmp $R0 0 +2
Quit
${EndIf}
FunctionEnd

Function un.KillPPT
  FindProcDLL::FindProc "${PPT_NAME}.EXE"
  Pop $R0
  IntCmp $R0 1 kill_ppt ppt_no_run
  kill_ppt:
    KillProcDLL::KillProc "${PPT_NAME}.EXE"
    Sleep 500
  ppt_no_run:
FunctionEnd

Function un.KillUpdate
  FindProcDLL::FindProc "${UPDATE_NAME}.EXE"
  Pop $R0
  IntCmp $R0 1 kill_update update_no_run
  kill_update:
    KillProcDLL::KillProc "${UPDATE_NAME}.EXE"
    Sleep 500
  update_no_run:
FunctionEnd

Function un.KillCoursePlayer
  FindProcDLL::FindProc "${COURSE_NAME}.EXE"
  Pop $R0
  IntCmp $R0 1 kill_course course_no_run
  kill_course:
    KillProcDLL::KillProc "${COURSE_NAME}.EXE"
    Sleep 500
  course_no_run:
FunctionEnd

Function un.onGUIInit
    ;�����߿�
    System::Call `user32::SetWindowLong(i$HWNDPARENT,i${GWL_STYLE},0x9480084C)i.R0`
    ;����һЩ���пؼ�
    GetDlgItem $0 $HWNDPARENT 1034
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1035
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1036
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1037
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1038
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1039
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1256
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1028
    ShowWindow $0 ${SW_HIDE}

    ${NSW_SetWindowSize} $HWNDPARENT 602 428 ;�ı�Page��С   588 291
    ;${NSW_SetWindowSize} $HWNDPARENT $DialogWidth $DialogHight ;�ı�Page��С   588 291
    System::Call User32::GetDesktopWindow()i.R0
    ;Բ��
    System::Alloc 16
    System::Call user32::GetWindowRect(i$HWNDPARENT,isR0)
    System::Call *$R0(i.R1,i.R2,i.R3,i.R4)
    IntOp $R3 $R3 - $R1
    IntOp $R4 $R4 - $R2
    System::Call gdi32::CreateRoundRectRgn(i0,i0,iR3,iR4,i4,i4)i.r0
    System::Call user32::SetWindowRgn(i$HWNDPARENT,ir0,i1)
    System::Free $R0

FunctionEnd

;�����ޱ߿��ƶ�
Function un.onGUICallback
  ${If} $MSG = ${WM_LBUTTONDOWN}
    SendMessage $HWNDPARENT ${WM_NCLBUTTONDOWN} ${HTCAPTION} $0
  ${EndIf}
FunctionEnd
#----------------------------------------------
#ִ��ж������
#----------------------------------------------
Function UninstallSoft
  ReadRegStr $R0 HKLM \
  "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
  "UninstallString"
  ${GetParent} "$R0" $R1
  StrCmp $R0 "" done
  IfFileExists $R0 uninst
	Goto done
;����ж�س���
uninst:
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_TOPMOST "ϵͳ�Ѵ���${PRODUCT_NAME}���Ƿ�ж�أ�" IDYES +2
  Goto done
  ExecWait "$R0 /S _?=$R1" ;����$R0Ϊ��ȡ����ж�س������ƣ� /S�Ǿ�Ĭж�ز���ʹ��NSIS���ɵ�ж�س������Ҫ���� _? ���ܵȴ�ж�ء�$R1�����λ��
  IfFileExists "$R1" dir ;��� $R1���λ�� �����ļ�����ת�� DIR: ɾ������������ļ�
  Goto done
dir: ;����ļ��д���
	;Delete "$R1\*.*" ;��ɾ�����������ļ�,�����ʹ��
  StrCpy $INSTDIR $R1
  Delete "$INSTDIR\*.exe"
  Delete "$INSTDIR\*.dll"
  Delete "$INSTDIR\*.dat"

  RMDir /r "$INSTDIR\framework"
  RMDir /r "$INSTDIR\bin"
  RMDir /r "$INSTDIR\Microsoft.VC90.CRT"
  RMDir /r "$INSTDIR\Microsoft.VC90.MFC"
  RMDir /r "$INSTDIR\MainSkin"
  RMDir /r "$INSTDIR\temp"
  RMDir /r "$INSTDIR\package"
done:

FunctionEnd


/******************************
 *  �����ǰ�װ�����ж�ز���  *
 ******************************/

Section Uninstall
;Call un.KillAllApp
Sleep 100
SetDetailsPrint textonly
DetailPrint "����ж��..."
SetDetailsPrint none
  ;ReadRegStr $R0 HKLM \
  ;"Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" \
  ;"UninstallString"
  ;Messagebox MB_YESNO "$R0  + $INSTDIR"
  ;IfFileExists "$R0" uninstdone
  ;${GetParent} "$R0" $R1
  ;StrCpy $INSTDIR $R1
  ;SetOverwrite on
  SetShellVarContext all
  
  ;������ʼ�˵���ݷ�ʽ           �Ƚ�������ɾ����ݣ����ɾ��Ӧ�ã���Ȼ�ᵼ�½���ʧ��
  ExecShell "startunpin" "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"
  ;������������ݷ�ʽ
  ExecShell "taskbarunpin" "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"

  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}.lnk"
  Delete "$DESKTOP\${PRODUCT_WEB_LINK_NAME}.lnk"
  RMDir /r /REBOOTOK "$SMPROGRAMS\${PRODUCT_NAME}"
  
  Delete "$QUICKLAUNCH\User Pinned\TaskBar\${PRODUCT_NAME}.lnk"
  Delete "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"
  Delete "$STARTMENU\${PRODUCT_NAME}.lnk"
  
  Delete "$INSTDIR\*.exe"
  Delete "$INSTDIR\*.dll"
  Delete "$INSTDIR\*.dat"
  Delete "$INSTDIR\*.log"
  
  RMDir /r /REBOOTOK "$INSTDIR\bin"
  RMDir /r /REBOOTOK "$INSTDIR\Skins"
  RMDir /r /REBOOTOK "$INSTDIR\template"
  RMDir /r /REBOOTOK "$INSTDIR\Cache"
  RMDir /r /REBOOTOK "$INSTDIR\log"
  RMDir /r /REBOOTOK "$INSTDIR\temp"
  RMDir /r /REBOOTOK "$INSTDIR\package"
  
  Messagebox MB_YESNO "�Ƿ�ɾ�������ļ�?" IDYES true IDNO false
  true:
    RMDir /r /REBOOTOK "$INSTDIR\settting"
    RMDir /r /REBOOTOK "$INSTDIR"
  false:

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  
  ;�ָ�ע���
 #------------------ Win 8 -------------------------------------
  DeleteRegKey HKCR ".ppt\PPTShell"
  DeleteRegKey HKCR ".pptx\PPTShells"

  WriteRegStr HKCR ".ppt" "" "PowerPoint.Show.8"
  WriteRegStr HKCR ".ppt\PowerPoint.Show.8\ShellNew" "" ""
  
  WriteRegStr HKCR ".pptx" "" "PowerPoint.Show.12"
  WriteRegStr HKCR ".pptx\PowerPoint.Show.12\ShellNew" "" ""
  
 #------------------ ���� -------------------------------------
 
 DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.pptx\UserChoice"
 DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.ppt\UserChoice"
 
 DeleteRegKey HKCR "PPTShell\\Shell\\Open\\Command"
 DeleteRegKey HKCU "Software\\Classes\\Applications\\PPTShell\\shell\\open\\command"
 
 WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.pptx\UserChoice" "Progid" "PowerPoint.Show.12"
 WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.ppt\UserChoice" "Progid" "PowerPoint.Show.8"
  
uninstdone:
  Sleep 10
  SetAutoClose true
SectionEnd

Function un.KillAllApp
    KillProcDLL::KillProc "${PPT_EXE_NAME}"
    KillProcDLL::KillProc "${PPTSHELL_EXE_NAME}"
    KillProcDll::KillProc "${OLD_PPTSHELL_EXE_NAME}"
    KillProcDLL::KillProc "${UPDATE_EXE_NAME}"
    KillProcDLL::KillProc "${COURSE_EXE_NAME}"
    KillProcDLL::KillProc "${NODE_EXE_NAME}"
FunctionEnd

#-- ���� NSIS �ű��༭�������� Function ���α�������� Section ����֮���д���Ա��ⰲװ�������δ��Ԥ֪�����⡣--#

Function un.Page.5
   GetDlgItem $0 $HWNDPARENT 1
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 2
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 3
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1990
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1991
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1992
    ShowWindow $0 ${SW_HIDE}

    nsDialogs::Create 1044
    Pop $0
    ${If} $0 == error
        Abort
    ${EndIf}
    SetCtlColors $0 ""  transparent ;�������͸��

    ;${NSW_SetWindowSize} $0 $DialogWidth $DialogHight ;�ı�Page��С   588 291
    ${NSW_SetWindowSize} $0 602 428 ;�ı�Page��С   588 291

    ;�˳���ʾ����
    ${NSD_CreateLabel} 56 249 493U 20u "��ȷ��Ҫж��${PRODUCT_NAME}������"
    Pop $Lbl_AppName
    SetCtlColors $Lbl_AppName "FFFFFF" transparent ;�������͸��
    CreateFont $1 "΢���ź�" "16" "220"
    SendMessage $Lbl_AppName ${WM_SETFONT} $1 0
    ${NSD_AddStyle} $Lbl_AppName ${ES_CENTER}

    GetFunctionAddress $0 un.onGUICallback
    WndProc::onCallback $Lbl_AppName $0 ;�����ޱ߿����ƶ�

    ;��������
    ${NSD_CreateLabel} 5u 5u 115u 12u "${PRODUCT_NAME}" ; ж����"
    Pop $Lbl_Title
    ;SetCtlColors $Lbl_Title "" 0xFFFFFF ;��ɫ
    SetCtlColors $Lbl_Title "FFFFFF"  transparent ;�������͸��
    CreateFont $1 "΢���ź�" "11" "800"
    SendMessage $Lbl_Title ${WM_SETFONT} $1 0
    ;${NSD_AddStyle} $Lbl_Title ${ES_LEFT}

    GetFunctionAddress $0 un.onGUICallback
    WndProc::onCallback $Lbl_Title $0 ;�����ޱ߿����ƶ�

    ;��(ж��)��ť
    ${NSD_CreateButton} 470 379 105 38 ""
    Pop $Btn_Yes
    SkinBtn::Set /IMGID=$PLUGINSDIR\Yes.bmp $Btn_Yes
    SetCtlColors $Btn_Yes "808080"  transparent ;�������͸��
    CreateFont $1 "΢���ź�" "16" "500"
    SendMessage $Btn_Yes ${WM_SETFONT} $1 0
    ${NSD_AddStyle} $Btn_Yes ${ES_CENTER}

    GetFunctionAddress $3 un.OnNext
    SkinBtn::onClick $Btn_Yes $3

    ;��(�˳�)��ť
    ${NSD_CreateButton} 365 379 84 38 ""
    Pop $Btn_No
    SkinBtn::Set /IMGID=$PLUGINSDIR\No.bmp $Btn_No
    SetCtlColors $Btn_No "0d6190"  transparent ;�������͸��
    CreateFont $1 "΢���ź�" "16" "500" ;
    SendMessage $Btn_No ${WM_SETFONT} $1 0
    ${NSD_AddStyle} $Btn_No ${ES_CENTER}

    GetFunctionAddress $3 un.OnExit
    SkinBtn::onClick $Btn_No $3

    ;������ͼ��
    ${NSD_CreateBitmap} 227 72 146 146 ""
    Pop $BGImage
    ${NSD_SetImage} $BGImage $PLUGINSDIR\Icon.bmp $ImageHandle
    GetFunctionAddress $0 un.onGUICallback
    WndProc::onCallback $BGImage $0 ;�����ޱ߿����ƶ�
    
    ;��������ͼ
    ${NSD_CreateBitmap} 0 0 100% 100% ""
    Pop $BGImage
    ${NSD_SetImage} $BGImage $PLUGINSDIR\bg.bmp $ImageHandle

    GetFunctionAddress $0 un.onGUICallback
    WndProc::onCallback $BGImage $0 ;�����ޱ߿����ƶ�
    nsDialogs::Show
    ${NSD_FreeImage} $ImageHandle
FunctionEnd

Function un.InstFiles.PRO

FunctionEnd

;ж�ؽ���ҳ��
Function un.InstFiles.Show

    StrCpy $Bool_UnInstall 0
    FindWindow $R2 "#32770" "" $HWNDPARENT
    StrCpy $DefaultPage $R2
    GetDlgItem $0 $HWNDPARENT 1
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 2
    ShowWindow $0 ${SW_HIDE}
    EnableWindow $0 0       ;��ֹȡ����ť���ﵽ����ESC��Ч��
    GetDlgItem $0 $HWNDPARENT 3
    ShowWindow $0 ${SW_HIDE}

    GetDlgItem $1 $R2 1027
    ShowWindow $1 ${SW_HIDE}

    GetDlgItem $1 $R2 1990  ;��С��
    ShowWindow $1 ${SW_HIDE}
    GetDlgItem $1 $R2 1991 ;�ر�
    ShowWindow $1 ${SW_HIDE}
    EnableWindow $1 0
    GetDlgItem $1 $R2 1994  ;��װ
    ShowWindow $1 ${SW_HIDE}
    ;EnableWindow $1 0

    StrCpy $R0 $R2 ;�ı�ҳ���С,��Ȼ��ͼ����ȫҳ
    System::Call "user32::MoveWindow(i R0, i 0, i 0, i 601, i 427) i R2"    ;�޸Ĵ�С   588 291
    GetFunctionAddress $0 un.onGUICallback
    WndProc::onCallback $R0 $0 ;�����ޱ߿����ƶ�

    GetDlgItem $R0 $R2 1004  ;���ý�����λ��
    System::Call "user32::MoveWindow(i R0, i 15, i 400, i 562, i 6) i R2"
    Strcpy $PB_ProgressBar $R0
    SkinProgress::Set $PB_ProgressBar "$PLUGINSDIR\Progress.bmp" "$PLUGINSDIR\ProgressBar.bmp"

    ;��������
    GetDlgItem $R0 $R2 1993  ;
    System::Call "user32::MoveWindow(i R0, i 8, i 8, i 170, i 20) i R2"    ;ע�⣺����ϵͳAPI�������� R0 ~ R9, ����Ч��                   ;5u 5u 115u 12u
    StrCpy $Lbl_Title $R0
    SetCtlColors $Lbl_Title "FFFFFF"  transparent ;�������͸��
    ${NSD_SetText} $Lbl_Title "${PRODUCT_NAME}" ; ж����"
    CreateFont $1 "΢���ź�" "11" "800"
    SendMessage $Lbl_Title ${WM_SETFONT} $1 0

    ;Ӧ����
    ;${NSD_CreateLabel} 270 70 300 60 "${PRODUCT_NAME}"
    GetDlgItem $R0 $R2 1997  ;����Ӧ����λ��
    System::Call "user32::MoveWindow(i R0, i 130, i 95, i 360, i 60) i R2"
    StrCpy $Lbl_AppName $R0
    SetCtlColors $Lbl_AppName "FFFFFF" transparent ;�������͸��
    ${NSD_SetText} $Lbl_AppName "��л��ʹ��${PRODUCT_NAME}!"
    CreateFont $1 "΢���ź�" "24" "300"
    SendMessage $Lbl_AppName ${WM_SETFONT} $1 0

    ;DetailPrint ���� -- "����ж��..." ��ǰ��Section�������
    GetDlgItem $R1 $R2 1006  ;��ȡ1006�ؼ�������ɫ���ı�λ��
    System::Call "user32::MoveWindow(i R1, i 15, i 376, i 500, i 40) i R2"
    SetCtlColors $R1 "888888"  transparent ;�������F6F6F6,ע����ɫ������Ϊ͸���������ص�
    CreateFont $1 "΢���ź�" "10" "50"
    SendMessage $R1 ${WM_SETFONT} $1 0

    ;��������
    GetDlgItem $R0 $R2 1992  ;��ȡ1992�ؼ�������ɫ���ı�λ��   [�ð�ť����ʾ]      1997 ���ı�
    ShowWindow $R0 ${SW_HIDE}
    ;��������
    ;GetDlgItem $R0 $R2 1997  ;��ȡ1992�ؼ�������ɫ���ı�λ��   [�ð�ť����ʾ]      1997 ���ı�
    ;ShowWindow $R0 ${SW_HIDE}
    System::Call "user32::MoveWindow(i R0, i 90, i 376, i 40, i 20, i 1) i R2"
    SetCtlColors $R0 "888888" "FFFFFF" ;�������F6F6F6,ע����ɫ������Ϊ͸���������ص�
    CreateFont $1 "΢���ź�" "10" "50"
    SendMessage $R0 ${WM_SETFONT} $1 0
    Strcpy $PPercent $R0
    ${NSD_SetText} $PPercent "0%"

    ;Сͼ��
    GetDlgItem $R0 $R2 1996
    System::Call "user32::MoveWindow(i R0, i 227, i 155, i 146, i 146) i R2"
    ${NSD_SetImage} $R0 $PLUGINSDIR\IconUnInstall.bmp $IconHandle

    ;��������ͼ
    GetDlgItem $R0 $R2 1995
    System::Call "user32::MoveWindow(i R0, i 0, i 0, i 601, i 427) i R2"    ;�޸Ĵ�С   588 291
    ${NSD_SetImage} $R0 $PLUGINSDIR\bg.bmp $ImageHandle

    #����WndSubclass����趨timer������ַ�����Թ�ֱ��Ϊsettimer�趨������ַ����û�в��֧�ֺ����ѣ�
    ;${WndSubclass_Subclass} $HWNDPARENT un.GetProgressPos $SubProgress $SubProgress
    ;System::Call user32::SetTimer(i $DefaultPage, i 10, i 50, i 0)
    ;GetFunctionAddress $2 un.GetProgressPos
    ;ThreadTimer::Start 200 0 $2
    
FunctionEnd

Function un.GetProgressPos                       ;GetProgressPos
  SendMessage $PB_ProgressBar 0x0407 0 0 $1
  SendMessage $PB_ProgressBar 0x0408 0 0 $2
  ${If} $2 > 0
  ${If} $1 > 0
    IntOp $2 $2 * 100
    IntOp $1 $2 / $1
    ${NSD_SetText} $PPercent "$1%"
    StrCpy $Bool_UnInstall 1
    ${If} $1 >= 99
      ThreadTimer::Stop
      ;System::Call user32::KillTimer(i $DefaultPage, i 10)
      ${NSD_SetText} $PPercent "100%"
      ;System::Call user32::InvalidateRect(i$DefaultPage, i 0, i 1)
    ${Endif}
  ;${ElseIf} $Bool_UnInstall = 1
  ;    System::Call user32::KillTimer(i $DefaultPage, i 10)
  ${Endif}
  ${Endif}
FunctionEnd


Function un.InstFiles.Leave
  ${IfThen} ${Abort} ${|} Quit ${|}
FunctionEnd

Function un.Page.6
    GetDlgItem $0 $HWNDPARENT 1
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 2
    ShowWindow $0 ${SW_HIDE}
    EnableWindow $0 0       ;��ֹȡ����ť���ﵽ����ESC��Ч��
    GetDlgItem $0 $HWNDPARENT 3
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1990
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1991
    ShowWindow $0 ${SW_HIDE}
    GetDlgItem $0 $HWNDPARENT 1992
    ShowWindow $0 ${SW_HIDE}

    ;���õ�ǰҳ��λ�����˳�ҳ��������
    StrCpy $CurrentPage 2

    nsDialogs::Create 1044
    Pop $0
    ${If} $0 == error
        Abort
    ${EndIf}
    SetCtlColors $0 ""  transparent ;�������͸��
    ${NSW_SetWindowSize} $0 601 427 ;�ı�Page��С   588 291
    ;${NSW_SetWindowSize} $0 $DialogWidth $DialogHight ;�ı�Page��С   588 291

    ;��ɰ�ť  ${NSD_CreateButton} 232 158 122 47 ""
    ${NSD_CreateButton} 470 379 105 38 ""
    Pop $Btn_Done
    SkinBtn::Set /IMGID=$PLUGINSDIR\Yes.bmp $Btn_Done

    GetFunctionAddress $3 un.OnExit
    SkinBtn::onClick $Btn_Done $3
    ${NSD_SetFocus} $Btn_Done

    ;ж�����
    ${NSD_CreateLabel} 195 205 300 60 "ж�����"
    Pop $Lbl_AppName
    SetCtlColors $Lbl_AppName "FFFFFF" transparent ;�������͸��
    CreateFont $1 "΢���ź�" "38" "400"
    SendMessage $Lbl_AppName ${WM_SETFONT} $1 0
    ;;${NSD_AddStyle} $Lbl_AppName ${ES_CENTER}
    GetFunctionAddress $0 un.onGUICallback
    WndProc::onCallback $Lbl_AppName $0 ;�����ޱ߿����ƶ�

    ;��������
    ${NSD_CreateLabel} 5u 5u 115u 12u "${PRODUCT_NAME}" ; ж����"
    Pop $Lbl_Title
    ;SetCtlColors $Lbl_Title "" 0xFFFFFF ;��ɫ
    SetCtlColors $Lbl_Title "FFFFFF"  transparent ;�������͸��
    CreateFont $1 "΢���ź�" "11" "800"
    SendMessage $Lbl_Title ${WM_SETFONT} $1 0
    ;${NSD_AddStyle} $Lbl_Title ${ES_CENTER}
    GetFunctionAddress $0 un.onGUICallback
    WndProc::onCallback $Lbl_Title $0 ;�����ޱ߿����ƶ�


    ;�رհ�ť
    ;${NSD_CreateButton} 574 0 27 27 ""
    ;Pop $0
    ;SkinBtn::Set /IMGID=$PLUGINSDIR\btnClose.bmp $0
    ;GetFunctionAddress $3 onClickClose
    ;SkinBtn::onClick $0 $3

    ;������ͼ��
    ${NSD_CreateBitmap} 254 90 93 96 ""
    Pop $BGImage
    ${NSD_SetImage} $BGImage $PLUGINSDIR\IconDone.bmp $ImageHandle

    GetFunctionAddress $0 un.onGUICallback
    WndProc::onCallback $BGImage $0 ;�����ޱ߿����ƶ�

    ;��������ͼ
    ${NSD_CreateBitmap} 0 0 100% 100% ""
    Pop $BGImage
    ${NSD_SetImage} $BGImage $PLUGINSDIR\bg.bmp $ImageHandle

    GetFunctionAddress $0 un.onGUICallback
    WndProc::onCallback $BGImage $0 ;�����ޱ߿����ƶ�
    nsDialogs::Show
    ${NSD_FreeImage} $ImageHandle

FunctionEnd


Function un.Page.7

FunctionEnd

#------------------------------------------
#��С������
#------------------------------------------
Function un.OnMini
System::Call user32::CloseWindow(i$HWNDPARENT) ;��С��
FunctionEnd

#------------------------------------------
#ж�����ҳʹ�ö������η�����������ĳ����ҳ
#------------------------------------------

;����ҳ����ת������
Function un.RelGotoPage
  IntCmp $R9 0 0 Move Move
    StrCmp $R9 "X" 0 Move
      StrCpy $R9 "120"
  Move:
  SendMessage $HWNDPARENT "0x408" "$R9" ""
FunctionEnd

Function un.OnNext
  StrCpy $R9 1 ;Goto the next page
  Call un.RelGotoPage
  Abort
FunctionEnd

#------------------------------------------
#�رմ���
#------------------------------------------
Function un.OnExit
RMDir /r /REBOOTOK "$PLUGINSDIR"
System::Call kernel32::ExitProcess(0) ;�ر�
FunctionEnd
