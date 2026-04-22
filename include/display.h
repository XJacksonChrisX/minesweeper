#ifndef DISPLAY_H
#define DISPLAY_H

#include "board.h"

class Display {
public:
    // 顯示歡迎訊息
    static void showWelcome(int difficulty);
    
    // 顯示棋盤
    static void showBoard(Board* board);
    
    // 顯示操作菜單
    static void showMenu();
    
    // 顯示遊戲結果
    static void showGameResult(bool won, Board* board);
    
    // 顯示錯誤訊息
    static void showError(const char* message);
};

#endif
