echo Generating vs2026 project files...

where cmake >target 2>nul
if %ERRORLEVEL% neq 0 (
    echo HIBA: A CMake nincs telepitve, vagy nincs hozzaadva a PATH-hoz.
    pause
    exit /b 1
)
cmake -G "Visual Studio 18 2026" -A x64
echo Done
PAUSE
