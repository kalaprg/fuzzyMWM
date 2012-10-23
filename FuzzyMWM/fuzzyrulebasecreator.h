#ifndef FUZZYRULEBASECREATOR_H
#define FUZZYRULEBASECREATOR_H
#include "fuzzyset.h"
#include "dataset.h"
#include "rulebase.h"

class FuzzyRuleBaseCreator
{
public:
    FuzzyRuleBaseCreator(const DataSet& dataSet);
    //not sure to allow one function per attribute or one function per division
    void setOptionsForAttribute(int attrID, int numberOfDivisions, bool regularDivision = true/*,const FuzzySet &functionClass*/);
    void setOptionsForClass(int numberOfDivisions, bool regularDivision = true/*,const FuzzySet &functionClass*/);
    void learnRules(RuleBase &outRuleBase) const;
};

#endif // FUZZYRULEBASECREATOR_H
