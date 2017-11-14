#include "grid.hpp"

const int POSSIBLE_ADJACENT_CELL_COUNT = 4;

CellVector getCellSet(int xMax, int yMax)
{
    CellVector cellSet;
    for (int x = 0; x < xMax; x++) {
        for (int y = 0; y < yMax; y++) {
            cellSet.push_back(Cell(x, y));
        }
    }
    return cellSet;
}

Organism*** get2dOrganismArray(int xMax, int yMax)
{
    Organism*** organismArray = 0;
    // create columns first
    organismArray = new Organism**[xMax];
    for (int x = 0; x < xMax; x++) {
        // create rows second
        organismArray[x] = new Organism*[yMax];
        for (int y = 0; y < yMax; y++) {
            organismArray[x][y] = NULL;
        }
    }
    return organismArray;
}

Grid::Grid(int x, int y) :
    xMax(x),
    yMax(y),
    cellSet(getCellSet(x, y)),
    organism2dArray(get2dOrganismArray(x, y))
{
    std::cout << "Grid created." << std::endl;
}

void Grid::clearCell(const Cell& cell)
{
    setCellValue(cell, NULL);
}

void Grid::setCellValue(const Cell& cell, Organism* organism)
{
    organism2dArray[cell.x][cell.y] = organism;
}

Cell Grid::getRandomEmptyCellInGrid() const
{
    Cell cell;
    bool found = false;

    OrganismComparator comparator = OrganismComparator(EMPTY_SPACE_CHAR);
    cell = getRandomMatchingCell(
        cellSet, comparator, found);

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
    return organism2dArray[x][y];
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

Cell Grid::getRandomMatchingCell(
    const CellVector& cellSet,
    OrganismComparator comparator,
    bool& found) const
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
    OrganismComparator comparator = OrganismComparator(EMPTY_SPACE_CHAR);
    return getRandomMatchingCell(adjacentCells, comparator, found);
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
