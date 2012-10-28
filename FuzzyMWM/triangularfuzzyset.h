#ifndef TRIANGULARFUZZYSET_H
#define TRIANGULARFUZZYSET_H
#include "fuzzyset.h"

class TriangularFuzzySet : public FuzzySet
{
public:
    TriangularFuzzySet();
    TriangularFuzzySet(float a, float b, float c);
    virtual ~TriangularFuzzySet() {}
    virtual FuzzySet *getInstance() const;
    virtual FuzzySet &operator=(const FuzzySet &rhs);
    FuzzySet& operator=(const TriangularFuzzySet &rhs)
    { return  static_cast<FuzzySet &>(*this) = rhs; }
    virtual void getSupportRange(float &left, float &right) const;
    virtual float getMi(float x) const;
private:
    float a_, b_, c_;
};

#endif // TRIANGULARFUZZYSET_H
