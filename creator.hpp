#ifndef CREATOR_H
#define CREATOR_H

#include "cell.hpp"
#include "grid.hpp"
#include "organism.hpp"
#include <iostream>

class OrganismCreator
{
public:
    OrganismCreator(Grid* grid);
    virtual Organism* create(const Cell& cell) = 0;
    OrganismVector getOrganisms();
    void remove(const Cell& cell);
    int count() const;
protected:
    Grid* myGrid;
    void store(Organism* organism, Cell cell);
    int findIndex(const Cell& cell);
    OrganismVector organisms;
};

#endif /* CREATOR_H */
