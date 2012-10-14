#ifndef DATASET_H
#define DATASET_H
#include <QString>
#include <QList>
#include <QVector>

class DataSet
{
public:
    struct AtrributeInfo
    {
        QString name;
        float min_value;
        float max_value;
    };

    struct DataInstance
    {
        QVector<float> &attributeValues;
        int classID;
    };

    DataSet(QString &filename);
    DataSet(int numOfInfoAttributes, const QList<QString> &attributeNames);
    int getNumberOfAttributes() const;
    AtrributeInfo getAttributeInfo(int attrID) const;
    int getNumberOfInstances() const;
    int getNumberOfClasses() const;
    int addInstance(const QVector<float> &attributes, int classID);
    void getInstance(int instanceID, DataInstance &outInstance) const;
    void getTrainingAndTestSets(float percentage, DataSet &outTrainingData, DataSet &outTestData) const;
    //optional
    float getInterClassVariance();
    float getInsideClassVariance();
};

#endif // DATASET_H
