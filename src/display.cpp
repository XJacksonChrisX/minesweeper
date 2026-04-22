#include "display.h"
#include <iostream>

using namespace std;

void Display::showWelcome(int difficulty) {
    cout << "\n========== 歡迎來到踩地雷遊戲 ==========" << endl;
    cout << "難度: ";
    if (difficulty == 1) cout << "簡單 (8x8, 10個地雷)" << endl;
    else if (difficulty == 2) cout << "中等 (12x12, 30個地雷)" << endl;
    else cout << "困難 (16x16, 40個地雷)" << endl;
    cout << "========================================\n" << endl;
}

void Display::showBoard(Board* board) {
    cout << "\n  ";
    // 顯示列號
    for (int j = 0; j < board->getCols(); j++) {
        cout << j % 10 << " ";
    }
    cout << endl;
    
    // 顯示棋盤
    for (int i = 0; i < board->getRows(); i++) {
        cout << i % 10 << " ";  // 顯示行號
        for (int j = 0; j < board->getCols(); j++) {
            Cell& cell = board->getCell(i, j);
            
            if (!cell.getRevealed()) {
                if (cell.getMarked()) {
                    cout << "🚩 ";  // 標記的格子
                } else {
                    cout << "□ ";  // 未揭露的格子
                }
            } else {
                if (cell.getMine()) {
                    cout << "💣 ";  // 地雷
                } else if (cell.getAdjacentMines() == 0) {
                    cout << "· ";  // 空格
                } else {
                    cout << cell.getAdjacentMines() << " ";  // 數字
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}

void Display::showMenu() {
    cout << "操作命令:" << endl;
    cout << "  r row col  - 揭露 (row, col) 的格子" << endl;
    cout << "  m row col  - 標記 (row, col) 為地雷" << endl;
    cout << "  q          - 退出遊戲" << endl;
    cout << endl;
}

void Display::showGameResult(bool won, Board* board) {
    if (won) {
        cout << "\n🎉 恭喜！您贏了！ 🎉" << endl;
    } else {
        cout << "\n💥 遊戲結束，您踩到地雷了！ 💥" << endl;
        // 顯示所有地雷
        for (int i = 0; i < board->getRows(); i++) {
            for (int j = 0; j < board->getCols(); j++) {
                if (board->getCell(i, j).getMine()) {
                    board->getCell(i, j).setRevealed(true);
                }
            }
        }
        showBoard(board);
    }
}

void Display::showError(const char* message) {
    cout << "❌ " << message << endl;
}
