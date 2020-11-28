@echo OFF
ninja -f ./build/Debug/buildx64.ninja -v
clang -g %WLIBPATH64% %WLIBVALUES% -l Xinput.lib ./bin/Debug/x64/obj/*.o -o ./bin/Debug/x64/Tetris24hr.exe