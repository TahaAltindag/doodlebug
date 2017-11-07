#include "cell.hpp"

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
