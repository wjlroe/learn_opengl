@echo off

call build.bat
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

pushd build
main.exe
echo "Game exited with %ERRORLEVEL%"
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
popd
