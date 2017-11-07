#ifndef CELL_H
#define CELL_H

#include <string>
#include <vector>

class Cell
{
public:
    Cell();
    Cell(int xVal, int yVal);
    friend bool operator ==(const Cell& cell1, const Cell& cell2);
    int x;
    int y;
    std::string str() const;
};

typedef std::vector<Cell> CellVector;

#endif /* CELL_H */
