#ifndef DOODLEBUG_H
#define DOODLEBUG_H

#include "cell.hpp"
#include "creator.hpp"
#include "organismimpl.hpp"
#include <vector>

class Doodlebug : public OrganismImpl
{
public:
    Doodlebug(const Cell& cell);
    ~Doodlebug();
    void incrementTimeWithoutEating();
    bool isStarving() const;
    void eat();
private:
    int timeWithoutEating;
    const int max_time_without_eating;
};

typedef std::vector<Doodlebug*> DoodlebugVector;

class DoodlebugCreator : public OrganismCreator
{
public:
    Doodlebug* create(const Cell& cell);
};

#endif /* DOODLEBUG_H */
