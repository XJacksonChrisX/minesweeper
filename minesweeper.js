'use strict';

const DIFFICULTY = {
  easy:   { rows: 9,  cols: 9,  mines: 10 },
  medium: { rows: 16, cols: 16, mines: 40 },
  hard:   { rows: 16, cols: 30, mines: 99 },
};

const STATE = { HIDDEN: 0, REVEALED: 1, FLAGGED: 2 };

let board = [];
let rows = 0;
let cols = 0;
let totalMines = 0;
let minesLeft = 0;
let revealedCount = 0;
let gameOver = false;
let gameStarted = false;
let timerInterval = null;
let elapsedSeconds = 0;

function init(difficulty) {
  const cfg = DIFFICULTY[difficulty];
  rows = cfg.rows;
  cols = cfg.cols;
  totalMines = cfg.mines;
  minesLeft = totalMines;
  revealedCount = 0;
  gameOver = false;
  gameStarted = false;
  elapsedSeconds = 0;

  stopTimer();
  updateMineCounter(minesLeft);
  updateTimer(0);
  setFaceButton('😊');
  updateStatus('');

  buildBoard();
  renderBoard();
}

function buildBoard() {
  board = [];
  for (let r = 0; r < rows; r++) {
    board[r] = [];
    for (let c = 0; c < cols; c++) {
      board[r][c] = {
        mine: false,
        state: STATE.HIDDEN,
        adjacent: 0,
      };
    }
  }
}

function placeMines(safeRow, safeCol) {
  let placed = 0;
  while (placed < totalMines) {
    const r = Math.floor(Math.random() * rows);
    const c = Math.floor(Math.random() * cols);
    if (board[r][c].mine) continue;
    if (Math.abs(r - safeRow) <= 1 && Math.abs(c - safeCol) <= 1) continue;
    board[r][c].mine = true;
    placed++;
  }

  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      if (!board[r][c].mine) {
        board[r][c].adjacent = countAdjacentMines(r, c);
      }
    }
  }
}

function countAdjacentMines(r, c) {
  let count = 0;
  forEachNeighbor(r, c, (nr, nc) => {
    if (board[nr][nc].mine) count++;
  });
  return count;
}

function forEachNeighbor(r, c, fn) {
  for (let dr = -1; dr <= 1; dr++) {
    for (let dc = -1; dc <= 1; dc++) {
      if (dr === 0 && dc === 0) continue;
      const nr = r + dr;
      const nc = c + dc;
      if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
        fn(nr, nc);
      }
    }
  }
}

function reveal(r, c) {
  const first = board[r][c];
  if (first.state !== STATE.HIDDEN) return;

  // Iterative BFS flood-fill to avoid stack overflow on large empty regions
  const queue = [[r, c]];
  first.state = STATE.REVEALED;
  revealedCount++;

  while (queue.length > 0) {
    const [cr, cc] = queue.shift();
    const cell = board[cr][cc];
    updateCell(cr, cc);

    if (cell.mine) {
      endGame(false, cr, cc);
      return;
    }

    if (cell.adjacent === 0) {
      forEachNeighbor(cr, cc, (nr, nc) => {
        if (board[nr][nc].state === STATE.HIDDEN) {
          board[nr][nc].state = STATE.REVEALED;
          revealedCount++;
          queue.push([nr, nc]);
        }
      });
    }
  }

  checkWin();
}

function chordReveal(r, c) {
  const cell = board[r][c];
  if (cell.state !== STATE.REVEALED || cell.adjacent === 0) return;

  let flagCount = 0;
  forEachNeighbor(r, c, (nr, nc) => {
    if (board[nr][nc].state === STATE.FLAGGED) flagCount++;
  });

  if (flagCount === cell.adjacent) {
    forEachNeighbor(r, c, (nr, nc) => {
      if (board[nr][nc].state === STATE.HIDDEN) reveal(nr, nc);
    });
  }
}

function toggleFlag(r, c) {
  const cell = board[r][c];
  if (cell.state === STATE.REVEALED) return;

  if (cell.state === STATE.HIDDEN) {
    cell.state = STATE.FLAGGED;
    minesLeft--;
  } else {
    cell.state = STATE.HIDDEN;
    minesLeft++;
  }
  updateMineCounter(minesLeft);
  updateCell(r, c);
}

function checkWin() {
  const nonMineCells = rows * cols - totalMines;
  if (revealedCount === nonMineCells) {
    endGame(true);
  }
}

function endGame(won, explodeRow, explodeCol) {
  gameOver = true;
  stopTimer();

  if (won) {
    setFaceButton('😎');
    updateStatus('🎉 You won!');
    flagAllMines();
    updateMineCounter(0);
  } else {
    setFaceButton('😵');
    updateStatus('💥 Game over!');
    revealAllMines(explodeRow, explodeCol);
  }
}

