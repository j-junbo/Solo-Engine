@echo off
echo Cleaning Build and Solutions folders...

if exist "Engine\Build" (
    rmdir /s /q "Engine\Build"
    echo Removed Engine\Build
) else (
    echo Engine\Build not found, skipping.
)

if exist "Engine\Solutions" (
    rmdir /s /q "Engine\Solutions"
    echo Removed Engine\Solutions
) else (
    echo Engine\Solutions not found, skipping.
)

echo Done.
pause