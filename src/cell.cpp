#include "cell.h"

Cell::Cell() : isMine(false), isRevealed(false), isMarked(false), adjacentMines(0) {}

bool Cell::getMine() const {
    return isMine;
}

bool Cell::getRevealed() const {
    return isRevealed;
}

bool Cell::getMarked() const {
    return isMarked;
}

int Cell::getAdjacentMines() const {
    return adjacentMines;
}

void Cell::setMine(bool mine) {
    isMine = mine;
}

void Cell::setRevealed(bool revealed) {
    isRevealed = revealed;
}

void Cell::setMarked(bool marked) {
    isMarked = marked;
}

void Cell::setAdjacentMines(int count) {
    adjacentMines = count;
}
