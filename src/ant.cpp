#include "ant.hpp"

Ant::~Ant() {}

Ant::Ant(const Cell& cell) :
    OrganismImpl::OrganismImpl(3, ANT_CHAR, cell)
{ }

Ant* AntCreator::create(const Cell& cell)
{
    Ant* newAnt = new Ant(cell);
    addOrganism(newAnt);
    return newAnt;
}
