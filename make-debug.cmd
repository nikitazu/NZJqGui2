@echo off
echo "[MAKE] [DEBUG] WIN32"

cmake --build ./build/ --config Debug -j 8
XCOPY ./bin/jq-windows-amd64.exe ./build/Debug/bin/ /Y
