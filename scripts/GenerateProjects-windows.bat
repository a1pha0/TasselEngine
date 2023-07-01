@echo off
pushd ..\
call thirdparty\premake5\Windows\premake5.exe vs2022
popd
pause
