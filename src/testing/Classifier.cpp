/** Implementation of Classifier
 */

#include "Classifier.hpp"

#include <iostream>
#include <fstream>
#include <set>

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

bool inSet(string s, set<string> words)
{
    return !(words.find(s) == words.end());
}

int Classifier::loadData(string testConf, string pathToSil)
{
    // Open file with testing configuration
    ifstream tcStream(testConf.c_str());
    if (!tcStream.is_open())
    {
        printf ("Could not open %s\n", testConf.c_str());
        return 1;
    }

    string classId;
    string folderName;
    set<string> testImgsNames;

    while (tcStream.good())
    {
        // Skip comments and empty lines
        string line;
        getline(tcStream, line);
        if (!hasData(line))
            continue;
        else
        {
            // Get folder name
            folderName = line;
            classId = getClassId(folderName);

            // Get testing images names
            while(tcStream.good())
            {
                getline(tcStream, line);
                if (!hasData(line))
                    break;

                testImgsNames.insert(line);
            }
        }

        cout << "Folder name: " << folderName << endl;
        cout << "classID: " << classId << endl;
        cout << "----" << endl;

        // Get pictures from folder
        DIR *Dir;
        struct dirent *DirEntry;
        string dirPath = pathToSil + folderName;

        Dir = opendir(dirPath.c_str());
        if (Dir == NULL)
        {
            printf ("Directory %s cannot be opened\n", dirPath.c_str());
            return 2;
        }

        while(DirEntry = readdir(Dir))
        {
            if (!isFile(DirEntry->d_name))
                continue;

            // Store image to corresponding set (test or learning)
            string imgName(DirEntry->d_name);
            if (inSet(imgName, testImgsNames))
            {
                testData[classId].push_back( imread(imgName, 1) );
                cout << "test: " << imgName << endl;
            }
            else
            {
                learningData[classId].push_back( imread(imgName, 1) );
                cout << "learn: " << imgName << endl;
            }
        }
        cout << "-------------------------------" << endl;
        cout << endl;

    }
    
    // Everything went ok
    return 0;
}

int Classifier::countWrongs()
{
    // Count wrongs
    int wrong = 0;
    
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

int Classifier::test(string testConf, string pathToSil)
{
    // Load data
    int status = loadData(testConf, pathToSil);
    if (status != 0)
    {
        printf ("Error occured!\n");
        return -1;
    }

    // Learn classifier
    learn(learningData);

    // Evaluate
    int wrong = countWrongs();
    return wrong;
}
