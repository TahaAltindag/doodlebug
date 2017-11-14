#include "controller.hpp"

GridController::GridController() { }

GridController::GridController(Grid* gridPtr) : grid(gridPtr)
{
    this->antCreator = new AntCreator();
    this->doodlebugCreator = new DoodlebugCreator();
}

void GridController::createOrganism(OrganismCreator* creator, const Cell& cell)
{
    Organism* organism = creator->create(cell);
    grid->setCellValue(cell, organism);
}

void GridController::createOrganisms(OrganismCreator* creator, int count)
{
    for (int i = 0; i < count; i++) {
        Cell cell = grid->getRandomEmptyCellInGrid();
        createOrganism(creator, cell);
    }
}

void GridController::createAnts(int count)
{
    createOrganisms(antCreator, count);
}

void GridController::createDoodlebugs(int count)
{
    createOrganisms(doodlebugCreator, count);
}

void GridController::moveOrganism(Organism* organism, const Cell& newCell)
{
    grid->clearCell(organism->getCell());
    grid->setCellValue(newCell, organism);
    organism->move(newCell);
    organism->incrementTimeSurvived();
}

void GridController::killOrganism(Organism* organism, OrganismCreator* creator)
{
    creator->remove(organism->getCell());
    grid->clearCell(organism->getCell());
}

void GridController::makeDoodlebugEatAnt(Doodlebug* doodlebug, Cell cell)
{
    Organism* organism = grid->getCellValue(cell);
    killOrganism(organism, antCreator);
    doodlebug->eat();
}

Cell GridController::findNearbyAnts(const CellVector& nearbyCells, bool& found) const
{
    OrganismComparator comparator = OrganismComparator(ANT_CHAR);
    return grid->getRandomMatchingCell(nearbyCells, comparator, found);
}

Cell GridController::getNextDoodlebugCell(Doodlebug* doodlebug, bool& found)
{
    Cell cell = doodlebug->getCell();
    CellVector adjacentCells = grid->getRandomizedAdjacentCells(cell);

    Cell nextCell = findNearbyAnts(adjacentCells, found);

    if (found) {
        makeDoodlebugEatAnt(doodlebug, nextCell);
        return nextCell;
    } else {
        return grid->getEmptyCell(adjacentCells, found);
    }
}

void GridController::moveDoodlebug(Doodlebug* doodlebug)
{
    Cell newCell;
    bool canMove = false;

    newCell = getNextDoodlebugCell(doodlebug, canMove);

    if (canMove) {
        moveOrganism(doodlebug, newCell);
    }

    doodlebug->incrementTimeWithoutEating();
}

Cell GridController::getNextAdjacentCell(Organism* organism, bool& found)
{
    Cell cell = organism->getCell();
    CellVector adjacentCells = grid->getRandomizedAdjacentCells(cell);
    return grid->getEmptyCell(adjacentCells, found);
}

void GridController::moveAnt(Organism* organism)
{
    Cell newCell;
    bool canMove = false;
    newCell = getNextAdjacentCell(organism, canMove);

    if (canMove) {
        moveOrganism(organism, newCell);
    }
}

void GridController::breedOrganism(Organism* organism, OrganismCreator* creator)
{
    Cell breedCell;
    bool canBreed = false;

    Cell cell = organism->getCell();
    CellVector adjacentCells = grid->getRandomizedAdjacentCells(cell);

    breedCell = grid->getEmptyCell(adjacentCells, canBreed);

    if (canBreed) {
        createOrganism(creator, breedCell);
    }
}

void GridController::moveDoodlebugs()
{
    // During one turn, all the doodlebugs should move before the ants do.
    DoodlebugVector breedingDoodlebugs;
    DoodlebugVector starvingDoodlebugs;

    OrganismVector doodlebugs = doodlebugCreator->getOrganisms();

    for (int i = 0; i < doodlebugs.size(); i++) {
        Doodlebug* doodlebug = dynamic_cast<Doodlebug*>(doodlebugs[i]);

        moveDoodlebug(doodlebug);

        if (doodlebug->readyToBreed())
            breedingDoodlebugs.push_back(doodlebug);

        // If a doodlebug has not eaten an ant within the last three time steps,
        // then at the end of the third time step it will starve and die.
        // The doodlebug should then be removed from the grid of cells.
        if (doodlebug->isStarving())
            starvingDoodlebugs.push_back(doodlebug);
    }

    for (int i = 0; i < breedingDoodlebugs.size(); i++) {
        Doodlebug* doodlebug = breedingDoodlebugs[i];
        breedOrganism(doodlebug, doodlebugCreator);
    }

    for (int i = 0; i < starvingDoodlebugs.size(); i++) {
        Doodlebug* doodlebug = starvingDoodlebugs[i];
        killOrganism(doodlebug, doodlebugCreator);
    }
}

void GridController::moveAnts()
{
    OrganismVector ants = antCreator->getOrganisms();

    OrganismVector antsToBreed;
    for (int i = 0; i < ants.size(); i++) {
        Organism* ant = ants[i];

        moveAnt(ant);

        if (ant->readyToBreed())
            antsToBreed.push_back(ant);
    }

    for (int i = 0; i < antsToBreed.size(); i++) {
        Organism* ant = ants[i];
        breedOrganism(ant, antCreator);
    }
}

void GridController::step()
{
    moveDoodlebugs();
    moveAnts();
}

void GridController::display() const
{
    std::cout << "total # of doodlebugs: ";
    std::cout << doodlebugCreator->count() << std::endl;
    std::cout << "total # of ants: ";
    std::cout << antCreator->count() << std::endl;
    grid->display();
}

void GridController::writeDataToCsv(std::ostream& csvFileStream, int iteration) const
{
    csvFileStream << iteration << ",";
    csvFileStream << doodlebugCreator->count() << ",";
    csvFileStream << antCreator->count() << std::endl;
}
