/** Implementation of Classifier
 */

#include "Classifier.hpp"

#include <iostream>
#include <fstream>

#include <cstdlib>
#include <dirent.h>

bool hasData(string line)
{
    return (line[0] != '#' && line[0] != ' ' && line.size() > 0);
}

string getClassId(string folderName)
{
    return folderName;

    //int idStart = folderName.find_first_of('l') + 1;
    //int idEnd   = folderName.find_first_of('_') - 1;

    //return atoi(folderName.substr(idStart, idEnd - idStart + 1).c_str());
}

bool isFile(string name)
{
    return (name != ".." && name != ".");
}

void Classifier::loadData(string testConf, string pathToSil)
{

    // Open file with testing configuration
    ifstream tcStream(testConf.c_str());

    string classId;
    string folderName;
    string testImgName;
    while (tcStream.good())
    {
        // Skip comments and empty lines
        string line;
        getline(tcStream, line);
        if (!hasData(line))
            continue;
        else
        {
            folderName = line;
            classId = getClassId(folderName);

            getline(tcStream, testImgName); // unaprijediti da moze vise
        }

        // Get pictures from folder
        DIR *Dir;
        struct dirent *DirEntry;
        string dirPath = pathToSil + folderName;

        Dir = opendir(dirPath.c_str());
        while(DirEntry = readdir(Dir))
        {
            if (!isFile(DirEntry->d_name))
                continue;

            // Store image to corresponding set (test or learning)
            string imgName(DirEntry->d_name);
            if (imgName == testImgName)
            {
                testData[classId].push_back( imread(imgName, 1) );
            }
            else
            {
                learningData[classId].push_back( imread(imgName, 1) );
            }
        }
        cout << endl;
    }
}

int Classifier::test(string testConf)
{

    int wrong = 0;
    
    // Izvrti preko testData i izbroji krive
    map< string, vector<Mat> >::iterator iter;
    for (iter = testData.begin(); iter != testData.end(); iter++)
    {
        string realClassId = iter->first;
        vector<Mat>& imgs = iter->second;

        for (int i = 0; i < imgs.size(); i++)
        {
            string predClassId = classify(imgs[i]);
            if (realClassId != predClassId) wrong ++;
        }
    }

    return wrong;
}
