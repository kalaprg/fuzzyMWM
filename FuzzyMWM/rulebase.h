#ifndef RULEBASE_H
#define RULEBASE_H
#include <QVector>
#include <fuzzyset.h>
#include <fuzzysetcontainer.h>

class RuleBase
{
public:
    RuleBase();
    void addRule(const FuzzySetContainer &premises, const FuzzySet &conclusion);
    void getRule(int ruleID, FuzzySetContainer &outPremises, FuzzySet &outConclusion) const;
    int getNumberOfRules() const;
    //optional
    //serialize

};

#endif // RULEBASE_H
