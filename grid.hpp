#ifndef GRID_H
#define GRID_H

#include <cell.hpp>
#include <organism.hpp>
#include <algorithm>
#include <cstdlib>

class Grid
{
public:
    Grid(int x, int y);
    void display() const;
    Cell getRandomEmptyCellInGrid() const;
    void clearCell(const Cell& cell);
    void setCellValue(const Cell& cell, Organism* organism);
    Organism* getCellValue(const Cell& cell) const;
    Organism* getCellValue(int x, int y) const;
    CellVector getAdjacentCells(const Cell& cell) const;
    CellVector getRandomizedAdjacentCells(const Cell& cell) const;
    Cell getEmptyCell(const CellVector& adjacentCells, bool& found) const;
    Cell getRandomMatchingCell(const CellVector& cellSet, OrganismComparator comparator, bool& found) const;
private:
    bool cellContains(const Cell& cell, OrganismComparator comparator) const;
    bool isInRange(const Cell& cell) const;
    Organism*** cell2dArray;
    const CellVector cell1dSet;
    const int xMax;
    const int yMax;
};

#endif /* GRID_H */
