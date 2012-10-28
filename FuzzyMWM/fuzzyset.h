#ifndef FUZZYSET_H
#define FUZZYSET_H

class FuzzySet
{
public:
    virtual ~FuzzySet() {}
    virtual FuzzySet *getInstance() const {return 0;}
    virtual FuzzySet &operator=(const FuzzySet &rhs) = 0;
    virtual void getSupportRange(float &left, float &right) const = 0;
    virtual float getMi(float x) const = 0;
};

#endif // FUZZYSET_H
