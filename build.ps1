# Minesweeper Game - Windows Build and Run Script
# Simply run this file to build and play!

Write-Host "================================" -ForegroundColor Green
Write-Host "  Minesweeper Game - Build & Play" -ForegroundColor Green
Write-Host "================================" -ForegroundColor Green
Write-Host ""

# Check if g++ is installed
try {
    $gppVersion = g++ --version 2>$null | Select-Object -First 1
    if ($gppVersion) {
        Write-Host "Found g++ compiler: $gppVersion" -ForegroundColor Green
    }
} catch {
    Write-Host "Error: g++ compiler not found!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please install MinGW-w64:" -ForegroundColor Yellow
    Write-Host "1. Visit: https://www.mingw-w64.org/downloads/" -ForegroundColor Yellow
    Write-Host "2. Download and run the installer" -ForegroundColor Yellow
    Write-Host "3. Choose x86_64 architecture" -ForegroundColor Yellow
    Write-Host "4. Run this script again" -ForegroundColor Yellow
    Write-Host ""
    pause
    exit 1
}

# Clean old files
Write-Host "Cleaning old files..." -ForegroundColor Cyan
Remove-Item -Path "minesweeper.exe" -ErrorAction SilentlyContinue | Out-Null
Remove-Item -Path "src\*.o" -ErrorAction SilentlyContinue | Out-Null

# Compile
Write-Host "Compiling..." -ForegroundColor Cyan
Write-Host ""

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/main.cpp -o src/main.o
if ($LASTEXITCODE -ne 0) { Write-Host "Compilation failed!"; exit 1 }

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/cell.cpp -o src/cell.o
if ($LASTEXITCODE -ne 0) { Write-Host "Compilation failed!"; exit 1 }

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/board.cpp -o src/board.o
if ($LASTEXITCODE -ne 0) { Write-Host "Compilation failed!"; exit 1 }

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/game.cpp -o src/game.o
if ($LASTEXITCODE -ne 0) { Write-Host "Compilation failed!"; exit 1 }

g++ -std=c++17 -Wall -Wextra -Iinclude -c src/display.cpp -o src/display.o
if ($LASTEXITCODE -ne 0) { Write-Host "Compilation failed!"; exit 1 }

g++ src/main.o src/cell.o src/board.o src/game.o src/display.o -o minesweeper.exe
if ($LASTEXITCODE -ne 0) { Write-Host "Linking failed!"; exit 1 }

Write-Host ""
Write-Host "Build successful!" -ForegroundColor Green
Write-Host "Starting game..." -ForegroundColor Green
Write-Host ""

# Run the game
.\minesweeper.exe
