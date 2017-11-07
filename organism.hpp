#ifndef ORGANISM_H
#define ORGANISM_H

#include "cell.hpp"
#include <vector>

const char ANT_CHAR = 'o';
const char DOODLEBUG_CHAR = 'X';
const char EMPTY_SPACE_CHAR = '-';

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

#endif /* ORGANISM_H */
