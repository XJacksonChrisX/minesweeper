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

void Game::start() {
    cout << "\n========== 歡迎來到踩地雷遊戲 ==========" << endl;
    cout << "難度: ";
    if (difficulty == 1) cout << "簡單 (8x8, 10個地雷)" << endl;
    else if (difficulty == 2) cout << "中等 (12x12, 30個地雷)" << endl;
    else cout << "困難 (16x16, 40個地雷)" << endl;
    cout << "========================================\n" << endl;
}

void Game::displayBoard() {
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

void Game::displayMenu() {
    cout << "操作命令:" << endl;
    cout << "  r row col  - 揭露 (row, col) 的格子" << endl;
    cout << "  m row col  - 標記 (row, col) 為地雷" << endl;
    cout << "  q          - 退出遊戲" << endl;
    cout << endl;
}

void Game::play() {
    start();
    displayBoard();
    displayMenu();
    
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
                cout << "座標無效！" << endl;
            }
        } else if (command == "m") {
            cin >> row >> col;
            if (board->isValid(row, col)) {
                markCell(row, col);
            } else {
                cout << "座標無效！" << endl;
            }
        } else {
            cout << "命令無效！" << endl;
            continue;
        }
        
        displayBoard();
    }
    
    // 顯示最終結果
    if (state == WON) {
        cout << "\n🎉 恭喜！您贏了！ 🎉" << endl;
    } else if (state == LOST) {
        cout << "\n💥 遊戲結束，您踩到地雷了！ 💥" << endl;
        // 顯示所有地雷
        for (int i = 0; i < board->getRows(); i++) {
            for (int j = 0; j < board->getCols(); j++) {
                if (board->getCell(i, j).getMine()) {
                    board->getCell(i, j).setRevealed(true);
                }
            }
        }
        displayBoard();
    }
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
