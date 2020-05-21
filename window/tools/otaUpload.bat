@echo off

set PYTHON_PATH=%LOCALAPPDATA%\Arduino15\packages\esp8266\tools\python3\3.7.2-post1
set PYTHON=%PYTHON_PATH%\python3.exe

echo Updating led-window over the air...
echo Usage: otaUpload.bat [IP ADDRESS] [BOARD PASSWORD]
echo If this fails to upload, make sure python is permitted through the Windows firewall!

cd %~dp0

if not exist espota.py (
    curl https://raw.githubusercontent.com/esp8266/Arduino/master/tools/espota.py -o espota.py
)

%PYTHON% espota.py -i %1 -f ..\..\build\window.ino.bin -a %2

echo Completed.