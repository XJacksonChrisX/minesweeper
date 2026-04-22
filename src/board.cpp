#include "board.h"
#include <cstdlib>
#include <ctime>

Board::Board(int rows, int cols, int mines) 
    : rows(rows), cols(cols), totalMines(mines), revealedCells(0) {
    board = new Cell*[rows];
    for (int i = 0; i < rows; i++) {
        board[i] = new Cell[cols];
    }
}

Board::~Board() {
    for (int i = 0; i < rows; i++) {
        delete[] board[i];
    }
    delete[] board;
}

void Board::initialize() {
    // 隨機放置地雷
    int placedMines = 0;
    srand(time(0));
    
    while (placedMines < totalMines) {
        int row = rand() % rows;
        int col = rand() % cols;
        
        if (!board[row][col].getMine()) {
            board[row][col].setMine(true);
            placedMines++;
        }
    }
    
    // 計算相鄰地雷數
    calculateAdjacentMines();
}

Cell& Board::getCell(int row, int col) {
    return board[row][col];
}

int Board::getRows() const {
    return rows;
}

int Board::getCols() const {
    return cols;
}

int Board::getTotalMines() const {
    return totalMines;
}

int Board::getRevealedCells() const {
    return revealedCells;
}

void Board::revealCell(int row, int col) {
    if (isValid(row, col) && !board[row][col].getRevealed()) {
        board[row][col].setRevealed(true);
        revealedCells++;
    }
}

void Board::markCell(int row, int col) {
    if (isValid(row, col) && !board[row][col].getRevealed()) {
        board[row][col].setMarked(!board[row][col].getMarked());
    }
}

void Board::unmarkCell(int row, int col) {
    if (isValid(row, col)) {
        board[row][col].setMarked(false);
    }
}

bool Board::isValid(int row, int col) const {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void Board::calculateAdjacentMines() {
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!board[i][j].getMine()) {
                int count = 0;
                for (int k = 0; k < 8; k++) {
                    int ni = i + dr[k];
                    int nj = j + dc[k];
                    if (isValid(ni, nj) && board[ni][nj].getMine()) {
                        count++;
                    }
                }
                board[i][j].setAdjacentMines(count);
            }
        }
    }
}
