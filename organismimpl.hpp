#ifndef ORGANISMIMPL_H
#define ORGANISMIMPL_H

#include "cell.hpp"
#include "creator.hpp"
#include "grid.hpp"
#include "organism.hpp"

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

#endif /* ORGANISMIMPL_H */
