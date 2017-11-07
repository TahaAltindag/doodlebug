#ifndef DOODLEBUG_H
#define DOODLEBUG_H

#include "cell.hpp"
#include "creator.hpp"
#include "grid.hpp"
#include "organism.hpp"
#include "organismimpl.hpp"
#include <vector>

class Doodlebug : public OrganismImpl
{
public:
    Doodlebug(const Cell& cell, Grid* grid, OrganismCreator* creator);
    ~Doodlebug();
    void incrementTimeWithoutEating();
    bool isStarving() const;
    void move();
    void eatOrganism(Cell cell);
    Cell getNextCell(bool& found);
    Cell searchAnts(const CellVector& searchCells, bool& found) const;
private:
    int timeWithoutEating;
    const int max_time_without_eating;
    OrganismCreator* myCreator;
};

typedef std::vector<Doodlebug*> DoodlebugVector;

class DoodlebugCreator : public OrganismCreator
{
public:
    DoodlebugCreator(Grid* grid);
    Doodlebug* create(const Cell& cell);
};

#endif /* DOODLEBUG_H */
