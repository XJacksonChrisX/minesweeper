@echo off
REM 踩地雷遊戲 - Windows 編譯腳本

echo.
echo ========================================
echo  踩地雷遊戲 - 編譯並執行
echo ========================================
echo.

REM 檢查編譯器
where g++ >nul 2>nul
if %errorlevel% neq 0 (
    echo 錯誤：找不到 g++ 編譯器
    echo 請先安裝 MinGW-w64 或 Dev C++
    echo 下載：https://www.mingw-w64.org/
    pause
    exit /b 1
)

REM 清理舊檔案
echo 清理舊檔案...
if exist minesweeper.exe del minesweeper.exe
del src\*.o 2>nul

REM 編譯
echo.
echo 編譯中...
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/main.cpp -o src/main.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/cell.cpp -o src/cell.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/board.cpp -o src/board.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/game.cpp -o src/game.o
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/display.cpp -o src/display.o
g++ src/main.o src/cell.o src/board.o src/game.o src/display.o -o minesweeper.exe

if %errorlevel% neq 0 (
    echo.
    echo 編譯失敗！
    pause
    exit /b 1
)

REM 執行
echo.
echo 編譯成功！開始遊戲...
echo.
minesweeper.exe

pause
