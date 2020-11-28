@echo OFF
ninja -f ./build/Release/buildx64.ninja -v
clang -O3 %WLIBPATH64% %WLIBVALUES% -l Xinput.lib ./bin/Release/x64/obj/*.o -o ./bin/Release/x64/Tetris24hr.exe