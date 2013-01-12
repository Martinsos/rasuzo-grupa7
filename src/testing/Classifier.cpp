/** Implementation of Classifier
 */

#include "Classifier.hpp"

#include <iostream>
#include <fstream>
#include <set>

#include <cstdlib>

#ifdef WIN32
#include "StdAfx.h"
#include <Windows.h>
#include <tchar.h> 
#include <strsafe.h>
#else
#include <dirent.h>
#endif

// ------------------------------- Utility methods ---------------------------------- //

bool hasData(string line)
{
    return (line.size() > 0 && line[0] != '#' && line[0] != ' ');
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

bool inPredClasses(string real, vector< pair<string, double> > pred)
{
    for (int i = 0; i < pred.size(); i++)
        if (real == pred[i].first) return true;

    return false;
}

// ------------------------------- Class methods ---------------------------------- //

vector<string> Classifier::getFilesFromFolder(string folderName)
{
	vector<string> filenames;

#ifdef WIN32
	const int maxLength = 1024;
	TCHAR szDir[maxLength];
	StringCchCopy(szDir, maxLength, folderName.c_str());
	StringCchCat(szDir, maxLength, TEXT("\\*"));

	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(szDir, &ffd);
	
	if (INVALID_HANDLE_VALUE == hFind) {
		fprintf(stderr, "FindFirstFile error");
		return filenames;
	} 

	do {
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			string filename = ffd.cFileName;
			if (filename.find("jpg")) {
				filenames.push_back(ffd.cFileName);
			}
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
#else
	DIR *d;
	struct dirent* dirent;

	d = opendir(folderName.c_str());
	if (!d) {
		fprintf(stderr, "Invalid directory %s", folderName.c_str());
		return filenames;
	} else {
		while ((dirent = readdir(d)) != NULL) {
			if (isFile(dirent->d_name)) {
			        string filename(dirent->d_name);
			        filenames.push_back(filename);
			}
		}
		closedir(d);
	}
#endif

	return filenames;
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
            folderName = line;                  // Get folder name
            classId = getClassId(folderName);   // Get classId

            testImgsNames.clear();
            while (tcStream.good())             // Get names of test images
            {
                getline(tcStream, line);
                if(!hasData(line)) break;
                
                testImgsNames.insert(line);
            }
        }

        // Get pictures from folder
	string dirPath = pathToSil + folderName;
	const vector<string>& filenames = getFilesFromFolder(dirPath);
       
#ifdef WIN32

        dirPath += "\\";
#else
        dirPath += "/";

#endif

        for (int i = 0; i < (int) filenames.size(); i++) 
        {
            // Store image to corresponding set (test or learning)
            string imgName = filenames[i];
            string imgPath = dirPath + imgName;

            if (inSet(imgName, testImgsNames))
            {
                testData[classId].push_back( imread(imgPath, 1) );
            }
            else
            {
                learningData[classId].push_back( imread(imgPath, 1) );
            }
        }
    }

    // Everything went ok
    return 0;
}

int Classifier::countWrongs(int resNum) 
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
            vector< pair<string, double> > predClasses = classify(imgs[i], resNum);

            // Wrong if not equal to any predicted class
            if (!inPredClasses(realClassId, predClasses)) wrong++;
        }
    }
    return wrong;
}

int Classifier::test(string testConf, int resNum, string pathToSil)
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
    int wrong = countWrongs(resNum);
    return wrong;
}

// typedef map < string, map<string, int> > ConfusionMatrix;
Classifier::ConfusionMatrix Classifier::getConfusionMatrix()
{
    ConfusionMatrix ret;

    map< string, vector<Mat> >::iterator iter;
    for (iter = testData.begin(); iter != testData.end(); iter++)
    {
        string realClassId = iter->first;
        vector<Mat>& imgs = iter->second;

        for (int i = 0; i < imgs.size(); i++)
        {
            string predClassId = classify(imgs[i], 1)[0].first;
            ret[realClassId][predClassId] += 1;
        }
    }

    return ret; 
}
