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
