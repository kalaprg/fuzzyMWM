#include "dataset.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/assert.hpp>
#include <map>
#include <algorithm>

DataSet::DataSet(const std::string &filename)
    : title_("")
{
    enum ParsingMode{HEADER_START, TITLE, PARAMS, NAMES, DATA_ITEM};
    std::ifstream inputFile(filename.c_str());
    if(inputFile.is_open())
    {
        ParsingMode mode = HEADER_START;
        char separator = ' ';
        int lineNum = 0;
        int classAttrID = 0;
        int attributesNum = 0;
        int attributesRead = 0;
        bool EOF_expected = false;
        while(inputFile.good())
        {
            ++lineNum;
            std::string line;
            std::getline(inputFile, line);
            boost::algorithm::trim_left(line);
            if(!line.size() || line[0] == '#')
                continue;//skip comment or blank line

            size_t pos = line.find('#');
            if(pos != std::string::npos)
                line = line.substr(0, pos);//exclude comment

            boost::algorithm::trim_right(line);

            if(!line.size())
            {
                std::stringstream tmp;
                tmp << "L:" << lineNum << " ERROR: Unexpected blank line.";
                throw std::logic_error(tmp.str());
            }

            switch(mode)
            {
            case HEADER_START:
                if(line.size() > 1)
                {
                    std::stringstream tmp;
                    tmp << "L:" << lineNum << " ERROR: Only one separator character allowed";
                    throw std::runtime_error(tmp.str());
                }
                separator = line[0];
                mode = TITLE;
                break;
            case TITLE:
                if(line[0] == separator)
                    mode = PARAMS;
                else
                {
                    title_ += line;
                    title_ += "\n";
                }
                break;
            case PARAMS:
            {
                const int PARAMS_COUNT = 2;
                int params[PARAMS_COUNT];
                std::stringstream extractor(line);
                for(int i = 0; i < PARAMS_COUNT; ++i)
                {
                    if(extractor.good())
                    {
                        if(i)
                        {
                            char c = extractor.get();
                            if(c != separator)
                            {
                                std::stringstream tmp;
                                tmp << "L:" << lineNum << " ERROR: Separator expected.";
                                throw std::runtime_error(tmp.str());
                            }
                        }
                        extractor >> params[i];
                    }
                    else
                    {
                        std::stringstream tmp;
                        tmp << "L:" << lineNum << " ERROR: Syntax error.";
                        throw std::runtime_error(tmp.str());
                    }
                }
                attributesNum = params[0];
                classAttrID = params[1];

                mode = NAMES;
                attributes_.resize(attributesNum - 1);
                attributesRead = 0;
                break;
            }
            case NAMES:
                if(attributesRead == classAttrID)
                {
                    classAttribute_.name = line;
                    classAttribute_.min_value = std::numeric_limits<float>::max();
                    classAttribute_.max_value = -std::numeric_limits<float>::max();
                }
                else
                {
                    int trueAttrID = (attributesRead < classAttrID)
                            ? attributesRead : (attributesRead - classAttrID - 1);
                    attributes_[trueAttrID].name = line;
                    attributes_[trueAttrID].min_value = std::numeric_limits<float>::max();
                    attributes_[trueAttrID].max_value = -std::numeric_limits<float>::max();
                }
                if(++attributesRead == attributesNum)
                {
                    mode = DATA_ITEM;
                    instances_.clear();
                }
                break;
            case DATA_ITEM:
            {
                std::stringstream extractor(line);
                DataInstance inst;
                inst.attributeValues.resize(attributesNum - 1);
                for(int i = 0; i < attributesNum; ++i)
                {
                    if(extractor.good())
                    {
                        if(i)
                        {
                            char c = extractor.get();
                            if(c != separator)
                            {
                                std::stringstream tmp;
                                tmp << "L:" << lineNum << " ERROR: Separator expected.";
                                throw std::runtime_error(tmp.str());
                            }
                        }
                        if(i == classAttrID)
                        {
                            extractor >> inst.classID;
                            classAttribute_.min_value =
                                    std::min(classAttribute_.min_value, (float)inst.classID);
                            classAttribute_.max_value =
                                    std::max(classAttribute_.max_value, (float)inst.classID);
                        }
                        else
                        {
                            int trueAttrID = (i < classAttrID)
                                    ? i : (i - classAttrID - 1);
                            extractor >> inst.attributeValues[trueAttrID];
                            attributes_[trueAttrID].min_value =
                                    std::min(attributes_[trueAttrID].min_value,
                                             inst.attributeValues[trueAttrID]);
                            attributes_[trueAttrID].max_value =
                                    std::max(attributes_[trueAttrID].max_value,
                                             inst.attributeValues[trueAttrID]);
                        }
                    }
                    else
                    {
                        std::stringstream tmp;
                        tmp << "L:" << lineNum << " ERROR: Syntax error.";
                        throw std::runtime_error(tmp.str());
                    }
                }
                instances_.push_back(inst);
                EOF_expected = true;
                break;
            }
            }
        }
        if(inputFile.eof() && !EOF_expected)
        {
            std::stringstream tmp;
            tmp << "L:" << lineNum << " ERROR: Unexpected end of file.";
            throw std::runtime_error(tmp.str());
        }
    }
    else
    {
        std::stringstream tmp;
        tmp << "The file '" << filename << "' cannot be opened.";
        throw std::runtime_error(tmp.str());
    }
}

