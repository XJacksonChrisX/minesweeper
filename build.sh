#!/bin/bash
# 踩地雷遊戲 - Linux/Mac 編譯腳本

echo ""
echo "========================================"
echo "  踩地雷遊戲 - 編譯並執行"
echo "========================================"
echo ""

# 檢查編譯器
if ! command -v g++ &> /dev/null; then
    echo "錯誤：找不到 g++ 編譯器"
    echo "請先安裝 g++ 和 make"
    echo "Ubuntu/Debian: sudo apt-get install build-essential"
    echo "macOS: brew install gcc"
    exit 1
fi

# 清理舊檔案
echo "清理舊檔案..."
make clean 2>/dev/null

# 編譯
echo ""
echo "編譯中..."
make

if [ $? -ne 0 ]; then
    echo ""
    echo "編譯失敗！"
    exit 1
fi

# 執行
echo ""
echo "編譯成功！開始遊戲..."
echo ""
./minesweeper

