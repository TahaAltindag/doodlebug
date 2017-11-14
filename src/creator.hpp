#ifndef CREATOR_H
#define CREATOR_H

#include "cell.hpp"
#include "grid.hpp"
#include "organism.hpp"
#include <iostream>

class OrganismCreator
{
public:
    OrganismCreator();
    virtual Organism* create(const Cell& cell) = 0;
    OrganismVector getOrganisms();
    void remove(const Cell& cell);
    int count() const;
protected:
    void addOrganism(Organism* organismPtr);
    int findIndex(const Cell& cell);
private:
    OrganismVector organisms;
};

#endif /* CREATOR_H */
