#ifndef BOARD_H
#define BOARD_H

#include "cell.h"

class Board {
private:
    Cell** board;           // 二維棋盤
    int rows;              // 行數
    int cols;              // 列數
    int totalMines;        // 地雷總數
    int revealedCells;     // 已揭露的格子數

public:
    Board(int rows, int cols, int mines);
    ~Board();
    
    // 初始化棋盤和放置地雷
    void initialize();
    
    // Getter
    Cell& getCell(int row, int col);
    int getRows() const;
    int getCols() const;
    int getTotalMines() const;
    int getRevealedCells() const;
    
    // 操作
    void revealCell(int row, int col);
    void markCell(int row, int col);
    void unmarkCell(int row, int col);
    bool isValid(int row, int col) const;
    
    // 計算相鄰地雷數
    void calculateAdjacentMines();
};

#endif
