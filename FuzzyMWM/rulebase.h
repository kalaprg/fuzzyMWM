#ifndef RULEBASE_H
#define RULEBASE_H
#include <vector>
#include <fuzzyset.h>
#include <fuzzysetcontainer.h>
#include <cstring>
#include <QDebug>

class RuleBase
{
public:
    RuleBase() {}
    class Rule
    {
    public:
        Rule() : conclusion_(0) {}
        Rule(const FuzzySetContainer &premises, const FuzzySet &conclusion)
            : premises_(premises), conclusion_(conclusion.getInstance())
        {
            if(conclusion_)
            {
                *conclusion_ = conclusion;
            }
            else
                conclusion_ = 0;
        }

        Rule(const Rule &r)
        {
            premises_ = r.premises_;
            if(r.conclusion_)
            {
                conclusion_ = r.conclusion_->getInstance();
                *conclusion_ = *r.conclusion_;
            }
            else
                conclusion_ = 0;
        }

        ~Rule()
        {
            if(conclusion_)
            {
                delete conclusion_;
                conclusion_ = 0;
            }
        }

        Rule &operator = (const Rule &rhs)
        {
            if(this != &rhs)
            {
                if(conclusion_)
                    delete conclusion_;
                premises_ = rhs.premises_;
                conclusion_ = rhs.conclusion_->getInstance();
                *conclusion_ = *rhs.conclusion_;
            }
            return *this;
        }

        const FuzzySetContainer &getPremises() const { return premises_; }
        const FuzzySet &getConclusion() const { return *conclusion_; }
    private:
        FuzzySetContainer premises_;
        FuzzySet *conclusion_;
    };

    void addRule(const Rule &rule);
    const Rule &getRule(int ruleID) const;
    size_t getNumberOfRules() const;
    //optional
    //serialize
private:
    std::vector<Rule> rules_;

};

#endif // RULEBASE_H
