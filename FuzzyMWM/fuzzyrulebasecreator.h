#ifndef FUZZYRULEBASECREATOR_H
#define FUZZYRULEBASECREATOR_H
#include "fuzzyset.h"
#include "triangularfuzzyset.h"
#include "dataset.h"
#include "rulebase.h"
#include "dataset.h"

class FuzzyRuleBaseCreator
{
public:
    FuzzyRuleBaseCreator(const DataSet& dataSet);
    ~FuzzyRuleBaseCreator();
    void setFunctionClassForAttribute(int attrID,
                                const FuzzySet &functionClass);

    void setCustomPartitionForAttribute(int attrID,
                                const std::vector<float> &partitionPoints);
    void setRegularPartitionForAttribute(int attrID, int t);

    void setFunctionClassForClass(const FuzzySet &functionClass);

    void setRegularPartitionForClass(int t);

    void learnRules(RuleBase &outRuleBase);

private:
    const FuzzySet &bestSet(float value, const FuzzySetContainer &partitions) const;
    bool premisesMatch(const FuzzySetContainer &p1, const FuzzySetContainer &p2) const;
    void makeClassPartition(FuzzySetContainer &partitions);
    void makeRegularClassPartition(FuzzySetContainer &partitions);

    const DataSet &dataSet_;
    std::vector<std::vector<float> > partitionPoints_;
    std::vector<float> classPartitionPoints_;
    FuzzySetContainer functionClasses_;
    FuzzySet *classFunction_;
};

#endif // FUZZYRULEBASECREATOR_H
