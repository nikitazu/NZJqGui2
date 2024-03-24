@echo off
echo "[MAKE] [RELEASE] WIN32"

cmake --build ./build/ --config Release -j 8
XCOPY ./bin/jq-windows-amd64.exe ./build/Release/bin/ /Y
