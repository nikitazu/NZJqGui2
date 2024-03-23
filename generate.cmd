@echo off
echo "[GENERATE] WIN32"

cmake -S ./src/ -B ./build/ -DCMAKE_INSTALL_PREFIX=./lib/nappgui
