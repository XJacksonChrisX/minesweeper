@echo off
REM Minesweeper Game - Windows Build & Run Script
REM 踩地雷遊戲 - Windows 編譯運行腳本

echo ========================================
echo   踩地雷遊戲 - Minesweeper
echo ========================================
echo.

REM Check if g++ is installed
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ❌ 錯誤：找不到 g++ 編譯器
    echo.
    echo 請安裝 MinGW-w64 編譯器：
    echo https://www.mingw-w64.org/
    echo.
    echo 或使用 Chocolatey 安裝：
    echo choco install mingw-w64
    echo.
    pause
    exit /b 1
)

REM Clean old build artifacts
echo 🧹 清理舊產物...
if exist src\*.o del src\*.o
if exist minesweeper.exe del minesweeper.exe

REM Compile
echo 📦 編譯中...
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/main.cpp -o src/main.o
if %ERRORLEVEL% NEQ 0 goto error
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/cell.cpp -o src/cell.o
if %ERRORLEVEL% NEQ 0 goto error
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/board.cpp -o src/board.o
if %ERRORLEVEL% NEQ 0 goto error
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/game.cpp -o src/game.o
if %ERRORLEVEL% NEQ 0 goto error
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/display.cpp -o src/display.o
if %ERRORLEVEL% NEQ 0 goto error

echo 🔗 連結中...
g++ src/main.o src/cell.o src/board.o src/game.o src/display.o -o minesweeper.exe
if %ERRORLEVEL% NEQ 0 goto error

REM Run
echo.
echo ✅ 編譯成功！開始遊戲...
echo.
minesweeper.exe
goto end

:error
echo.
echo ❌ 編譯失敗！請檢查程式碼。
pause
exit /b 1

:end
echo.
pause
