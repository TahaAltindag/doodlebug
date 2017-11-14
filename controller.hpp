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
    void createOrganism(OrganismCreator* creator, const Cell& cell);
    void createOrganisms(OrganismCreator* creator, int count);
    void moveOrganism(Organism* organism, const Cell& newCell);
    void killOrganism(Organism* organism, OrganismCreator* creator);
    void makeDoodlebugEatAnt(Doodlebug* doodlebug, Cell cell);
    void breedOrganism(Organism* organism, OrganismCreator* creator);
    Cell findNearbyAnts(const CellVector& nearbyCells, bool& found) const;
    Cell getNextDoodlebugCell(Doodlebug* doodlebug, bool& found);
    Cell getNextAdjacentCell(Organism* organism, bool& found);
    void moveDoodlebug(Doodlebug* doodlebug);
    void moveAnt(Organism* organism);
};

#endif /* CONTROLLER_H */
