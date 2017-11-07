#include "controller.hpp"

GridController::GridController() { }

GridController::GridController(Grid* gridPtr) : grid(gridPtr)
{
    this->antCreator = new AntCreator(gridPtr);
    this->doodlebugCreator = new DoodlebugCreator(gridPtr);
}

void GridController::createOrganisms(OrganismCreator* creator, int count)
{
    for (int i = 0; i < count; i++) {
        Cell cell = grid->getRandomEmptyCellInGrid();
        creator->create(cell);
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

void GridController::moveDoodlebugs()
{
    // During one turn, all the doodlebugs should move before the ants do.
    DoodlebugVector breedingDoodlebugs;
    DoodlebugVector starvingDoodlebugs;

    OrganismVector doodlebugs = doodlebugCreator->getOrganisms();

    for (int i = 0; i < doodlebugs.size(); i++) {
        Doodlebug* doodlebug = dynamic_cast<Doodlebug*>(doodlebugs[i]);

        doodlebug->move();

        doodlebug->incrementTimeWithoutEating();
        doodlebug->incrementTimeSurvived();

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
        doodlebug->breed();
    }

    for (int i = 0; i < starvingDoodlebugs.size(); i++) {
        Doodlebug* doodlebug = starvingDoodlebugs[i];
        doodlebug->die();
    }
}

void GridController::moveAnts()
{
    OrganismVector ants = antCreator->getOrganisms();

    OrganismVector antsToBreed;
    for (int i = 0; i < ants.size(); i++) {
        Organism* ant = ants[i];

        ant->move();

        ant->incrementTimeSurvived();

        if (ant->readyToBreed())
            antsToBreed.push_back(ant);
    }

    for (int i = 0; i < antsToBreed.size(); i++) {
        Organism* ant = ants[i];
        ant->breed();
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
