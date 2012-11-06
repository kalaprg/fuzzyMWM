#include <QCoreApplication>
#include <QTimer>
#include <ctime>
#include <list>
#include <string>
#include <iostream>
#include <limits>
#include "dataset.h"
#include "rulebase.h"
#include "fuzzyinferenceengine.h"
#include "triangularfuzzyset.h"
#include "fuzzyrulebasecreator.h"

float randFloat(float a, float b)
{
    return ((b - a) * ((float)rand()/RAND_MAX)) + a;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    srand(time(NULL));

    DataSet sample("../sample_datasets/wine");
    DataSet sample2("../sample_datasets/winequality_white");

    std::string attributeNames[] = {"kolor", "kwaskowatość"};
    std::list<std::string> attributes(attributeNames, attributeNames + sizeof(attributeNames) / sizeof(attributeNames[0]));
    DataSet test(attributes);
    //kolor: 0 żółty 1.2  1 zielony 2
    for(int i = 0; i < 256; ++i)
    {
        if(i % 4 == 0)
        {
            //słodkie zielone jabłko: kolor [1.0 1.5] kwaskowatość[0.1;0.4]
            std::vector<float> attrs(2);
            attrs[0] = randFloat(1.0f, 1.5f);
            attrs[1] = randFloat(.1f, .4f);
            test.addInstance(DataSet::DataInstance(attrs, 0));
        }
        if(i % 4 == 1)
        {
            //banan: kolor [0.5;1.1] kwaskowatość[0.0 0.1]
            std::vector<float> attrs(2);
            attrs[0] = randFloat(.5f, 1.1f);
            attrs[1] = randFloat(0.0f, 0.1f);
            test.addInstance(DataSet::DataInstance(attrs, 1));
        }
        if(i % 4 == 2)
        {
            //kiwi: kolor [1.5;2.0] kwaskowatość[0.3;0.8]
            std::vector<float> attrs(2);
            attrs[0] = randFloat(1.5f, 2.0f);
            attrs[1] = randFloat(.3f, .8f);
            test.addInstance(DataSet::DataInstance(attrs, 2));
        }
        if(i % 4 == 3)
        {
            //ananas: kolor [0.0;0.8] kwaskowatość[0.3 0.9]
            std::vector<float> attrs(2);
            attrs[0] = randFloat(0.0f, 0.8f);
            attrs[1] = randFloat(.3f, .9f);
            test.addInstance(DataSet::DataInstance(attrs, 3));
        }
    }
    
    DataSet trainingData, testData;
//    test.getTrainingAndTestSets(0.1, trainingData, testData);
//    sample.getTrainingAndTestSets(0.1, trainingData, testData);
    sample2.getTrainingAndTestSets(0.1, trainingData, testData);

    //    RuleBase testRuleBase;
    //    {//jabłko
    //        FuzzySetContainer premises;
    //        premises.insert(TriangularFuzzySet(1.0, 1.25, 1.5), 0);
    //        premises.insert(TriangularFuzzySet(1.0, 1.25, 1.5), 1);
    //        testRuleBase.addRule(RuleBase::Rule(premises, TriangularFuzzySet(-0.1,0,0.1)));
    //    }
    //    {//banan
    //        FuzzySetContainer premises;
    //        premises.insert(TriangularFuzzySet(0.5, 1.0, 1.1), 0);
    //        premises.insert(TriangularFuzzySet(0.0, 0.05, 0.1), 1);
    //        testRuleBase.addRule(RuleBase::Rule(premises, TriangularFuzzySet(0.9,1,1.1)));
    //    }
    //    {//kiwi
    //        FuzzySetContainer premises;
    //        premises.insert(TriangularFuzzySet(1.5, 1.75, 2.0), 0);
    //        premises.insert(TriangularFuzzySet(0.3, 0.5, 0.8), 1);
    //        testRuleBase.addRule(RuleBase::Rule(premises, TriangularFuzzySet(1.9,2,2.1)));
    //    }
    //    {//ananas
    //        FuzzySetContainer premises;
    //        premises.insert(TriangularFuzzySet(0.0, 0.6, 0.8), 0);
    //        premises.insert(TriangularFuzzySet(0.3, 0.6, 0.9), 1);
    //        testRuleBase.addRule(RuleBase::Rule(premises, TriangularFuzzySet(2.9,3,3.1)));
    //    }

    float perf;
    //    FuzzyInferenceEngine engine(testRuleBase);
    //    engine.init();
    //    perf = engine.testPerformance(trainingData);
    //    std::cout<<"Performance on training data: "<<perf*100.0f<<'%'<<std::endl;
    //    perf = engine.testPerformance(testData);
    //    std::cout<<"Performance on test data: "<<perf*100.0f<<'%'<<std::endl;

    FuzzyRuleBaseCreator creator(trainingData);

    for(int i = 0; i < trainingData.getNumberOfAttributes(); ++i)
    {
        creator.setRegularPartitionForAttribute(i, 4);
        creator.setFunctionClassForAttribute(i, TriangularFuzzySet());
    }
    creator.setFunctionClassForClass(TriangularFuzzySet());
    creator.setRegularPartitionForClass(4);
    RuleBase learnedBase;
    creator.learnRules(learnedBase);
    FuzzyInferenceEngine engine2(learnedBase);
    engine2.init();
    perf = engine2.testPerformance(trainingData);
    std::cout<<"Performance on training data: "<<perf*100.0f<<'%'<<std::endl;
    perf = engine2.testPerformance(testData);
    std::cout<<"Performance on test data: "<<perf*100.0f<<'%'<<std::endl;

    QTimer::singleShot(0, &a, SLOT(quit()));
    return a.exec();
}
