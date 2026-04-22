#!/bin/bash
# Minesweeper Game - Unix/Linux/Mac Build & Run Script
# 踩地雷遊戲 - Unix/Linux/Mac 編譯運行腳本

echo "========================================"
echo "  踩地雷遊戲 - Minesweeper"
echo "========================================"
echo ""

# Check if g++ is installed
if ! command -v g++ &> /dev/null; then
    echo "❌ 錯誤：找不到 g++ 編譯器"
    echo ""
    echo "請安裝 g++ 編譯器："
    echo ""
    echo "Ubuntu/Debian:"
    echo "  sudo apt-get install g++"
    echo ""
    echo "macOS (使用 Homebrew):"
    echo "  brew install gcc"
    echo ""
    exit 1
fi

# Clean old build artifacts
echo "🧹 清理舊產物..."
rm -f src/*.o minesweeper

# Compile
echo "📦 編譯中..."
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/main.cpp -o src/main.o || exit 1
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/cell.cpp -o src/cell.o || exit 1
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/board.cpp -o src/board.o || exit 1
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/game.cpp -o src/game.o || exit 1
g++ -std=c++17 -Wall -Wextra -Iinclude -c src/display.cpp -o src/display.o || exit 1

# Link
echo "🔗 連結中..."
g++ src/main.o src/cell.o src/board.o src/game.o src/display.o -o minesweeper || exit 1

# Run
echo ""
echo "✅ 編譯成功！開始遊戲..."
echo ""
./minesweeper
