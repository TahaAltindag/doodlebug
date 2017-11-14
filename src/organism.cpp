#include "organism.hpp"

Organism::Organism(
    int requiredSurvivalTime,
    char organismSymbol,
    const Cell& cell
) :
    myCell(cell),
    required_survival_time_for_breeding(requiredSurvivalTime),
    symbol(organismSymbol),
    timeSurvived(0)
{ }

Organism::~Organism() {}

char getSymbolOrEmptyChar(Organism* organism)
{
    if (organism == NULL)
        return EMPTY_SPACE_CHAR;
    else
        return organism->symbol;
}

OrganismComparator::OrganismComparator(char value) :
    compareValue(value)
{ }

OrganismComparator::OrganismComparator(Organism* organism) :
    compareValue(getSymbolOrEmptyChar(organism))
{ }

bool OrganismComparator::compare(Organism* organism)
{
    return (compareValue == getSymbolOrEmptyChar(organism));
}
