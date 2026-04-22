#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "display.h"

enum GameState {
    PLAYING,   // 遊戲進行中
    WON,       // 玩家獲勝
    LOST       // 玩家敗北
};

class Game {
private:
    Board* board;
    GameState state;
    int difficulty;  // 難度等級

public:
    Game(int difficulty = 1);  // 1=簡單, 2=中等, 3=困難
    ~Game();
    
    // 遊戲控制
    void play();
    
    // 遊戲狀態檢查
    GameState checkGameState();
    GameState getState() const;
    
    // 輔助函數
    void revealCell(int row, int col);
    void markCell(int row, int col);
    void unmarkCell(int row, int col);
    void floodFill(int row, int col);
    
private:
    void initializeDifficulty(int difficulty);
};

#endif
