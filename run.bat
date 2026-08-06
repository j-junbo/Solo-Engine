@echo off
REM Configures the project and generates the Visual Studio 2022 solution
REM into the Engine/Build/Engine folder.
 
cmake -B Engine/Build/Engine -G "Visual Studio 17 2022" -A x64
 
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo CMake configuration failed.
    pause
    exit /b %ERRORLEVEL%
)
 
echo.
echo Done. Solution generated in Engine/Build/Engine
pause