#ifndef FUZZYSET_H
#define FUZZYSET_H
#include <string>

class FuzzySet
{
public:
    virtual ~FuzzySet() {}
    virtual FuzzySet *getInstance() const {return 0;}
    virtual FuzzySet *getInstance(float /*left*/, float /*middle*/, float /*right*/) const {return 0;}
    virtual FuzzySet &operator=(const FuzzySet &rhs) = 0;
    virtual bool operator==(const FuzzySet &rhs) const = 0;
    virtual void getSupportRange(float &left, float &right) const = 0;
    virtual float getMi(float x) const = 0;
    virtual std::string toString() const = 0;
};

#endif // FUZZYSET_H
