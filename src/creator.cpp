#include "creator.hpp"

OrganismCreator::OrganismCreator() { }

OrganismVector OrganismCreator::getOrganisms()
{
    return organisms;
}

void OrganismCreator::addOrganism(Organism* organismPtr)
{
    organisms.push_back(organismPtr);
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
