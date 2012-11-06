#ifndef FUZZYSETCONTAINER_H
#define FUZZYSETCONTAINER_H
#include <vector>
#include <cstring>

class FuzzySet;

class FuzzySetContainer
{
public:
    FuzzySetContainer() {}
    explicit FuzzySetContainer(const size_t size);
    FuzzySetContainer(const FuzzySetContainer &c);
    ~FuzzySetContainer();
    FuzzySetContainer &operator=(const FuzzySetContainer &rhs);
    size_t size() const;
    void resize(size_t newSize, const FuzzySet &value);
    void resize(size_t newSize);
    const FuzzySet &getFuzzySet(int index) const;
    void setFuzzySet(int index, FuzzySet *fset);
    void setFuzzySet(int index, const FuzzySet &fset);
    //inserts at position
    int insert(const FuzzySet &item, int position = -1);
    int insert(FuzzySet *item, int position = -1);
    void remove(int position);
private:
    void resize(size_t newSize, const FuzzySet *value);
    void freeContainer();
    std::vector<FuzzySet*> sets_;
};

#endif // FUZZYSETCONTAINER_H
