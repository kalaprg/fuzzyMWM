#include <stdexcept>
#include <sstream>
#include "triangularfuzzyset.h"

TriangularFuzzySet::TriangularFuzzySet()
    : a_(0.0f), b_(0.0f), c_(0.0f)
{
}

TriangularFuzzySet::TriangularFuzzySet(float a, float b, float c)
    : a_(a), b_(b), c_(c)
{
    if(a > b || b > c)
        throw std::logic_error("a <= b <= c relation violated!");
}

FuzzySet *TriangularFuzzySet::getInstance() const
{
    return new TriangularFuzzySet();
}

FuzzySet *TriangularFuzzySet::getInstance(float left, float middle, float right) const
{
    return new TriangularFuzzySet(left, middle, right);
}

bool TriangularFuzzySet::operator==(const FuzzySet &rhs) const
{
    const TriangularFuzzySet &rhs2 = static_cast<const TriangularFuzzySet&>(rhs);
    return (rhs2.a_ == a_) && (rhs2.b_ == b_) && (rhs2.c_ == c_);
}

FuzzySet &TriangularFuzzySet::operator =(const FuzzySet &rhs)
{
    const TriangularFuzzySet &rhs2 = static_cast<const TriangularFuzzySet&>(rhs);
    a_ = rhs2.a_;
    b_ = rhs2.b_;
    c_ = rhs2.c_;
    return *this;
}

void TriangularFuzzySet::getSupportRange(float &left, float &right) const
{
    left = a_;
    right = c_;
}

float TriangularFuzzySet::getMi(float x) const
{
    float ba_diff = (a_ != b_) ? (b_ - a_) : 1;
    if(x < a_ || x > c_)
        return 0;
    else if(x <= b_)
        return (x - a_) / ba_diff;
    else
        return (c_ - x) / (c_ - b_);
}

std::string TriangularFuzzySet::toString() const
{
    std::stringstream stream;
    stream<<"T("<<a_<<";"<<b_<<";"<<c_<<")";
    return stream.str();
}
