#ifndef FUZZYINFERENCEENGINE_H
#define FUZZYINFERENCEENGINE_H
#include "rulebase.h"
#include "fuzzyset.h"
#include "dataset.h"

class FuzzyInferenceEngine
{
public:
    FuzzyInferenceEngine(const RuleBase &rule_base);
    int inferClass(const QVector<float> &input) const;
    float testPerformance(const DataSet &testDataSet) const;
};

#endif // FUZZYINFERENCEENGINE_H