DataSet::DataSet(const std::list<std::string> &attributeNames)
{
    std::list<std::string>::const_iterator it;
    for(it = attributeNames.begin(); it != attributeNames.end(); ++it)
    {
        DataSet::AtrributeInfo info;
        info.name = *it;
        info.min_value = std::numeric_limits<float>::max();
        info.max_value = -std::numeric_limits<float>::max();
        attributes_.push_back(info);
    }
    title_ = "Default";
    classAttribute_.name = "Class";
    classAttribute_.min_value = std::numeric_limits<float>::max();
    classAttribute_.max_value = -std::numeric_limits<float>::max();
}

int DataSet::getNumberOfAttributes() const
{
    return attributes_.size();
}

DataSet::AtrributeInfo DataSet::getAttributeInfo(int attrID) const
{
    if(attrID > 0 && attrID < (int)attributes_.size())
    {
        return attributes_[attrID];
    }
    else
    {
        std::stringstream tmp;
        tmp << "Attribute ID \"" << attrID << "\" is out of range.";
        throw std::out_of_range(tmp.str());
    }
}

int DataSet::getNumberOfInstances() const
{
    return instances_.size();
}

int DataSet::getNumberOfClasses() const
{
    BOOST_ASSERT_MSG(false, "not implemented yet!");
    return -1;
}

int DataSet::addInstance(const std::vector<float> &attributes, int classID)
{
    if(attributes.size() != attributes_.size())
        throw std::logic_error("Wrong number of attributes");

    DataInstance inst(attributes, classID);
    instances_.push_back(inst);
    for(int i = 0; i < (int)attributes_.size(); ++i)
    {
        attributes_[i].min_value = std::min(attributes_[i].min_value, attributes[i]);
        attributes_[i].max_value = std::max(attributes_[i].max_value, attributes[i]);
    }
    return instances_.size() - 1;
}

void DataSet::getInstance(int instanceID, DataSet::DataInstance &outInstance) const
{
    if(instanceID < 0 || instanceID >= (int)instances_.size())
    {
        std::stringstream tmp;
        tmp << "Instance ID \"" << instanceID << "\" is out of range.";
        throw std::out_of_range(tmp.str());
    }

    outInstance = instances_[instanceID];
}

void DataSet::getTrainingAndTestSets(float percentage, DataSet &outTrainingData, DataSet &outTestData) const
{
    typedef std::map<int, int> Map;

    if(percentage <= 0.0f || percentage >= 1.0f)
    {
        std::stringstream tmp;
        tmp << "Percentage value \"" << percentage << "\" is out of range.";
        throw std::out_of_range(tmp.str());
    }

    outTestData.classAttribute_.name  = outTrainingData.classAttribute_.name
            = classAttribute_.name;
    outTestData.classAttribute_.max_value = outTrainingData.classAttribute_.max_value
            = - std::numeric_limits<float>::max();
    outTestData.classAttribute_.min_value = outTrainingData.classAttribute_.min_value
            = std::numeric_limits<float>::max();
    outTestData.title_ = outTrainingData.title_ = title_;
    outTestData.instances_.clear();
    outTrainingData.instances_.clear();
    outTestData.attributes_.clear();
    outTrainingData.attributes_.clear();

    for(int i = 0; i < (int) attributes_.size(); ++i)
    {
        DataSet::AtrributeInfo info;
        info.name = attributes_[i].name;
        info.min_value = std::numeric_limits<float>::max();
        info.max_value = -std::numeric_limits<float>::max();
        outTestData.attributes_.push_back(info);
        outTrainingData.attributes_.push_back(info);
    }

    Map classCounter;
    for(int i = 0; i < (int)instances_.size(); ++i)
    {
        int c = instances_[i].classID;
        Map::iterator it = classCounter.find(c);
        if(it == classCounter.end())
        {
            classCounter.insert(std::pair<int, int>(c, 1));
        }
        else
            ++(it->second);
    }

    for(Map::iterator it = classCounter.begin(); it != classCounter.end(); ++it)
        it->second *= percentage;

    std::vector<int> indices(instances_.size());
    for(int i = 0; i < indices.size(); ++i)
        indices[i] = i;

    std::random_shuffle(indices.begin(), indices.end());

    for(int i = 0; i < (int) indices.size(); ++i)
    {
        DataInstance inst;
       // if()
    }
}
