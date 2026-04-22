#include "game.h"
#include <iostream>
#include <queue>

using namespace std;

Game::Game(int difficulty) : state(PLAYING), difficulty(difficulty) {
    initializeDifficulty(difficulty);
}

Game::~Game() {
    delete board;
}

void Game::initializeDifficulty(int difficulty) {
    int rows, cols, mines;
    
    switch(difficulty) {
        case 1:  // 簡單
            rows = 8;
            cols = 8;
            mines = 10;
            break;
        case 2:  // 中等
            rows = 12;
            cols = 12;
            mines = 30;
            break;
        case 3:  // 困難
            rows = 16;
            cols = 16;
            mines = 40;
            break;
        default:
            rows = 8;
            cols = 8;
            mines = 10;
    }
    
    board = new Board(rows, cols, mines);
    board->initialize();
}

void Game::play() {
    Display::showWelcome(difficulty);
    Display::showBoard(board);
    Display::showMenu();
    
    string command;
    int row, col;
    
    while (state == PLAYING) {
        cout << "輸入命令: ";
        cin >> command;
        
        if (command == "q") {
            cout << "遊戲結束，感謝遊玩！" << endl;
            break;
        } else if (command == "r") {
            cin >> row >> col;
            if (board->isValid(row, col)) {
                revealCell(row, col);
                state = checkGameState();
            } else {
                Display::showError("座標無效");
            }
        } else if (command == "m") {
            cin >> row >> col;
            if (board->isValid(row, col)) {
                markCell(row, col);
            } else {
                Display::showError("座標無效");
            }
        } else {
            Display::showError("命令無效");
            continue;
        }
        
        Display::showBoard(board);
    }
    
    // 顯示最終結果
    Display::showGameResult(state == WON, board);
}

GameState Game::checkGameState() {
    // 檢查輸局（踩到地雷）
    for (int i = 0; i < board->getRows(); i++) {
        for (int j = 0; j < board->getCols(); j++) {
            Cell& cell = board->getCell(i, j);
            if (cell.getRevealed() && cell.getMine()) {
                return LOST;
            }
        }
    }
    
    // 檢查贏局（所有非地雷格子都已揭露）
    int totalCells = board->getRows() * board->getCols();
    int mineCells = board->getTotalMines();
    int nonMineCells = totalCells - mineCells;
    
    if (board->getRevealedCells() == nonMineCells) {
        return WON;
    }
    
    return PLAYING;
}

GameState Game::getState() const {
    return state;
}

void Game::revealCell(int row, int col) {
    Cell& cell = board->getCell(row, col);
    
    if (cell.getRevealed()) {
        return;
    }
    
    board->revealCell(row, col);
    
    // 如果是空格，使用泛洪填充揭露周圍格子
    if (cell.getAdjacentMines() == 0) {
        floodFill(row, col);
    }
}

void Game::markCell(int row, int col) {
    board->markCell(row, col);
}

void Game::unmarkCell(int row, int col) {
    board->unmarkCell(row, col);
}

void Game::floodFill(int row, int col) {
    queue<pair<int, int>> q;
    q.push({row, col});
    
    int dr[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        
        for (int k = 0; k < 8; k++) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            
            if (board->isValid(nr, nc)) {
                Cell& nextCell = board->getCell(nr, nc);
                if (!nextCell.getRevealed()) {
                    board->revealCell(nr, nc);
                    if (nextCell.getAdjacentMines() == 0) {
                        q.push({nr, nc});
                    }
                }
            }
        }
    }
}
