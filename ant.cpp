#include <ant.hpp>

Ant::~Ant() {}

Ant::Ant(const Cell& cell, Grid* grid, OrganismCreator* creator) :
    OrganismImpl::OrganismImpl(3, ANT_CHAR, cell, grid, creator)
{ }

Cell Ant::getNextAdjacentCell(bool& found) const
{
    Cell cell = getCell();
    CellVector adjacentCells = myGrid->getRandomizedAdjacentCells(cell);
    return myGrid->getEmptyCell(adjacentCells, found);
}

void Ant::move()
{
    Cell newCell;
    bool canMove;
    canMove = false;
    newCell = getNextAdjacentCell(canMove);

    if (canMove) {
        moveOrganism(newCell);
    }
}

AntCreator::AntCreator(Grid* grid) :
    OrganismCreator::OrganismCreator(grid)
{ }

Ant* AntCreator::create(const Cell& cell)
{
    Ant* newAnt = new Ant(cell, myGrid, this);
    store(newAnt, cell);
    return newAnt;
}
