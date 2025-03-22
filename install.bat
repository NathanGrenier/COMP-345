@ECHO OFF
:parse
if "%1"=="" goto endparse
if "%1"=="/skipstart" set SKIPSTART=1
if "%1"=="/verbose" set VERBOSE=1
shift
goto parse
:endparse

:: Delete build directory if it exists (silently)
rmdir /s /q build 2>nul

:: Create and enter build directory
mkdir build & cd build

:: Configure and build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-Wl" ..
if defined VERBOSE (
    cmake --build . --config Release --verbose
) else (
    cmake --build . --config Release
)

:: Start the Executable unless /skipstart is passed
if not defined SKIPSTART (
    echo Starting COMP-345.exe...
    start /b COMP-345.exe
) else (
    echo Skipping start due to /skipstart flag.
)

echo Release Build Installed Successfully!