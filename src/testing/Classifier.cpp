/** Implementation of Classifier
 */
#include "stdafx.h"

#include "Classifier.hpp"

#include <iostream>
#include <fstream>
#include <set>

#include <cstdlib>

#ifdef WIN32
#include <Windows.h>
#include <tchar.h> 
#include <strsafe.h>
#else
#include <dirent.h>
#endif

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
				std::string filename(dirent->d_name);
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
    string testImgName;
    while (tcStream.good())
    {
        // Skip comments and empty lines
        string line;
        getline(tcStream, line);
        if (!hasData(line)) {
            continue;
		} else {
            folderName = line;
            classId = getClassId(folderName);

            getline(tcStream, testImgName); // unaprijediti da moze vise
        }

        // Get pictures from folder
		string dirPath = pathToSil + folderName;

		const vector<string>& filenames = Classifier::getFilesFromFolder(pathToSil);
       
        for (int i = 0; i < (int) filenames.size(); i++) {

            // Store image to corresponding set (test or learning)
            string imgName = filenames[i];

			printf("Image %d name: %s\n", i, imgName.c_str());

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

            if (inPredClasses(realClassId, predClasses)) wrong ++;
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
