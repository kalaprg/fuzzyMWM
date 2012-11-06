#include <sstream>
#include "rulebase.h"


void RuleBase::addRule(const RuleBase::Rule &rule)
{
    rules_.push_back(rule);
}

const RuleBase::Rule &RuleBase::getRule(int ruleID) const
{
    return rules_[ruleID];
}

size_t RuleBase::getNumberOfRules() const
{
    return rules_.size();
}

std::string RuleBase::Rule::toString() const
{
    std::stringstream stream;
    stream << "IF";
    for(int i = 0; i < (int) premises_.size(); ++i)
    {
        stream << " " << premises_.getFuzzySet(i).toString();
    }
    stream << " THEN " << conclusion_->toString();
    return stream.str();
}
