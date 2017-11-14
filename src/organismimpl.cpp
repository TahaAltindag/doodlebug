#include "organismimpl.hpp"

OrganismImpl::OrganismImpl(
    int requiredSurvivalTime,
    char organismSymbol,
    const Cell& cell
) :
    Organism::Organism(requiredSurvivalTime, organismSymbol, cell)
{ }

OrganismImpl::~OrganismImpl() {}

bool OrganismImpl::readyToBreed() const
{
    if (getTimeSurvived() == 0)
        return false;
    return ((getTimeSurvived() % required_survival_time_for_breeding) == 0);
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

void OrganismImpl::move(const Cell& newCell)
{
    setCell(newCell);
}
