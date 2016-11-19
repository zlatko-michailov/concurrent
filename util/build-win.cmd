@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
pushd %~dp0..\test
cd
nmake all
popd
