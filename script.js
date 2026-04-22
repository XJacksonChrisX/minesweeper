// 遊戲配置
const DIFFICULTY_LEVELS = {
    easy: {
        rows: 8,
        cols: 8,
        mines: 10
    },
    medium: {
        rows: 10,
        cols: 10,
        mines: 30
    },
    hard: {
        rows: 12,
        cols: 12,
        mines: 50
    }
};

// 遊戲狀態
let gameState = {
    board: [],
    revealed: [],
    flagged: [],
    gameOver: false,
    won: false,
    startTime: null,
    currentDifficulty: 'medium',
    timerInterval: null
};

// 初始化遊戲
function initGame(difficulty = 'medium') {
    // 清除舊計時器
    if (gameState.timerInterval) {
        clearInterval(gameState.timerInterval);
    }

    // 重置遊戲狀態
    gameState.currentDifficulty = difficulty;
    gameState.gameOver = false;
    gameState.won = false;
    gameState.startTime = null;

    const config = DIFFICULTY_LEVELS[difficulty];
    const totalCells = config.rows * config.cols;

    // 初始化棋盤
    gameState.board = Array(totalCells).fill(0);
    gameState.revealed = Array(totalCells).fill(false);
    gameState.flagged = Array(totalCells).fill(false);

    // 放置地雷
    placeMines(config.mines, totalCells);

    // 計算每個格子周圍的地雷數
    calculateNumbers(config.rows, config.cols);

    // 渲染棋盤
    renderBoard(config.rows, config.cols);

    // 隱藏遊戲結束對話框
    document.getElementById('gameOver').classList.add('hidden');

    // 更新難度按鈕
    document.querySelectorAll('.diff-btn').forEach(btn => {
        btn.classList.remove('active');
        if (btn.dataset.difficulty === difficulty) {
            btn.classList.add('active');
        }
    });

    // 更新地雷計數
    updateMineCount(config.mines);
}

// 放置地雷
function placeMines(mineCount, totalCells) {
    let placed = 0;
    while (placed < mineCount) {
        const randomIndex = Math.floor(Math.random() * totalCells);
        if (gameState.board[randomIndex] !== 'M') {
            gameState.board[randomIndex] = 'M';
            placed++;
        }
    }
}

// 計算每個格子周圍的地雷數
function calculateNumbers(rows, cols) {
    for (let row = 0; row < rows; row++) {
        for (let col = 0; col < cols; col++) {
            const index = row * cols + col;
            if (gameState.board[index] !== 'M') {
                const count = countAdjacentMines(row, col, rows, cols);
                gameState.board[index] = count;
            }
        }
    }
}

// 計算相鄰地雷數
function countAdjacentMines(row, col, rows, cols) {
    let count = 0;
    for (let r = Math.max(0, row - 1); r <= Math.min(rows - 1, row + 1); r++) {
        for (let c = Math.max(0, col - 1); c <= Math.min(cols - 1, col + 1); c++) {
            if (r !== row || c !== col) {
                const index = r * cols + c;
                if (gameState.board[index] === 'M') {
                    count++;
                }
            }
        }
    }
    return count;
}

// 渲染棋盤
function renderBoard(rows, cols) {
    const gameBoard = document.getElementById('gameBoard');
    gameBoard.innerHTML = '';
    gameBoard.style.gridTemplateColumns = `repeat(${cols}, 1fr)`;

    for (let i = 0; i < rows * cols; i++) {
        const cell = document.createElement('div');
        cell.className = 'cell';
        cell.dataset.index = i;

        // 添加點擊事件
        cell.addEventListener('click', () => revealCell(i));
        cell.addEventListener('contextmenu', (e) => {
            e.preventDefault();
            toggleFlag(i);
        });

        gameBoard.appendChild(cell);
    }

    updateDisplay();
}

// 更新格子顯示
function updateDisplay() {
    const config = DIFFICULTY_LEVELS[gameState.currentDifficulty];
    document.querySelectorAll('.cell').forEach(cell => {
        const index = parseInt(cell.dataset.index);
        cell.textContent = '';
        cell.className = 'cell';

        if (gameState.revealed[index]) {
            cell.classList.add('opened');
            if (gameState.board[index] === 'M') {
                cell.classList.add('mine');
                cell.textContent = '💣';
            } else if (gameState.board[index] === 0) {
                cell.classList.add('empty');
                cell.textContent = '';
            } else {
                cell.classList.add(`num-${gameState.board[index]}`);
                cell.textContent = gameState.board[index];
            }
        } else if (gameState.flagged[index]) {
            cell.classList.add('flagged');
            cell.textContent = '🚩';
        }
    });
}

