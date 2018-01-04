
@echo off

title 清除工程垃圾文件

echo.
echo clean.bat: Deleting output
echo.

rem ****************************************
rem   variables
rem ****************************************

% 项目目录 %
SET BASE_PATH=%CD%

% 工程文件夹名 %
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

% 目标文件夹名 %
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

% 目标文件后缀名 %
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