// Walter Savitch, 2014 -- CH16 Question #9 Answer ("Doodlebug")

// The goal for this project is to create a simple two-dimensional
// predator-prey simulation. In this simulation the prey are ants
// and the predators are doodlebugs. These critters live in a world
// composed of a 20 × 20 grid of cells. Only one critter may occupy
// a cell at a time. The grid is enclosed, so a critter is not
// allowed to move off the edges of the world. Time is simulated in
// time steps. Each critter performs some action every time step.

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

// debugging only:
#include<mach/mach.h>
struct task_basic_info t_info;

const int POSSIBLE_ADJACENT_CELL_COUNT = 4;

const char ANT_CHAR = 'o';
const char DOODLEBUG_CHAR = 'X';
const char EMPTY_SPACE_CHAR = '-';

// ============================== Cell ==============================

class Cell
{
public:
    Cell();
    Cell(int xVal, int yVal);
    friend bool operator ==(const Cell& cell1, const Cell& cell2);
    int x;
    int y;
    std::string str() const;
};

typedef std::vector<Cell> CellVector;

Cell::Cell()
{ }

Cell::Cell(int xVal, int yVal) : x(xVal), y(yVal)
{ }

bool operator ==(const Cell& a, const Cell& b)
{
    return ((a.x == b.x) && (a.y == b.y));
}

std::string Cell::str() const
{
    return '(' + std::to_string(x) + ',' + std::to_string(y) + ')';
}

// ============================== Organism Interface ==============================

class Organism
{
public:
    Organism(int requiredSurvivalTime, char organismSymbol, const Cell& cell);
    virtual ~Organism() = 0;
    virtual void move() = 0;
    virtual void breed() = 0;
    virtual void die() = 0;
    virtual Cell getCell() const = 0;
    virtual void setCell(const Cell& cell) = 0;
    virtual int getTimeSurvived() const = 0;
    virtual void incrementTimeSurvived() = 0;
    virtual bool readyToBreed() const = 0;
    const char symbol;
    friend char getSymbolOrEmptyChar(Organism* organism);
protected:
    const int required_survival_time_for_breeding;
    int timeSurvived;
    Cell myCell;
};

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

typedef std::vector<Organism*> OrganismVector;

class OrganismComparator
{
public:
    OrganismComparator(char value);
    OrganismComparator(Organism* organism);
    bool compare(Organism* organism);
private:
    const char compareValue;
};

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

// ============================== Grid ==============================

CellVector getCell1dSet(int xMax, int yMax)
{
    CellVector cellSet;
    for (int x = 0; x < xMax; x++) {
        for (int y = 0; y < yMax; y++) {
            cellSet.push_back(Cell(x, y));
        }
    }
    return cellSet;
}

Organism*** getCell2dArray(int xMax, int yMax)
{
    Organism*** cellArray = 0;
    // create columns first
    cellArray = new Organism**[xMax];
    for (int x = 0; x < xMax; x++) {
        // create rows second
        cellArray[x] = new Organism*[yMax];
        for (int y = 0; y < yMax; y++) {
            cellArray[x][y] = NULL;
        }
    }
    return cellArray;
}

class Grid
{
public:
    Grid(int x, int y);
    void display() const;
    Cell getRandomEmptyCellInGrid() const;
    void clearCell(const Cell& cell);
    void setCellValue(const Cell& cell, Organism* organism);
    Organism* getCellValue(const Cell& cell) const;
    Organism* getCellValue(int x, int y) const;
    CellVector getAdjacentCells(const Cell& cell) const;
    CellVector getRandomizedAdjacentCells(const Cell& cell) const;
    Cell getEmptyCell(const CellVector& adjacentCells, bool& found) const;
    Cell getRandomMatchingCell(const CellVector& cellSet, OrganismComparator comparator, bool& found) const;
private:
    bool cellContains(const Cell& cell, OrganismComparator comparator) const;
    bool isInRange(const Cell& cell) const;
    Organism*** cell2dArray;
    const CellVector cell1dSet;
    const int xMax;
    const int yMax;
};

