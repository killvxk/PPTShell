@color B
@echo off
@echo ������ - �����ļ��Զ����������� - LSJ
@pause
@cd VerRoot
@echo �������ԱȰ汾���£� 
@dir /b
@set /p newVer=�������°汾��(���ֺ͵����,��ʽ a.b.c.d):
md %newVer%
@cd %newVer%
@set /p tips=��ʼ����SkinsĿ¼���س�������
xcopy ..\..\..\PPTShell\PPTShell\Skins .\Skins\ /E /Y /D
@set /p tips=��ʼ����binĿ¼���س�������
xcopy ..\..\..\PPTShell\PPTShell\bin .\bin\ /E /Y /D /exclude:..\..\exclude.txt
xcopy ..\..\..\PPTShell\PPTShell\ReleaseCopy\updatelib\������ .\bin\CoursePlayer\ /E /Y /D
@set /p tips=��ʼ���Ƴ����ļ����س�������
copy ..\..\..\bin\Release������\msftedit.dll .\ 
copy ..\..\..\bin\Release������\Update.exe .\
copy ..\..\..\bin\Release������\DuiLib.dll .\ 
copy ..\..\..\bin\Release������\101PPT.exe .\
>version.dat set /p=%newVer%<nul
@echo ��ϲ���汾�ļ����Ƴɹ���
pause