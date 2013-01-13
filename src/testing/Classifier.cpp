/** Implementation of Classifier
 */

#include "Classifier.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
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

string intToStr(int a)
{
    stringstream ss;
    ss << a;

    return ss.str();
}

string doubleToStr(double a)
{
    stringstream ss;
    ss << a;

    return ss.str();
}

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
        fprintf (stderr, "Could not open %s\n", testConf.c_str());
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

int Classifier::test(string testConf, int resNum, string pathToSil, string reportPath)
{
    // Load data
    int status = loadData(testConf, pathToSil);
    if (status != 0)
    {
        printf ("Error occured while loading data!\n");
        return -1;
    }

    // Learn classifier
    learn(learningData);

    // Acquire statistics
    int wrong = 0;                      // Number of wrong classifications (in best resNum)
    int wrongStrict = 0;                // Number of wrong classifs (just the best)

    int total = 0;                      // Total number of test cases
    ConfusionMatrix confusionMat;
    ClassifResults classifResults;

    map< string, vector<Mat> >::iterator iter;
    for (iter = testData.begin(); iter != testData.end(); iter++)
    {
        string realClassId = iter->first;
        vector<Mat>& imgs = iter->second;

        for (int i = 0; i < imgs.size(); i++, total++)
        {
            // Get classifier output for single test image
            vector< pair<string, double> > predClasses = classify(imgs[i], resNum);
            // Save output for report generation later;
            classifResults.push_back( make_pair(realClassId, predClasses) );

            // Wrong if not equal to any of predicted classes
            if (!inPredClasses(realClassId, predClasses)) wrong++;

            // Update confusion matrix - take only best match
            string predClassId = predClasses[0].first;
            confusionMat[predClassId][realClassId] += 1;

            if (predClassId != realClassId) wrongStrict++;
        }
    }

    // -------------------------- Report generation -------------------------- //
    
    status = generateReport(confusionMat, wrong, total, reportPath, resNum, classifResults, wrongStrict);
    if (status != 0)
    {
        printf ("Error occured while generating report!\n");
        return -1;
    }

    return wrong;
}

vector<string> Classifier::getClassIDs()
{
    vector<string> keys;

    map< string, vector<Mat> >::iterator iter;
    for (iter = learningData.begin(); iter != learningData.end(); iter++)
    {
        keys.push_back(iter->first);
    }

    return keys;
}

int Classifier::generateReport(ConfusionMatrix& confusionMat, int wrong, int total, string repPath,
                               int resNum, ClassifResults& clRes, int wrongStrict)
{
    // Open file to store report
    ofstream reportFile(repPath.c_str());
    if (!reportFile.is_open())
    {
       fprintf(stderr, "Cannot open file %s to store report", repPath.c_str());
       return 1;
    }

    // Store confusion matrix as HTML table
    reportFile << confusionMatrixToHTML(confusionMat) << "\n<br>\n";
    

    // Store correctness (only the best)
    int correctStrict = total - wrongStrict;
    reportFile << "Correctness " <<  "(only the best): " 
               << correctStrict << "/" << total << " = ";
    reportFile << "<strong>" << correctStrict / (double) total << "</strong>" << "<br><br>\n\n";

    // Store correctness
    int correct = total - wrong;
    reportFile << "Correctness " <<  "(in <strong>" << resNum << "</strong> best): " 
               << correct << "/" << total << " = ";
    reportFile << "<strong>" << correct / (double) total << "</strong>" << "<br><br>\n\n";

    // Store classification results
    reportFile << classifResultsToHTML(clRes, resNum);

    reportFile.close();

    // Everything went ok
    return 0;
}

string Classifier::confusionMatrixToHTML(ConfusionMatrix& confusionMat)
{
    string HTMLrep = "";

    // Get keys (classIDs)
    vector<string> classIDs = getClassIDs();

    // ---------------------------------- Generate HTML ------------------------------- //

    // Print header
    HTMLrep += "<table border=\"1\" cellpadding=\"5\" style=\"border-collapse:collapse\">\n";

    HTMLrep += "<tr>\n";

        HTMLrep += "\t<th>Pred / Real</th>\n";    
        for (int i = 0; i < classIDs.size(); i++)
            HTMLrep += "\t<th>" + classIDs[i] + "</th>\n";

    HTMLrep += "</tr>\n";

    // Print table content
    for (int predIdx = 0; predIdx < classIDs.size(); predIdx++)
    {
        HTMLrep += "<tr>\n";
            HTMLrep += "\t<th>" + classIDs[predIdx] + "</th>\n";
        
            // Write each value of row
            for (int realIdx = 0; realIdx < classIDs.size(); realIdx++)
            {
                // Get keys for confusion matrix
                string predName = classIDs[predIdx];
                string realName = classIDs[realIdx];
                
                // Convert int to string
                string val = intToStr(confusionMat[predName][realName]);

                // Print value
                string tdColor = "white";
                if (predIdx == realIdx)     // Mark diagonal elements 
                {
                    tdColor = "#85FF5C";                    // Bold         
                    val = "<strong>" + val + "</strong>";   // Color
                }

                HTMLrep += "\t<td align=\"center\" style=\"background-color:"+tdColor+"\"" + ">" + val + "</td>\n";
            }
        HTMLrep += "</tr>\n";
    }

    // Print closing stuff
    HTMLrep += "</table>";

    // Everything went ok
    return HTMLrep;
}


string Classifier::classifResultsToHTML(ClassifResults& clRes, int resNum)
{
    string HTMLrep = "";

    // Print header
    HTMLrep += "<table border=\"1\" cellpadding=\"5\" style=\"border-collapse:collapse\">\n";

    HTMLrep += "<tr>\n";

        HTMLrep += "\t<th>Img ID</th>\n";    
        HTMLrep += "\t<th>Class name</th>\n";

        for (int i = 0; i < resNum; i++)
            HTMLrep += "\t<th>" + intToStr(i + 1) + "</th>\n";

    HTMLrep += "</tr>\n";

    // Print table row for each result
    for (int resIdx = 0; resIdx < clRes.size(); resIdx++)
    {
        HTMLrep += "<tr>\n";

            // Print idx of image
            HTMLrep += "\t<th>" + intToStr(resIdx + 1) + "</th>\n";
            
            string className = clRes[resIdx].first;
            vector< pair<string, double> > results = clRes[resIdx].second;

            // Print class of image
            HTMLrep += "\t<td>" + className + "</td>\n";

            // Print results of classification
            for (int i = 0; i < results.size(); i++)
            {
                string predClassName = results[i].first;
                double predClassVal  = results[i].second;

                HTMLrep += "\t<td align=\"center\">" + predClassName + " : " +
                           "<strong>" + doubleToStr(predClassVal) + " </strong>" + " </td>\n";
                
            }
        
        HTMLrep += "</tr>\n";
    }

    return HTMLrep;
}
