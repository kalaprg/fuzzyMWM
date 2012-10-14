#ifndef FUZZYSETCONTAINER_H
#define FUZZYSETCONTAINER_H
#include "fuzzyset.h"

class FuzzySetContainer
{
public:
    FuzzySetContainer();
    FuzzySetContainer(FuzzySetContainer &c);
    ~FuzzySetContainer();
    FuzzySetContainer &operator=(FuzzySetContainer &rhs);
    int size() const;
    FuzzySet &operator[](int index);
    const FuzzySet &operator[](int index) const;
    int insert(const FuzzySet &item, int position = -1);
    void remove(int position);
};

#endif // FUZZYSETCONTAINER_H
