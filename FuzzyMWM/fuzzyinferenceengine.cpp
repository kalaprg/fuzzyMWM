#include <cmath>
#include <stdexcept>
#include "fuzzyinferenceengine.h"
const float FuzzyInferenceEngine::delta_ = 0.0001;

FuzzyInferenceEngine::FuzzyInferenceEngine(const RuleBase &rule_base)
    : ruleBase_(rule_base), areasAndMassCenters_(rule_base.getNumberOfRules()),
      initialised_(false)
{
}

void FuzzyInferenceEngine::init()
{
    if(initialised_)
        return;

    initialised_ = true;

    int ruleNum = ruleBase_.getNumberOfRules();
    for(int i = 0; i < ruleNum; ++i)
    {
        float integral, integral2;
        float left, right;
        const RuleBase::Rule &rule = ruleBase_.getRule(i);
        const FuzzySet &conclusion = rule.getConclusion();
        conclusion.getSupportRange(left, right);
        integral = integrate(left, right, MembershipFunc(conclusion));
        integral2 = integrate(left, right, MembershipXFunc(conclusion));

        areasAndMassCenters_[i] = std::pair<float, float>(integral, integral2 / integral);
    }
}

int FuzzyInferenceEngine::inferClass(const std::vector<float> &input) const
{
    if(!initialised_)
        throw std::logic_error("Inference engine is not initialised!");

    int ruleNum = ruleBase_.getNumberOfRules();
    float nominator = 0.0f, denominator = 0.0f;
    for(int i = 0; i < ruleNum; ++i)
    {
        const RuleBase::Rule &rule = ruleBase_.getRule(i);
        const FuzzySetContainer &premises = rule.getPremises();
        float ruleSupport = 1.0f;
        int numPremises = premises.size();
        for(int k = 0; k < numPremises; ++k)
        {
            ruleSupport *= premises[k].getMi(input[k]);
        }
        float area = areasAndMassCenters_[i].first;
        float massCenter = areasAndMassCenters_[i].second;
        nominator += ruleSupport * area * massCenter;
        denominator += ruleSupport * area;
    }
    float defuzziefied = nominator / denominator;
    return round(defuzziefied);
}

float FuzzyInferenceEngine::testPerformance(const DataSet &testDataSet) const
{
    if(!initialised_)
        throw std::logic_error("Inference engine is not initialised!");

    int good = 0;
    int instNum = testDataSet.getNumberOfInstances();
    for(int i = 0; i < instNum; ++i)
    {
        DataSet::DataInstance inst;
        testDataSet.getInstance(i, inst);
        int outputClass = inferClass(inst.attributeValues);
        if(outputClass == inst.classID)
            ++good;
    }
    return (float) good / instNum;
}
