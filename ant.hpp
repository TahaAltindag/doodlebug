#ifndef ANT_H
#define ANT_H

#include "cell.hpp"
#include "creator.hpp"
#include "grid.hpp"
#include "organismimpl.hpp"

class Ant : public OrganismImpl
{
public:
    Ant(const Cell& cell, Grid* grid, OrganismCreator* creator);
    ~Ant();
    void move();
private:
    Cell getNextAdjacentCell(bool& found) const;
};

class AntCreator : public OrganismCreator
{
public:
    AntCreator(Grid* grid);
    Ant* create(const Cell& cell);
};

#endif /* ANT_H */
