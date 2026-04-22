#include "game.h"
#include <iostream>

using namespace std;

int main() {
    int difficulty;
    
    cout << "\n========== 踩地雷遊戲 ==========" << endl;
    cout << "請選擇難度:" << endl;
    cout << "1. 簡單   (8x8 棋盤, 10個地雷)" << endl;
    cout << "2. 中等   (12x12 棋盤, 30個地雷)" << endl;
    cout << "3. 困難   (16x16 棋盤, 40個地雷)" << endl;
    cout << "請輸入難度 (1-3): ";
    cin >> difficulty;
    
    if (difficulty < 1 || difficulty > 3) {
        cout << "難度無效，使用預設難度（簡單）" << endl;
        difficulty = 1;
    }
    
    Game game(difficulty);
    game.play();
    
    return 0;
}
