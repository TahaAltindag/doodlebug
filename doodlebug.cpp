#include "doodlebug.hpp"

Doodlebug::~Doodlebug() {}

Doodlebug::Doodlebug(const Cell& cell) :
    OrganismImpl::OrganismImpl(8, DOODLEBUG_CHAR, cell),
    timeWithoutEating(0),
    max_time_without_eating(3)
{ }

bool Doodlebug::isStarving() const
{
    return (timeWithoutEating >= max_time_without_eating);
}

void Doodlebug::incrementTimeWithoutEating()
{
    timeWithoutEating++;
}

void Doodlebug::eat()
{
    timeWithoutEating = 0;  // reset time without eating, since doodlebug just ate
}

Doodlebug* DoodlebugCreator::create(const Cell& cell)
{
    Doodlebug* newDoodlebug = new Doodlebug(cell);
    addOrganism(newDoodlebug);
    return newDoodlebug;
}
