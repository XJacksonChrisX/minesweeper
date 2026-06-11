// 踩地雷小遊戲 (Minesweeper) - C++ 簡易版
// 編譯: g++ minesweeper.cpp -o minesweeper
// 執行: ./minesweeper

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int SIZE = 9;   // 盤面大小 9x9
const int MINES = 10; // 地雷數量

char board[SIZE][SIZE];   // 玩家看到的盤面
bool mine[SIZE][SIZE];    // 哪裡有地雷
bool open[SIZE][SIZE];    // 哪裡已經被翻開
int opened = 0;           // 已翻開的格子數

// 在盤面上隨機埋地雷
void placeMines() {
    int count = 0;
    while (count < MINES) {
        int r = rand() % SIZE;
        int c = rand() % SIZE;
        if (!mine[r][c]) {      // 這格還沒有雷才埋
            mine[r][c] = true;
            count++;
        }
    }
}

// 計算 (r,c) 周圍 8 格有幾顆雷
int countNear(int r, int c) {
    int n = 0;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++) {
            int nr = r + i, nc = c + j;
            if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE && mine[nr][nc])
                n++;
        }
    return n;
}

// 印出目前盤面
void printBoard() {
    cout << "\n   ";
    for (int c = 0; c < SIZE; c++) cout << c << " ";
    cout << "\n";
    for (int r = 0; r < SIZE; r++) {
        cout << " " << r << " ";
        for (int c = 0; c < SIZE; c++)
            cout << board[r][c] << " ";
        cout << "\n";
    }
}

// 翻開 (r,c)；如果周圍沒有雷，就自動往外擴散
void reveal(int r, int c) {
    if (r < 0 || r >= SIZE || c < 0 || c >= SIZE) return; // 出界
    if (open[r][c]) return;                                // 已翻過

    open[r][c] = true;
    opened++;
    int n = countNear(r, c);

    if (n == 0) {
        board[r][c] = ' ';   // 周圍沒雷就顯示空白
        // 把周圍 8 格也一起翻開
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                reveal(r + i, c + j);
    } else {
        board[r][c] = '0' + n; // 顯示周圍雷數
    }
}

int main() {
    system("chcp 65001 > nul"); // 讓 Windows 主控台正確顯示中文
    srand(time(0));

    // 初始化盤面：全部顯示為 '?'
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++) {
            board[r][c] = '?';
            mine[r][c] = false;
            open[r][c] = false;
        }

    placeMines();

    cout << "=== 踩地雷 Minesweeper ===\n";
    cout << "盤面 " << SIZE << "x" << SIZE << "，共 " << MINES << " 顆地雷\n";
    cout << "輸入要翻開的 列 行 (例如: 3 4)\n";

    // 遊戲主迴圈
    while (true) {
        printBoard();
        int r, c;
        cout << "\n請輸入 列 行: ";
        cin >> r >> c;

        // 輸入檢查
        if (r < 0 || r >= SIZE || c < 0 || c >= SIZE) {
            cout << "超出範圍，請重新輸入！\n";
            continue;
        }

        if (mine[r][c]) {                 // 踩到雷 -> 遊戲結束
            cout << "\n💥 你踩到地雷了！遊戲結束。\n";
            // 顯示所有地雷位置
            for (int i = 0; i < SIZE; i++)
                for (int j = 0; j < SIZE; j++)
                    if (mine[i][j]) board[i][j] = '*';
            printBoard();
            break;
        }

        reveal(r, c);

        // 翻開的安全格 = 總格數 - 地雷數 -> 勝利
        if (opened == SIZE * SIZE - MINES) {
            cout << "\n🎉 恭喜！你找出所有安全格，獲勝！\n";
            printBoard();
            break;
        }
    }

    cout << "\n按任意鍵離開...";
    system("pause > nul"); // 讓視窗停住，看得到結果
    return 0;
}
