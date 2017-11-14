#ifndef ORGANISMIMPL_H
#define ORGANISMIMPL_H

#include "cell.hpp"
#include "organism.hpp"

class OrganismImpl : public Organism
{
public:
    OrganismImpl(
        int requiredSurvivalTime,
        char organismSymbol,
        const Cell& cell);
    ~OrganismImpl() = 0;
    virtual void move(const Cell& newCell);
    Cell getCell() const;
    void setCell(const Cell& cell);
    int getTimeSurvived() const;
    void incrementTimeSurvived();
    bool readyToBreed() const;
    void die();
};

#endif /* ORGANISMIMPL_H */
