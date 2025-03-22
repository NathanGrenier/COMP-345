@ECHO OFF

:: Delete build directory if it exists (silently)
rmdir /s /q build 2>nul

:: Create and enter build directory
mkdir build & cd build

:: Configure and build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-Wl" -DBUILD_SHARED_LIBS=OFF ..
cmake --build . --config Release

:: Start the Executable
start /b COMP-345.exe
echo "Release Build Installed Successfully!"