Grid::Grid(int x, int y) :
    xMax(x),
    yMax(y),
    cell1dSet(getCell1dSet(x, y)),
    cell2dArray(getCell2dArray(x, y))
{
    std::cout << "Grid created." << std::endl;
}

void Grid::clearCell(const Cell& cell)
{
    setCellValue(cell, NULL);
}

void Grid::setCellValue(const Cell& cell, Organism* organism)
{
    cell2dArray[cell.x][cell.y] = organism;
}

Cell Grid::getRandomEmptyCellInGrid() const
{
    Cell cell;
    bool found = false;

    cell = getRandomMatchingCell(cell1dSet, OrganismComparator(EMPTY_SPACE_CHAR), found);

    if (found)
        return cell;
    else {
        // The user likely created too many organisms.
        std::cout << "ERROR: There are no remaining empty cells in grid.\n";
        std::cout << "INFO: Ensure # of organisms < # of cells in grid.\n";
        exit(1);
    }
}

Organism* Grid::getCellValue(int x, int y) const
{
    return cell2dArray[x][y];
}

Organism* Grid::getCellValue(const Cell& cell) const
{
    return getCellValue(cell.x, cell.y);
}

bool Grid::cellContains(const Cell& cell, OrganismComparator comparator) const
{
    return comparator.compare(getCellValue(cell));
}

bool Grid::isInRange(const Cell& cell) const
{
    bool aboveMin = ((cell.x >= 0) && (cell.y >= 0));  // must be positive
    bool belowMax = ((cell.x < xMax) && (cell.y < yMax));
    return (aboveMin && belowMax);
}

CellVector Grid::getAdjacentCells(const Cell& cell) const
{
    CellVector actualAdjacentCells;

    int x = cell.x;
    int y = cell.y;

    Cell possibleAdjacentCells[POSSIBLE_ADJACENT_CELL_COUNT] = {
        Cell(x, y + 1),  // up
        Cell(x, y - 1),  // down
        Cell(x - 1, y),  // left
        Cell(x + 1, y)   // right
    };

    // if any location is off the grid, it's invalid.
    for (int i = 0; i < POSSIBLE_ADJACENT_CELL_COUNT; i++) {
        Cell possibleCell = possibleAdjacentCells[i];
        if (isInRange(possibleCell))
            actualAdjacentCells.push_back(possibleCell);
    }

    return actualAdjacentCells;
}

CellVector Grid::getRandomizedAdjacentCells(const Cell& cell) const
{
    CellVector adjacentCells = getAdjacentCells(cell);
    std::random_shuffle(adjacentCells.begin(), adjacentCells.end());
    return adjacentCells;
}

Cell Grid::getRandomMatchingCell(const CellVector& cellSet, OrganismComparator comparator, bool& found) const
{
    Cell cell;
    int cellsSearched = 0;
    int numCells = cellSet.size();

    while (cellsSearched < numCells && !found) {
        int randIndex = (std::rand() % numCells);  // one of [0, 1, 2, 3]
        cell = cellSet[randIndex];
        if (cellContains(cell, comparator)) {
            found = true;
        }
        cellsSearched++;
    }
    return cell;
}

Cell Grid::getEmptyCell(const CellVector& adjacentCells, bool& found) const
{
    return getRandomMatchingCell(adjacentCells, OrganismComparator(EMPTY_SPACE_CHAR), found);
}

