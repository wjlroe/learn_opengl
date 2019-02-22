@echo off

set start=%time%
set BUILD_PATH=%~dp0

SET /A errno=0

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set GLFW_INCLUDE_PATH="C:\glfw-3.2.1.bin.WIN64\include"
set GLFW_LIB_PATH="C:\glfw-3.2.1.bin.WIN64\lib-vc2015"
set ASSIMP_INCLUDE_PATH="C:\dev\assimp-4.1.0\include"
set ASSIMP_LIB_PATH="C:\dev\assimp-4.1.0\lib\Debug"

set build_message=BUILD_PATH is %BUILD_PATH%
echo %build_message%
cd %BUILD_PATH%
IF NOT EXIST build mkdir build
pushd build
IF NOT EXIST glfw3.dll copy %GLFW_LIB_PATH%\glfw3.dll .
cl /std:c++17 -FC -Zc:strictStrings -Zi -EHsc ..\src\main.cpp -I ..\include^
 -I %GLFW_INCLUDE_PATH% -I %ASSIMP_INCLUDE_PATH%^
 /link %GLFW_LIB_PATH%\glfw3.lib %GLFW_LIB_PATH%\glfw3dll.lib opengl32.lib^
 glu32.lib %ASSIMP_LIB_PATH%\assimp-vc140-mt.lib /SUBSYSTEM:CONSOLE
IF %ERRORLEVEL% NEQ 0 SET /A errno=%ERRORLEVEL%
popd

set end=%time%

set options="tokens=1-4 delims=:.,"
for /f %options% %%a in ("%start%") do set start_h=%%a&set /a start_m=100%%b %% 100&set /a start_s=100%%c %% 100&set /a start_ms=100%%d %% 100
for /f %options% %%a in ("%end%") do set end_h=%%a&set /a end_m=100%%b %% 100&set /a end_s=100%%c %% 100&set /a end_ms=100%%d %% 100

set /a hours=%end_h%-%start_h%
set /a mins=%end_m%-%start_m%
set /a secs=%end_s%-%start_s%
set /a ms=%end_ms%-%start_ms%
if %ms% lss 0 set /a secs = %secs% - 1 & set /a ms = 100%ms%
if %secs% lss 0 set /a mins = %mins% - 1 & set /a secs = 60%secs%
if %mins% lss 0 set /a hours = %hours% - 1 & set /a mins = 60%mins%
if %hours% lss 0 set /a hours = 24%hours%
if 1%ms% lss 100 set ms=0%ms%

:: Mission accomplished
set /a totalsecs = %hours%*3600 + %mins%*60 + %secs%
echo compile took (%totalsecs%.%ms%s total)

EXIT /B %errno%
