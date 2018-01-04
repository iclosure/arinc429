
@echo off

title ������������ļ�

echo.
echo clean.bat: Deleting output
echo.

rem ****************************************
rem   variables
rem ****************************************

% ��ĿĿ¼ %
SET BASE_PATH=%CD%

% �����ļ����� %
SET PROJ_NAMES=^
., ^
ArincApi, ^
WtApi, ^
ArincDriver, ^
ArincDriverProp, ^
ArincTest, ^
ArincTool, ^
Setup, ^
Setup/Arinc429

% Ŀ���ļ����� %
SET TARGET_FOLDER_NAMES=^
ipch, ^
debug, ^
release, ^
Unicode_Debug, ^
Unicode_Release, ^
objchk_wxp_x86, ^
objchk_win7_x86, ^
objfre_wxp_x86, ^
objfre_win7_x86, ^
obj, ^
PROJECT_ASSISTANT

% Ŀ���ļ���׺�� %
SET TARGET_FILE_SUFFIXS=^
ncb, ^
suo,^
sdf, ^
opensdf, ^
user, ^
log, ^
wrn, ^
err

rem ****************************************
rem   Delete working folders
rem ****************************************

echo.
echo clean.bat: Deleting working folders
echo.

for %%I in ( %PROJ_NAMES% ) do (
	if exist %BASE_PATH%\%%I (
		cd %BASE_PATH%\%%I
		for %%J in ( %TARGET_FOLDER_NAMES% ) do (
			if exist %%J (
				echo Delete folder: %cd%\%%J 
				rd /q /s %%J
			)
		)
	)
)

rem ****************************************
rem   Delete files
rem ****************************************

echo.
echo clean.bat: Deleting files
echo.

for %%I in ( %PROJ_NAMES% ) do (
	if exist %BASE_PATH%\%%I (
		cd %BASE_PATH%\%%I
		for %%J in ( %TARGET_FILE_SUFFIXS% ) do ( 
			for %%K in ( *.%%J ) do (
				if exist %%K (
					echo Delete file: %cd%\%%K
					del /a /q /s /f %%K
				)
			)
		)
	)
)

goto exit

:pause
pause

:exit
exit /b 0