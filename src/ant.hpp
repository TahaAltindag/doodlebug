#ifndef ANT_H
#define ANT_H

#include "cell.hpp"
#include "creator.hpp"
#include "organismimpl.hpp"


class Ant : public OrganismImpl
{
public:
    Ant(const Cell& cell);
    ~Ant();
};

class AntCreator : public OrganismCreator
{
public:
    Ant* create(const Cell& cell);
};

#endif /* ANT_H */
