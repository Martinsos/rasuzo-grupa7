
#include "TemplateMatching.hpp"

using namespace std;
using namespace cv;

vector< pair<string, double> > TemplateMatching::classify(Mat img, int resNum)
{
	vector< pair < string,double> > results;
	string className;
	set <string> usedNames;
	Mat temporaryBW, temporary;
	vector<double> values;
	int closestImage;
	CvPoint center;
	set<pair<double, string> > averages;
	set<pair<double,string> >::iterator iter;
	for(int i=0;i<(int)templateImages.size();i++){
		cvtColor(templateImages[i],temporaryBW,CV_BGR2GRAY);

		center=findCenter(temporaryBW);
		temporary=adjustByCenter(temporaryBW,center);
		values.push_back(matching(temporary,img));
	}
	
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
		averages.insert(make_pair(values[closestImage],classNames[closestImage]));
		values[closestImage]=-1;
	}
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
}

void TemplateMatching::learn(map < string, vector<Mat> >& learningData, void* param)
{
	map < string, vector<Mat> >:: iterator classId;
	Mat temporaryBW, temporary;
	set<pair<double, string> > averages;
	CvPoint center;

	for(classId = learningData.begin(); classId != learningData.end();++classId)
	{
		classNames[(int) classId->second.size()] = classId->first;
		for(int picture = 0; picture < (int) classId->second.size();++picture )
		{
			
			cvtColor(classId->second.at(picture),temporaryBW,CV_BGR2GRAY);

			center=findCenter(temporaryBW);
			temporary=adjustByCenter(temporaryBW,center);
			
			templateImages.push_back(temporary);
			
		}
	}

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
	return image(Range(center.x-485,center.x+495),Range(center.y-195,center.y+195));
}
