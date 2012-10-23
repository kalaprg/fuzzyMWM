#ifndef FUZZYSET_H
#define FUZZYSET_H

class FuzzySet
{
public:
    FuzzySet();
    virtual ~FuzzySet() {}
    virtual FuzzySet &getInstance() const = 0;
    virtual FuzzySet &operator=(FuzzySet &rhs) = 0;
};

#endif // FUZZYSET_H
