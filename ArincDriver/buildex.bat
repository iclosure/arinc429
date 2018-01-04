@echo off

::echo buildex.bat: build bat file

set BuildEvent=%1
set ProjectDir=%2
set SourceDir=%3
set TargetName=%4
set ConfigType=%5
set SystemName=%6
set PlatformName=%7
set ConfigTypeFolder=%8
set TargetFolder=%9

if "%BuildEvent%" == "-pre" (

	echo off
	
) else if "%BuildEvent%" == "-post" (

	if exist "%ProjectDir%\..\%ConfigTypeFolder%\crdarincprop.dll" (
		copy "%ProjectDir%\..\%ConfigTypeFolder%\crdarincprop.dll" "%TargetFolder%"
	)
	
) else (

	echo not supported BuildEvent ( '%BuildEvent%' )
	goto exit
	
)

goto exit

:exit
exit /b 0
