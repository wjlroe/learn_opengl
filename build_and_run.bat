@echo off

call build.bat
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

pushd build
main.exe
popd