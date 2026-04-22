@echo off
REM Minesweeper Game - Windows Build Script

echo.
echo Compiling Minesweeper Game...
echo.

REM Check if g++ exists
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo Error: g++ not found
    echo Please install MinGW-w64: https://www.mingw-w64.org/
    pause
    exit /b 1
)

REM Clean old files
del /q minesweeper.exe 2>nul
del /q src\*.o 2>nul

REM Compile
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/main.cpp -o src/main.o
if %errorlevel% neq 0 goto :error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/cell.cpp -o src/cell.o
if %errorlevel% neq 0 goto :error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/board.cpp -o src/board.o
if %errorlevel% neq 0 goto :error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/game.cpp -o src/game.o
if %errorlevel% neq 0 goto :error

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/display.cpp -o src/display.o
if %errorlevel% neq 0 goto :error

g++ src/main.o src/cell.o src/board.o src/game.o src/display.o -o minesweeper.exe
if %errorlevel% neq 0 goto :error

echo.
echo Build successful! Starting game...
echo.
minesweeper.exe
goto :end

:error
echo.
echo Compilation failed!
pause
exit /b 1

:end
pause
