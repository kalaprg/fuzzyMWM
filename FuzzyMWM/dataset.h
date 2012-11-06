#ifndef DATASET_H
#define DATASET_H
#include <string>
#include <list>
#include <vector>

class DataSet
{
public:
    struct AtrributeInfo
    {
        std::string name;
        float min_value;
        float max_value;
    };

    struct DataInstance
    {
        DataInstance(const std::vector<float> &attributeValues = std::vector<float>(), int classID = -1)
            : attributeValues(attributeValues), classID(classID) {}
        std::vector<float> attributeValues;
        int classID;
    };
    DataSet(const std::string &filename);
    DataSet(const std::list<std::string> &attributeNames = std::list<std::string>());

    int getNumberOfAttributes() const;
    AtrributeInfo getAttributeInfo(int attrID) const;
    AtrributeInfo getClassAttributeInfo() const;
    int getNumberOfInstances() const;
    int addInstance(const DataInstance &instance);
    void getInstance(int instanceID, DataInstance &outInstance) const;
    void getTrainingAndTestSets(float percentage, DataSet &outTrainingData, DataSet &outTestData) const;

private:
    const DataInstance &getInstance(int instanceID) const;
    std::vector<DataSet::AtrributeInfo> attributes_;
    DataSet::AtrributeInfo classAttribute_;
    std::string title_;
    std::vector<DataSet::DataInstance> instances_;

};

#endif // DATASET_H
