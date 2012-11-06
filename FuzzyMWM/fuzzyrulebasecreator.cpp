#include <stdexcept>
#include <sstream>
#include <iostream>
#include <set>
#include "fuzzyrulebasecreator.h"


FuzzyRuleBaseCreator::FuzzyRuleBaseCreator(const DataSet &dataSet)
    : dataSet_(dataSet)
{
    classFunction_ = new TriangularFuzzySet();
    functionClasses_.resize(dataSet.getNumberOfAttributes(), TriangularFuzzySet());
    partitionPoints_.resize(dataSet.getNumberOfAttributes());
}

FuzzyRuleBaseCreator::~FuzzyRuleBaseCreator()
{
    if(classFunction_)
        delete classFunction_;
}

void FuzzyRuleBaseCreator::setFunctionClassForAttribute(int attrID, const FuzzySet &functionClass)
{
    if(attrID < 0 || attrID >= (int)dataSet_.getNumberOfAttributes())
    {
        std::stringstream tmp;
        tmp << "Attribute ID \"" << attrID << "\" is out of range.";
        throw std::out_of_range(tmp.str());
    }
    functionClasses_.setFuzzySet(attrID, functionClass);
}

void FuzzyRuleBaseCreator::setCustomPartitionForAttribute(int attrID, const std::vector<float> &partitionPoints)
{
    if(attrID < 0 || attrID >= (int)dataSet_.getNumberOfAttributes())
    {
        std::stringstream tmp;
        tmp << "Attribute ID \"" << attrID << "\" is out of range.";
        throw std::out_of_range(tmp.str());
    }
    partitionPoints_[attrID] = partitionPoints;
}

void FuzzyRuleBaseCreator::setRegularPartitionForAttribute(int attrID, int t)
{
    if(attrID < 0 || attrID >= (int)dataSet_.getNumberOfAttributes())
    {
        std::stringstream tmp;
        tmp << "Attribute ID \"" << attrID << "\" is out of range.";
        throw std::out_of_range(tmp.str());
    }
    DataSet::AtrributeInfo info(dataSet_.getAttributeInfo(attrID));
    float range = info.max_value - info.min_value;
    int n = t * 2 + 1;
    partitionPoints_[attrID].resize(n + 1);
    for(int i = 0; i < n; ++i)
    {
        partitionPoints_[attrID][i] = info.min_value +  i * range / n;
    }
    partitionPoints_[attrID][n] = info.max_value;
}

void FuzzyRuleBaseCreator::setFunctionClassForClass(const FuzzySet &functionClass)
{
    if(classFunction_)
    {
        delete classFunction_;
        classFunction_ = 0;
    }
    classFunction_ = functionClass.getInstance();
}

void FuzzyRuleBaseCreator::setRegularPartitionForClass(int t)
{
    DataSet::AtrributeInfo info(dataSet_.getClassAttributeInfo());
    float range = info.max_value - info.min_value;
    int n = t * 2 + 1;
    classPartitionPoints_.resize(n + 1);
    for(int i = 0; i < n; ++i)
    {
        classPartitionPoints_[i] = info.min_value +  i * range / n;
    }
    classPartitionPoints_[n] = info.max_value;
}

