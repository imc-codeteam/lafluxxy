@ECHO OFF

rmdir /S bin

REM *************
REM Release build
REM *************
@ECHO ******************
@ECHO Deploying LaFluxxy
@ECHO ******************

REM Create directories
mkdir bin

REM Copy files LaFluxxy
xcopy build-lafluxxy-Desktop_Qt_5_10_1_MSVC2017_64bit-Release\release\lafluxxy.exe bin
xcopy lafluxxy_logo.ico bin
if exist C:\Qt\5.10.1\msvc2017_64\bin\windeployqt.exe (
    C:\Qt\5.10.1\msvc2017_64\bin\windeployqt.exe bin\lafluxxy.exe --release --force
)
if exist D:\Qt\5.10.1\msvc2017_64\bin\windeployqt.exe (
    D:\Qt\5.10.1\msvc2017_64\bin\windeployqt.exe bin\lafluxxy.exe --release --force
)