void Grid::display() const
{
    for (int x = 0; x < xMax; x++) {
        for (int y = 0; y < yMax; y++) {
            std::cout << getSymbolOrEmptyChar(getCellValue(x, y)) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// ============================== OrganismCreator ==============================

// Instances of this creator contain instances of organisms
// Instances of this creator record instances of organisms

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

OrganismCreator::OrganismCreator(Grid* grid) : myGrid(grid) { }

OrganismVector OrganismCreator::getOrganisms()
{
    return organisms;
}

void OrganismCreator::store(Organism* organismPtr, Cell cell)
{
    organisms.push_back(organismPtr);
    myGrid->setCellValue(cell, organismPtr);
}

int OrganismCreator::count() const
{
    return organisms.size();
}

int OrganismCreator::findIndex(const Cell& cell)
{
    for (int i = 0; i < organisms.size(); i++) {
        Cell orgCell = organisms[i]->getCell();
        if (orgCell == cell)
            return i;
    }

    std::cout << "Unable to find index of cell that should exist: " << cell.str() << std::endl;
    exit(1);
}

void OrganismCreator::remove(const Cell& cell)
{
    int index = findIndex(cell);
    Organism* ptr = organisms[index];
    organisms.erase(organisms.begin() + index);
    delete ptr;
    ptr = nullptr;
}

// ============================== Organism Implementation ==============================

class OrganismImpl : public Organism
{
public:
    OrganismImpl(
        int requiredSurvivalTime,
        char organismSymbol,
        const Cell& cell,
        Grid* grid,
        OrganismCreator* creator);
    ~OrganismImpl() = 0;
    virtual void move() = 0;
    void breed();
    Cell getCell() const;
    void setCell(const Cell& cell);
    int getTimeSurvived() const;
    void incrementTimeSurvived();
    bool readyToBreed() const;
    void die();
private:
    OrganismCreator* myCreator;
protected:
    void moveOrganism(const Cell& newCell);
    Grid* myGrid;
};

OrganismImpl::OrganismImpl(
    int requiredSurvivalTime,
    char organismSymbol,
    const Cell& cell,
    Grid* grid,
    OrganismCreator* creator
) :
    Organism::Organism(requiredSurvivalTime, organismSymbol, cell),
    myCreator(creator),
    myGrid(grid)
{ }

OrganismImpl::~OrganismImpl() {}

bool OrganismImpl::readyToBreed() const
{
    if (getTimeSurvived() == 0)
        return false;
    return ((getTimeSurvived() % required_survival_time_for_breeding) == 0);
}

void OrganismImpl::breed()
{
    Cell breedCell;
    bool canBreed = false;

    CellVector adjacentCells = myGrid->getRandomizedAdjacentCells(getCell());

    breedCell = myGrid->getEmptyCell(adjacentCells, canBreed);

    if (canBreed) {
        myCreator->create(breedCell);
    }
}

void OrganismImpl::setCell(const Cell& cell)
{
    this->myCell = cell;
}

Cell OrganismImpl::getCell() const
{
    return myCell;
}

int OrganismImpl::getTimeSurvived() const
{
    return timeSurvived;
}

void OrganismImpl::incrementTimeSurvived()
{
    timeSurvived++;
}

void OrganismImpl::die()
{
    myCreator->remove(getCell());
    myGrid->clearCell(getCell());
}

void OrganismImpl::moveOrganism(const Cell& newCell)
{
    myGrid->clearCell(getCell());
    myGrid->setCellValue(newCell, this);
    setCell(newCell);
}

// ============================== Ant ==============================


class Ant : public OrganismImpl
{
public:
    Ant(const Cell& cell, Grid* grid, OrganismCreator* creator);
    ~Ant();
    void move();
private:
    Cell getNextAdjacentCell(bool& found) const;
};

Ant::~Ant() {}

Ant::Ant(const Cell& cell, Grid* grid, OrganismCreator* creator) :
    OrganismImpl::OrganismImpl(3, ANT_CHAR, cell, grid, creator)
{ }

Cell Ant::getNextAdjacentCell(bool& found) const
{
    Cell cell = getCell();
    CellVector adjacentCells = myGrid->getRandomizedAdjacentCells(cell);
    return myGrid->getEmptyCell(adjacentCells, found);
}

void Ant::move()
{
    Cell newCell;
    bool canMove;
    canMove = false;
    newCell = getNextAdjacentCell(canMove);

    if (canMove) {
        moveOrganism(newCell);
    }
}

class AntCreator : public OrganismCreator
{
public:
    AntCreator(Grid* grid);
    Ant* create(const Cell& cell);
};

AntCreator::AntCreator(Grid* grid) :
    OrganismCreator::OrganismCreator(grid)
{ }

Ant* AntCreator::create(const Cell& cell)
{
    Ant* newAnt = new Ant(cell, myGrid, this);
    store(newAnt, cell);
    return newAnt;
}

// ============================== Doodlebug ==============================

class Doodlebug : public OrganismImpl
{
public:
    Doodlebug(const Cell& cell, Grid* grid, OrganismCreator* creator);
    ~Doodlebug();
    void incrementTimeWithoutEating();
    bool isStarving() const;
    void move();
    void eatOrganism(Cell cell);
    Cell getNextCell(bool& found);
    Cell searchAnts(const CellVector& searchCells, bool& found) const;
private:
    int timeWithoutEating;
    const int max_time_without_eating;
    OrganismCreator* myCreator;
};

Doodlebug::~Doodlebug() {}

Doodlebug::Doodlebug(const Cell& cell, Grid* grid, OrganismCreator* creator) :
    OrganismImpl::OrganismImpl(8, DOODLEBUG_CHAR, cell, grid, creator),
    timeWithoutEating(0),
    max_time_without_eating(3)
{ }

void Doodlebug::move()
{
    Cell newCell;
    bool canMove;
    canMove = false;
    newCell = getNextCell(canMove);

    if (canMove) {
        moveOrganism(newCell);
    }
}


bool Doodlebug::isStarving() const
{
    return (timeWithoutEating >= max_time_without_eating);
}

void Doodlebug::incrementTimeWithoutEating()
{
    timeWithoutEating++;
}

void Doodlebug::eatOrganism(Cell cell)
{
    Organism* organism = myGrid->getCellValue(cell);
    organism->die();
    timeWithoutEating = 0;  // reset last ant meal, since doodlebug just ate
}

Cell Doodlebug::searchAnts(const CellVector& searchCells, bool& found) const
{
    return myGrid->getRandomMatchingCell(searchCells, OrganismComparator(ANT_CHAR), found);
}

Cell Doodlebug::getNextCell(bool& found)
{
    Cell cell = getCell();
    CellVector adjacentCells = myGrid->getRandomizedAdjacentCells(cell);

    Cell nextCell = searchAnts(adjacentCells, found);

    if (found) {
        eatOrganism(nextCell);
        return nextCell;
    } else {
        return myGrid->getEmptyCell(adjacentCells, found);
    }
}

typedef std::vector<Doodlebug*> DoodlebugVector;

class DoodlebugCreator : public OrganismCreator
{
public:
    DoodlebugCreator(Grid* grid);
    Doodlebug* create(const Cell& cell);
};

DoodlebugCreator::DoodlebugCreator(Grid* grid) :
    OrganismCreator::OrganismCreator(grid)
{ }

Doodlebug* DoodlebugCreator::create(const Cell& cell)
{
    Doodlebug* newDoodlebug = new Doodlebug(cell, myGrid, this);
    store(newDoodlebug, cell);
    return newDoodlebug;
}

// ============================== Grid Controller ==============================

// controls the grid, creating and removing organisms

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

    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
    if (KERN_SUCCESS != task_info(mach_task_self(),
                                  TASK_BASIC_INFO, (task_info_t)&t_info, 
                                  &t_info_count)) {
        exit(-1);
    }
    std::cout << "resident size is: " << t_info.resident_size << std::endl;
    std::cout << "virtual size is: " << t_info.virtual_size << std::endl;

}

// ============================== Main ==============================

int main()
{
    std::string temp;
    int count = 0;

    std::srand(std::time(NULL));  // required for random function usage

    Grid* grid = new Grid(20, 20);
    GridController controller = GridController(grid);

    // Initialize the world with 5 doodlebugs and 100 ants.
    std::cout << "Creating organisms..." << std::endl;
    controller.createAnts(100);
    controller.createDoodlebugs(5);
    controller.display();

    // After each time step, prompt the user to press
    // Enter to move to the next time step.

    while (true) {
        std::cout << "Press enter to move to the next time step.\n";
        std::getline(std::cin, temp);

        controller.step();
        count++;

        std::cout << "iterations: " << count <<std::endl;
        controller.display();
    }

    // You should see a cyclical pattern between the population of
    // predators and prey, although random perturbations may lead
    // to the elimination of one or both species.

    return 0;
}