void FuzzyRuleBaseCreator::learnRules(RuleBase &outRuleBase)
{
    int attrNum = dataSet_.getNumberOfAttributes();
    outRuleBase = RuleBase();
    std::vector<FuzzySetContainer> partitions(attrNum);
    FuzzySetContainer classPartitions;

    for(int i = 0; i < attrNum; ++i)
    {
        if(!partitionPoints_[i].size())
            setRegularPartitionForAttribute(i, 2);

        int n = partitionPoints_[i].size() - 1;
        partitions[i].resize(n);

        float half = (partitionPoints_[i][0] + partitionPoints_[i][1]) * 0.5f;
        partitions[i].setFuzzySet(0, functionClasses_.getFuzzySet(i).getInstance(
                                      2 * partitionPoints_[i][0] - half,
                                      half,
                                      (partitionPoints_[i][1] + partitionPoints_[i][2]) * 0.5f));
        half = (partitionPoints_[i][n - 1] + partitionPoints_[i][n]) * 0.5f;
        partitions[i].setFuzzySet(n - 1, functionClasses_.getFuzzySet(i).getInstance(
                                      (partitionPoints_[i][n - 2] + partitionPoints_[i][n - 1]) * 0.5f,
                                      half,
                                      2 * partitionPoints_[i][n] - half));

        for(int j = 1; j < n - 1; ++j)
        {
            partitions[i].setFuzzySet(j, functionClasses_.getFuzzySet(i).getInstance(
                                          (partitionPoints_[i][j - 1] + partitionPoints_[i][j]) * 0.5f,
                                          (partitionPoints_[i][j] + partitionPoints_[i][j + 1]) * 0.5f,
                                          (partitionPoints_[i][j + 2] + partitionPoints_[i][j + 1]) * 0.5f));
        }
    }

//    makeClassPartition(classPartitions);
        makeRegularClassPartition(classPartitions);

    int instNum = dataSet_.getNumberOfInstances();
    std::vector<float> lambda(instNum);
    std::vector<RuleBase::Rule> rules(instNum);
    for(int i = 0; i < instNum; ++i)
    {
        DataSet::DataInstance inst;
        dataSet_.getInstance(i, inst);
        FuzzySetContainer premises(attrNum);
        float fit = 1.0f;


        for(int j = 0; j < attrNum; ++j)
        {
            float val = inst.attributeValues[j];
            premises.setFuzzySet(j, bestSet(val, partitions[j]));
            fit *= premises.getFuzzySet(j).getMi(val);
        }
        const FuzzySet &classSet = bestSet(inst.classID, classPartitions);
        rules[i] = RuleBase::Rule(premises, classSet);
        fit *= classSet.getMi(inst.classID);
        lambda[i] = fit;
    }

    //eliminate inconsistent rules (premises the same, different conclusion)
    for(std::vector<RuleBase::Rule>::iterator it = rules.begin();
        it != rules.end(); ++it)
    {
        const FuzzySetContainer &premises = it->getPremises();
        for(std::vector<RuleBase::Rule>::iterator it2 = it + 1;
            it2 != rules.end();)
        {
            if(premisesMatch(premises, it2->getPremises()))
            {
                if(!(it->getConclusion() == it2->getConclusion()))
                {
                    int i = it - rules.begin();
                    int j = it2 - rules.begin();

                    if(lambda[i] < lambda[j])
                    {
                        std::swap(*it, *it2);
                        std::swap(lambda[i], lambda[j]);
                    }
                    it2 = rules.erase(it2);
                    lambda.erase(lambda.begin() + j);
                }
                else
                    ++it2;
            }
            else
                ++it2;
        }
    }
    //eliminate identical rules
    lambda.clear();//lambda's are no longer needed, despite what book says :P
    for(std::vector<RuleBase::Rule>::iterator it = rules.begin();
        it != rules.end(); ++it)
    {
        const FuzzySetContainer &premises = it->getPremises();
        for(std::vector<RuleBase::Rule>::iterator it2 = it + 1;
            it2 != rules.end();)
        {
            if(premisesMatch(premises, it2->getPremises()))
            {
                it2 = rules.erase(it2);
            }
            else
                ++it2;
        }
        outRuleBase.addRule(*it);
    }
}

const FuzzySet &FuzzyRuleBaseCreator::bestSet(float value,
                                              const FuzzySetContainer &partitions) const
{
    int partNum = partitions.size();
    int minIndex = -1;
    float min = -1;
    for(int i = 0; i < partNum; ++i)
    {
        if(min < partitions.getFuzzySet(i).getMi(value))
        {
            minIndex = i;
            min = partitions.getFuzzySet(i).getMi(value);
        }
    }
    return partitions.getFuzzySet(minIndex);
}

bool FuzzyRuleBaseCreator::premisesMatch(const FuzzySetContainer &p1, const FuzzySetContainer &p2) const
{
    int p1Num = p1.size();
    int p2Num = p2.size();
    if(p1Num != p2Num)
        return false;

    for(int i = 0; i < p1Num; ++i)
    {
        if(!(p1.getFuzzySet(i) == p2.getFuzzySet(i)))
            return false;
    }
    return true;
}

void FuzzyRuleBaseCreator::makeClassPartition(FuzzySetContainer &partitions)
{
    const float delta = 0.1;
    partitions = FuzzySetContainer();
    std::set<int> classes;
    int n = dataSet_.getNumberOfInstances();
    for(int i = 0; i < n; ++i)
    {
        DataSet::DataInstance inst;
        dataSet_.getInstance(i, inst);
        classes.insert(inst.classID);
    }
    for(std::set<int>::iterator it = classes.begin(); it != classes.end(); ++it)
    {
        partitions.insert(classFunction_->getInstance(*it - delta, *it, *it + delta),
                          partitions.size());
    }
}

void FuzzyRuleBaseCreator::makeRegularClassPartition(FuzzySetContainer &partitions)
{
    int n = classPartitionPoints_.size() - 1;
    partitions.resize(n);

    float half = (classPartitionPoints_[0] + classPartitionPoints_[1]) * 0.5f;
    partitions.setFuzzySet(0, classFunction_->getInstance(
                                  2 * classPartitionPoints_[0] - half,
                                  half,
                                  (classPartitionPoints_[1] + classPartitionPoints_[2]) * 0.5f));
    half = (classPartitionPoints_[n - 1] + classPartitionPoints_[n]) * 0.5f;
    partitions.setFuzzySet(n - 1, classFunction_->getInstance(
                                  (classPartitionPoints_[n - 2] + classPartitionPoints_[n - 1]) * 0.5f,
                                  half,
                                  2 * classPartitionPoints_[n] - half));

    for(int j = 1; j < n - 1; ++j)
    {
        partitions.setFuzzySet(j, classFunction_->getInstance(
                                      (classPartitionPoints_[j - 1] + classPartitionPoints_[j]) * 0.5f,
                                      (classPartitionPoints_[j] + classPartitionPoints_[j + 1]) * 0.5f,
                                      (classPartitionPoints_[j + 2] + classPartitionPoints_[j + 1]) * 0.5f));
    }
}
