#include "fuzzysetcontainer.h"
#include "fuzzyset.h"

FuzzySetContainer::FuzzySetContainer(const FuzzySetContainer &c)
{
    freeContainer();
    size_t size = c.sets_.size();
    sets_.resize(size, 0);
    for(int i = 0; i < (int)size; ++i)
    {
        sets_[i] = c.sets_[i]->getInstance();
        *(sets_[i]) = *(c.sets_[i]);
    }
}

FuzzySetContainer::~FuzzySetContainer()
{
    freeContainer();
}

FuzzySetContainer &FuzzySetContainer::operator =(const FuzzySetContainer &rhs)
{
    if(this != &rhs)
    {
        freeContainer();
        size_t size = rhs.sets_.size();
        sets_.resize(size, 0);
        for(int i = 0; i < (int)size; ++i)
        {
            sets_[i] = rhs.sets_[i]->getInstance();
            *(sets_[i]) = *(rhs.sets_[i]);
        }
    }
    return *this;
}

size_t FuzzySetContainer::size() const
{
    return sets_.size();
}

FuzzySet &FuzzySetContainer::operator [](int index)
{
    return *sets_[index];
}

const FuzzySet &FuzzySetContainer::operator [](int index) const
{
    return *sets_[index];
}

int FuzzySetContainer::insert(const FuzzySet &item, int position)
{
    FuzzySet *newItem = item.getInstance();
    (*newItem) = item;
    position = std::min(std::max(0, position), (int)sets_.size());
    std::vector<FuzzySet*>::iterator it = sets_.insert(sets_.begin() + position,
                                                       newItem);
    return it - sets_.begin();
}

void FuzzySetContainer::remove(int position)
{
    sets_.erase(sets_.begin() + position);
}

void FuzzySetContainer::freeContainer()
{
    for(int i = 0; i < (int)sets_.size(); ++i)
    {
        if(sets_[i])
        {
            delete sets_[i];
            sets_[i] = 0;
        }
    }
    sets_.clear();
}

