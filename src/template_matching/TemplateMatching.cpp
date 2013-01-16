
#include "TemplateMatching.hpp"

using namespace std;
using namespace cv;

vector< pair<string, double> > TemplateMatching::classify(Mat img, int resNum)
{

	vector< pair < string,double> > results;

	string className;
	set <string> usedNames;
	Mat temporaryBW, temporary, testImage;
	vector<double> values;
	int closestImage;
	CvPoint center;
	vector<pair<string, double> > averages;
	set<pair<double,string> >::iterator iter;

        cvtColor(img, temporaryBW, CV_BGR2GRAY);

        center=findCenter(temporaryBW);
        testImage=adjustByCenter(temporaryBW, center);

        cout << "skup slika: " << templateImages.size() << endl;
        
	for(int i=0;i<(int)templateImages.size();i++){

                double match = matching(templateImages[i], testImage);
                cout << match << endl;

		values.push_back(matching(templateImages[i], testImage));
	}
        
        cout << "velicina rezultata: " << values.size() << endl;
	
	double max=0;
	for(int k=0;k<resNum;++k)
	{
		max=0;
		for(int i=0;i<(int)values.size();++i)   
		{
			if(values.at(i)>max)
			{
				max=values.at(i);
				closestImage=i;
			}
		}
                // cout << "ubacujem: " << values[closestImage] << " " << classNames[closestImage] << endl;
		averages.push_back(make_pair(classNames[closestImage],values[closestImage]));
		values[closestImage]=-1;

                cout << "idx maximalnog je: " << closestImage << endl;
	}
        
        /*
        iter = averages.begin();
        // cout << "Velicina averagesa: " << averages.size() << endl;
	while ((int)results.size() < resNum)
	{
		className = iter->second;
		if (usedNames.find(className) == usedNames.end())
		{
			results.push_back(make_pair(className, iter->first));
			usedNames.insert(className);
		}
		++iter;
	}
        */
        

        //results.push_back(make_pair("sil0_ivana", 0.97));
        return averages;
}

void TemplateMatching::learn(map < string, vector<Mat> >& learningData, void* param)
{
        
	map < string, vector<Mat> >:: iterator classId;
	set<pair<double, string> > averages;
	CvPoint center;

	for(classId = learningData.begin(); classId != learningData.end();++classId)
	{
                cout << "Ucim na: " << classId->first << endl;
		for(int picture = 0; picture < (int) classId->second.size();++picture )
		{
                        classNames.push_back(classId->first);
			
                        Mat img = classId->second[picture];
	                Mat temporaryBW, temporary;



                        // img = img.t();
			cvtColor(img, temporaryBW, CV_BGR2GRAY);

			center=findCenter(temporaryBW);
			temporary=adjustByCenter(temporaryBW,center);
			
			templateImages.push_back(temporary);
		}
	}
        
        // cout << "jesu li isti: " << templateImages[0] == templateImages[30] << endl;
        cout << "ucenje proslo!" << endl;
}

double TemplateMatching::matching(Mat image, Mat testImage)
{
	int rows=image.rows, cols=image.cols;
	double countMatch=0;
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		  {
			 if(image.at<uchar>(i,j)==testImage.at<uchar>(i,j))
				countMatch++;
		  }
	}
   return countMatch/(rows*cols);
}

CvPoint TemplateMatching::findCenter(Mat image)
{
	int sum_x=0,sum_y=0,n=0;
	int x,y;
	for (int i=0;i<image.rows;++i){
		for(int j=0;j<image.cols;++j){
			if(image.at<uchar>(i,j)!=0){
				sum_x+=i;
				sum_y+=j;
				n++;
			}
		}
	}
	x=sum_x/n; y=sum_y/n;

	return cvPoint(x,y);
}

Mat TemplateMatching::adjustByCenter(Mat image, CvPoint center)
{
        // return image(Range(center.x-485,center.x+495),Range(center.y-195,center.y+195));
        return image;
}
