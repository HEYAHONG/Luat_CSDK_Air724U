@echo off
set CSDK_VER=1.0.0
set CSDK_PRO=wifiloc
set FOTA_FLAG=%1
set FOTA_TYPE=%2
set AM_MODEL=iot_SDK_720U
call ..\tools\core_launch.bat wifiloc

REM/*+\NEW\chenzhimin\2020.07.22\*/
cd %PROJECT_OUT%
cmake ..\.. -G Ninja ^
-D CONFIG_BUILD_APP_HTTP=ON
ninja
cd ..\..\project
REM/*-\NEW\chenzhimin\2020.07.22\*/