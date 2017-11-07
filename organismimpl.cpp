#include <organismimpl.hpp>

OrganismImpl::OrganismImpl(
    int requiredSurvivalTime,
    char organismSymbol,
    const Cell& cell,
    Grid* grid,
    OrganismCreator* creator
) :
    Organism::Organism(requiredSurvivalTime, organismSymbol, cell),
    myCreator(creator),
    myGrid(grid)
{ }

OrganismImpl::~OrganismImpl() {}

bool OrganismImpl::readyToBreed() const
{
    if (getTimeSurvived() == 0)
        return false;
    return ((getTimeSurvived() % required_survival_time_for_breeding) == 0);
}

void OrganismImpl::breed()
{
    Cell breedCell;
    bool canBreed = false;

    CellVector adjacentCells = myGrid->getRandomizedAdjacentCells(getCell());

    breedCell = myGrid->getEmptyCell(adjacentCells, canBreed);

    if (canBreed) {
        myCreator->create(breedCell);
    }
}

void OrganismImpl::setCell(const Cell& cell)
{
    this->myCell = cell;
}

Cell OrganismImpl::getCell() const
{
    return myCell;
}

int OrganismImpl::getTimeSurvived() const
{
    return timeSurvived;
}

void OrganismImpl::incrementTimeSurvived()
{
    timeSurvived++;
}

void OrganismImpl::die()
{
    myCreator->remove(getCell());
    myGrid->clearCell(getCell());
}

void OrganismImpl::moveOrganism(const Cell& newCell)
{
    myGrid->clearCell(getCell());
    myGrid->setCellValue(newCell, this);
    setCell(newCell);
}
