#ifndef FUZZYINFERENCEENGINE_H
#define FUZZYINFERENCEENGINE_H
#include <vector>
#include "rulebase.h"
#include "fuzzyset.h"
#include "dataset.h"

class FuzzyInferenceEngine
{
public:
    FuzzyInferenceEngine(const RuleBase &rule_base);
    void init();
    int inferClass(const std::vector<float> &input) const;
    float testPerformance(const DataSet &testDataSet) const;
private:
    template <class T>
    static float integrate(float left, float right, const T &function)
    {
        float integral = 0.0f;
        float x = left + delta_;
        while(x < right)
        {
            integral += 0.5f * delta_ * (function(x) + function(x - delta_));
            x += delta_;
        }
        x -= delta_;
        integral += 0.5f * (right - x) * (function(right) + function(x));
        return integral;
    }

    static const float delta_;

    class MembershipFunc
    {
    public:
        MembershipFunc(const FuzzySet &fuzzyset)
            : fuzzySet_(fuzzyset) {}
        float operator() (float x) const { return fuzzySet_.getMi(x); }
    private:
        const FuzzySet &fuzzySet_;
    };
    class MembershipXFunc
    {
    public:
        MembershipXFunc(const FuzzySet &fuzzyset)
            : fuzzySet_(fuzzyset) {}
        float operator() (float x) const { return x * fuzzySet_.getMi(x); }
    private:
        const FuzzySet &fuzzySet_;
    };

    RuleBase ruleBase_;
    std::vector<std::pair<float, float> > areasAndMassCenters_;
    bool initialised_;
};

#endif // FUZZYINFERENCEENGINE_H
