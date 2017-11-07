#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ant.hpp"
#include "cell.hpp"
#include "creator.hpp"
#include "doodlebug.hpp"
#include "grid.hpp"
#include "organism.hpp"
#include <iostream>

class GridController
{
public:
    GridController();
    GridController(Grid* gridPtr);
    void createAnts(int count);
    void createDoodlebugs(int count);
    void display() const;
    void step();
private:
    Grid* grid;
    AntCreator* antCreator;
    DoodlebugCreator* doodlebugCreator;
    void moveAnts();
    void moveDoodlebugs();
    void createOrganisms(OrganismCreator* creator, int count);
};

#endif /* CONTROLLER_H */
