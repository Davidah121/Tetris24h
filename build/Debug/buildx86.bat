@echo OFF
ninja -f ./build/Debug/buildx86.ninja -v
"C:\Program Files (x86)\LLVM\bin\clang" -g -fsanitize=address %WLIBPATH32% %WLIBVALUES% -l Xinput.lib ./bin/Debug/x86/obj/*.o -o ./bin/Debug/x86/output.exe