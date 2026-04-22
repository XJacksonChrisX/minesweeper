#ifndef CELL_H
#define CELL_H

class Cell {
private:
    bool isMine;           // 是否是地雷
    bool isRevealed;       // 是否已被揭露
    bool isMarked;         // 是否已被標記為地雷
    int adjacentMines;     // 相鄰地雷數量

public:
    Cell();
    
    // Getter
    bool getMine() const;
    bool getRevealed() const;
    bool getMarked() const;
    int getAdjacentMines() const;
    
    // Setter
    void setMine(bool mine);
    void setRevealed(bool revealed);
    void setMarked(bool marked);
    void setAdjacentMines(int count);
};

#endif
