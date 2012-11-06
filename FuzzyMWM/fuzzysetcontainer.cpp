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

FuzzySetContainer::FuzzySetContainer(const size_t size)
{
    sets_.resize(size, 0);
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

void FuzzySetContainer::resize(size_t newSize, const FuzzySet &value)
{
    resize(newSize, &value);
}

void FuzzySetContainer::resize(size_t newSize)
{
    resize(newSize, 0);
}

const FuzzySet &FuzzySetContainer::getFuzzySet(int index) const
{
    return *sets_[index];
}

void FuzzySetContainer::setFuzzySet(int index, FuzzySet *fset)
{
    if(sets_[index])
    {
        delete sets_[index];
        sets_[index] = 0;
    }
    sets_[index] = fset;
}

void FuzzySetContainer::setFuzzySet(int index, const FuzzySet &fset)
{
    if(sets_[index])
    {
        delete sets_[index];
        sets_[index] = 0;
    }
    sets_[index] = fset.getInstance();
    *sets_[index] = fset;
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

int FuzzySetContainer::insert(FuzzySet *item, int position)
{
    position = std::min(std::max(0, position), (int)sets_.size());
    std::vector<FuzzySet*>::iterator it = sets_.insert(sets_.begin() + position,
                                                       item);
    return it - sets_.begin();
}

void FuzzySetContainer::remove(int position)
{
    delete *(sets_.begin() + position);
    sets_.erase(sets_.begin() + position);
}

void FuzzySetContainer::resize(size_t newSize, const FuzzySet *value)
{
    size_t oldSize = sets_.size();
    if(oldSize > newSize)
    {
        for(std::vector<FuzzySet*>::iterator it = sets_.begin() + newSize;
            it != sets_.end(); ++it)
        {
            if(*it)
                delete *it;
        }
    }
    sets_.resize(newSize, 0);
    if(oldSize < newSize)
    {
        for(std::vector<FuzzySet*>::iterator it = sets_.begin() + oldSize;
            it != sets_.end(); ++it)
        {
            if(value)
                *it = value->getInstance();
            else
                *it = 0;
        }
    }
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