// 開啟格子
function revealCell(index) {
    if (gameState.gameOver || gameState.won) return;
    if (gameState.revealed[index] || gameState.flagged[index]) return;

    // 開始計時
    if (!gameState.startTime) {
        gameState.startTime = Date.now();
        startTimer();
    }

    gameState.revealed[index] = true;

    // 踩到地雷
    if (gameState.board[index] === 'M') {
        gameState.gameOver = true;
        revealAllMines();
        endGame(false);
        return;
    }

    // 如果是空白，遞迴展開
    if (gameState.board[index] === 0) {
        const config = DIFFICULTY_LEVELS[gameState.currentDifficulty];
        revealEmpty(index, config.rows, config.cols);
    }

    updateDisplay();
    checkWin();
}

// 展開空白區域
function revealEmpty(index, rows, cols) {
    const row = Math.floor(index / cols);
    const col = index % cols;

    for (let r = Math.max(0, row - 1); r <= Math.min(rows - 1, row + 1); r++) {
        for (let c = Math.max(0, col - 1); c <= Math.min(cols - 1, col + 1); c++) {
            const neighborIndex = r * cols + c;
            if (!gameState.revealed[neighborIndex] && !gameState.flagged[neighborIndex]) {
                gameState.revealed[neighborIndex] = true;
                if (gameState.board[neighborIndex] === 0) {
                    revealEmpty(neighborIndex, rows, cols);
                }
            }
        }
    }
}

// 標記/取消標記地雷
function toggleFlag(index) {
    if (gameState.gameOver || gameState.won) return;
    if (gameState.revealed[index]) return;

    gameState.flagged[index] = !gameState.flagged[index];
    updateDisplay();
}

// 顯示所有地雷
function revealAllMines() {
    gameState.board.forEach((cell, index) => {
        if (cell === 'M') {
            gameState.revealed[index] = true;
        }
    });
    updateDisplay();
}

// 檢查是否獲勝
function checkWin() {
    const config = DIFFICULTY_LEVELS[gameState.currentDifficulty];
    const totalCells = config.rows * config.cols;
    const revealedNonMines = gameState.revealed.reduce((count, revealed, index) => {
        return revealed && gameState.board[index] !== 'M' ? count + 1 : count;
    }, 0);

    if (revealedNonMines === totalCells - config.mines) {
        gameState.won = true;
        endGame(true);
    }
}

// 遊戲結束
function endGame(won) {
    gameState.gameOver = true;
    if (gameState.timerInterval) {
        clearInterval(gameState.timerInterval);
    }

    const gameOverDiv = document.getElementById('gameOver');
    const gameOverText = document.getElementById('gameOverText');
    const gameStats = document.getElementById('gameStats');

    if (won) {
        gameOverText.textContent = '🎉 恭喜！你贏了！';
        gameOverText.style.color = '#4CAF50';
    } else {
        gameOverText.textContent = '💥 遊戲結束！';
        gameOverText.style.color = '#F44336';
    }

    const timeElapsed = gameState.startTime ? Math.floor((Date.now() - gameState.startTime) / 1000) : 0;
    gameStats.textContent = `用時：${timeElapsed} 秒`;

    gameOverDiv.classList.remove('hidden');
}

// 開始計時器
function startTimer() {
    gameState.timerInterval = setInterval(() => {
        const elapsed = Math.floor((Date.now() - gameState.startTime) / 1000);
        document.getElementById('timer').textContent = elapsed;
    }, 100);
}

// 更新地雷計數
function updateMineCount(count) {
    document.getElementById('mineCount').textContent = count;
}

// 事件監聽器
document.addEventListener('DOMContentLoaded', () => {
    // 難度按鈕
    document.querySelectorAll('.diff-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            initGame(btn.dataset.difficulty);
        });
    });

    // 重新開始按鈕
    document.getElementById('restartBtn').addEventListener('click', () => {
        initGame(gameState.currentDifficulty);
    });

    // 再玩一次按鈕
    document.getElementById('playAgainBtn').addEventListener('click', () => {
        initGame(gameState.currentDifficulty);
    });

    // 初始化遊戲
    initGame('medium');
});
