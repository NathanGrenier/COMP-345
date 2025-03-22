@ECHO OFF
setlocal enabledelayedexpansion

:parse
if "%1"=="" goto endparse
if /i "%1"=="/skipstart" set SKIPSTART=1
if /i "%1"=="/verbose" set VERBOSE=1
if /i "%1"=="/clean-pb" set CLEAN_POST_BUILD=1
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

:: Conditional cleanup based on /clean-pb flag
if defined CLEAN_POST_BUILD (
  if defined VERBOSE echo Performing cleanup...
  
  :: Directories to delete
  set "DIRS_TO_DELETE=CMakeFiles examples src vendor"
  :: Files to delete
  set "FILES_TO_DELETE=.ninja_deps .ninja_log build.ninja cmake_install.cmake CMakeCache.txt CPackConfig.cmake CPackSourceConfig.cmake"
  
  :: Delete directories
  for %%d in (!DIRS_TO_DELETE!) do (
    if exist "%%d" (
      if defined VERBOSE echo Deleting directory: %%d
      rmdir /s /q "%%d" 2>nul
      if errorlevel 1 echo Warning: Failed to delete directory %%d
    )
  )
  
  :: Delete files
  for %%f in (!FILES_TO_DELETE!) do (
    if exist "%%f" (
      if defined VERBOSE echo Deleting file: %%f
      del /q "%%f" 2>nul
      if errorlevel 1 echo Warning: Failed to delete file %%f
    )
  )
)

:: Start the Executable unless /skipstart is passed
if not defined SKIPSTART (
    echo Starting COMP-345.exe...
    start /b COMP-345.exe
) else (
    echo Skipping start due to /skipstart flag.
)

echo Release Build Installed Successfully!
exit /b 0