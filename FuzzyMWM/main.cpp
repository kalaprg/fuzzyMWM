#include <QCoreApplication>
#include <QTimer>
#include <list>
#include <string>
#include "dataset.h"

float randFloat(float a, float b)
{
    return ((b - a) * ((float)rand()/RAND_MAX)) + a;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DataSet sample("../sample_datasets/wine");

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
            test.addInstance(attrs, 0);
        }
        if(i % 4 == 1)
        {
            //banan: kolor [0.5;1.1] kwaskowatość[0.0 0.1]
            std::vector<float> attrs(2);
            attrs[0] = randFloat(.5f, 1.1f);
            attrs[1] = randFloat(0.0f, 0.1f);
            test.addInstance(attrs, 1);
        }
        if(i % 4 == 2)
        {
            //kiwi: kolor [1.5;2.0] kwaskowatość[0.3;0.8]
            std::vector<float> attrs(2);
            attrs[0] = randFloat(1.5f, 2.0f);
            attrs[1] = randFloat(.3f, .8f);
            test.addInstance(attrs, 2);
        }
        if(i % 4 == 3)
        {
            //ananas: kolor [0.0;0.8] kwaskowatość[0.3 0.9]
            std::vector<float> attrs(2);
            attrs[0] = randFloat(0.0f, 0.8f);
            attrs[1] = randFloat(.3f, .9f);
            test.addInstance(attrs, 3);
        }
    }
    
    QTimer::singleShot(0, &a, SLOT(quit()));
    return a.exec();
}