function flagAllMines() {
  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      if (board[r][c].mine && board[r][c].state !== STATE.FLAGGED) {
        board[r][c].state = STATE.FLAGGED;
        updateCell(r, c);
      }
    }
  }
}

function revealAllMines(explodeRow, explodeCol) {
  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      const cell = board[r][c];
      if (cell.mine) {
        if (r === explodeRow && c === explodeCol) {
          updateCell(r, c, 'exploded');
        } else if (cell.state !== STATE.FLAGGED) {
          cell.state = STATE.REVEALED;
          updateCell(r, c);
        }
      } else if (cell.state === STATE.FLAGGED) {
        updateCell(r, c, 'wrong-flag');
      }
    }
  }
}

// ── DOM rendering ──────────────────────────────────────────────────────────

const COLORS = ['', '#0000ff', '#008000', '#ff0000', '#000080', '#800000', '#008080', '#000000', '#808080'];

function renderBoard() {
  const container = document.getElementById('board');
  container.innerHTML = '';
  container.style.gridTemplateColumns = `repeat(${cols}, 32px)`;

  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      const el = document.createElement('div');
      el.className = 'cell hidden';
      el.dataset.r = r;
      el.dataset.c = c;

      el.addEventListener('click', onLeftClick);
      el.addEventListener('contextmenu', onRightClick);
      el.addEventListener('dblclick', onDoubleClick);

      container.appendChild(el);
    }
  }
}

function cellElement(r, c) {
  return document.querySelector(`[data-r="${r}"][data-c="${c}"]`);
}

function updateCell(r, c, extra) {
  const el = cellElement(r, c);
  if (!el) return;
  const cell = board[r][c];

  el.className = 'cell';
  el.textContent = '';
  el.style.color = '';

  if (extra === 'exploded') {
    el.classList.add('exploded');
    el.textContent = '💣';
    return;
  }

  if (extra === 'wrong-flag') {
    el.classList.add('revealed');
    el.textContent = '❌';
    return;
  }

  if (cell.state === STATE.HIDDEN) {
    el.classList.add('hidden');
  } else if (cell.state === STATE.FLAGGED) {
    el.classList.add('hidden', 'flagged');
    el.textContent = '🚩';
  } else {
    el.classList.add('revealed');
    if (cell.mine) {
      el.textContent = '💣';
    } else if (cell.adjacent > 0) {
      el.textContent = cell.adjacent;
      el.style.color = COLORS[cell.adjacent];
    }
  }
}

// ── Event handlers ─────────────────────────────────────────────────────────

function onLeftClick(e) {
  if (gameOver) return;
  const r = parseInt(this.dataset.r);
  const c = parseInt(this.dataset.c);

  if (!gameStarted) {
    gameStarted = true;
    placeMines(r, c);
    startTimer();
  }

  if (board[r][c].state === STATE.REVEALED) {
    chordReveal(r, c);
  } else {
    reveal(r, c);
  }
}

function onRightClick(e) {
  e.preventDefault();
  if (gameOver) return;
  const r = parseInt(this.dataset.r);
  const c = parseInt(this.dataset.c);
  toggleFlag(r, c);
}

function onDoubleClick(e) {
  if (gameOver) return;
  const r = parseInt(this.dataset.r);
  const c = parseInt(this.dataset.c);
  chordReveal(r, c);
}

// ── UI helpers ─────────────────────────────────────────────────────────────

function updateMineCounter(n) {
  const clamped = Math.max(-99, n);
  const abs = String(Math.abs(clamped)).padStart(clamped < 0 ? 2 : 3, '0');
  document.getElementById('mine-count').textContent = clamped < 0 ? '-' + abs : abs;
}

function updateTimer(s) {
  document.getElementById('timer').textContent = String(Math.min(999, s)).padStart(3, '0');
}

function setFaceButton(emoji) {
  document.getElementById('face-btn').textContent = emoji;
}

function updateStatus(msg) {
  document.getElementById('status').textContent = msg;
}

function startTimer() {
  stopTimer();
  timerInterval = setInterval(() => {
    elapsedSeconds++;
    updateTimer(elapsedSeconds);
  }, 1000);
}

function stopTimer() {
  if (timerInterval) {
    clearInterval(timerInterval);
    timerInterval = null;
  }
}

// ── Bootstrap ──────────────────────────────────────────────────────────────

document.addEventListener('DOMContentLoaded', () => {
  document.getElementById('face-btn').addEventListener('click', () => {
    const diff = document.getElementById('difficulty').value;
    init(diff);
  });

  document.getElementById('difficulty').addEventListener('change', () => {
    const diff = document.getElementById('difficulty').value;
    init(diff);
  });

  init('easy');
});
