#include "doodlebug.hpp"

Doodlebug::~Doodlebug() {}

Doodlebug::Doodlebug(const Cell& cell, Grid* grid, OrganismCreator* creator) :
    OrganismImpl::OrganismImpl(8, DOODLEBUG_CHAR, cell, grid, creator),
    timeWithoutEating(0),
    max_time_without_eating(3)
{ }

void Doodlebug::move()
{
    Cell newCell;
    bool canMove;
    canMove = false;
    newCell = getNextCell(canMove);

    if (canMove) {
        moveOrganism(newCell);
    }
}


bool Doodlebug::isStarving() const
{
    return (timeWithoutEating >= max_time_without_eating);
}

void Doodlebug::incrementTimeWithoutEating()
{
    timeWithoutEating++;
}

void Doodlebug::eatOrganism(Cell cell)
{
    Organism* organism = myGrid->getCellValue(cell);
    organism->die();
    timeWithoutEating = 0;  // reset last ant meal, since doodlebug just ate
}

Cell Doodlebug::searchAnts(const CellVector& searchCells, bool& found) const
{
    return myGrid->getRandomMatchingCell(searchCells, OrganismComparator(ANT_CHAR), found);
}

Cell Doodlebug::getNextCell(bool& found)
{
    Cell cell = getCell();
    CellVector adjacentCells = myGrid->getRandomizedAdjacentCells(cell);

    Cell nextCell = searchAnts(adjacentCells, found);

    if (found) {
        eatOrganism(nextCell);
        return nextCell;
    } else {
        return myGrid->getEmptyCell(adjacentCells, found);
    }
}

DoodlebugCreator::DoodlebugCreator(Grid* grid) :
    OrganismCreator::OrganismCreator(grid)
{ }

Doodlebug* DoodlebugCreator::create(const Cell& cell)
{
    Doodlebug* newDoodlebug = new Doodlebug(cell, myGrid, this);
    store(newDoodlebug, cell);
    return newDoodlebug;
}
