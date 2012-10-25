#ifndef FUZZYSETCONTAINER_H
#define FUZZYSETCONTAINER_H
#include <vector>
#include <cstring>

class FuzzySet;

class FuzzySetContainer
{
public:
    FuzzySetContainer() {}
    FuzzySetContainer(const FuzzySetContainer &c);
    ~FuzzySetContainer();
    FuzzySetContainer &operator=(const FuzzySetContainer &rhs);
    size_t size() const;
    FuzzySet &operator[](int index);
    const FuzzySet &operator[](int index) const;
    //inserts at position
    int insert(const FuzzySet &item, int position = -1);
    void remove(int position);
private:
    void freeContainer();
    std::vector<FuzzySet*> sets_;
};

#endif // FUZZYSETCONTAINER_H
