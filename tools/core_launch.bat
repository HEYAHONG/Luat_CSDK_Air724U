@rem Copyright (C) 2018 RDA Technologies Limited and\or its affiliates("RDA").
@rem All rights reserved.
@rem 
@rem This software is supplied "AS IS" without any warranties.
@rem RDA assumes no responsibility or liability for the use of the software,
@rem conveys no license or title under any patent, copyright, or mask work
@rem right to the product. RDA reserves the right to make changes in the
@rem software without notification.  RDA also make no representation or
@rem warranty that such application will be suitable for the specified use
@rem without further testing or modification.

@echo off

set CORE_LAUNCH_PATH=%~dp0

set PROJECT_ROOT=%CORE_LAUNCH_PATH%\..

set BUILD_TARGET=%1

if "%BUILD_RELEASE_TYPE%A" == "A"  set BUILD_RELEASE_TYPE=debug


call :add_path %PROJECT_ROOT%\prebuilts\win32\bin
call :add_path %PROJECT_ROOT%\prebuilts\win32\cmake\bin
call :add_path %PROJECT_ROOT%\prebuilts\win32\python3
call :add_path %PROJECT_ROOT%\prebuilts\win32\python3\Scripts
call :add_path %PROJECT_ROOT%\prebuilts\win32\gcc-arm-none-eabi\bin
call :add_path %PROJECT_ROOT%\tools
call :add_path %PROJECT_ROOT%\tools\win32


@rem 兼容原csdk的写法
if not exist "%BUILD_TARGET%"  call :build_demo

exit /B 0

:build_demo
set BUILD_DEMO=%BUILD_TARGET%
set BUILD_TARGET=demo\%BUILD_TARGET%
set PROJECT_OUT=%PROJECT_ROOT%\out\%BUILD_DEMO%_%BUILD_RELEASE_TYPE% 
if not exist %PROJECT_OUT% mkdir %PROJECT_OUT% 
goto :eof



:add_path
(echo ";%PATH%;" | find /C /I ";%1;" > nul) || set "PATH=%1;%PATH%"
goto :eof
