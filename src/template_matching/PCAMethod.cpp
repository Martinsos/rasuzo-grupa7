
#include "PCAMethod.hpp"

using namespace std;
using namespace cv;

vector<pair<string,double> >PCAMethod::classify(Mat image, int resNum)
{
	vector< pair < string,double> > results;
	set<pair<double, string> > averages;
	
	Mat featuresTemp, temp;
	Mat imageBW, imageRow=Mat::zeros(1,image.rows*image.cols,CV_64FC1), imageBWF;
	string className;
	
	set <string> usedNames;
	set<pair<double,string> >::iterator iter;
	
	CvPoint center;
	int closestImage;
	double sum;
	vector<double> distance;
	double min,max=0;

	cvtColor(image, imageBW, CV_BGR2GRAY);
	center = findCenter(imageBW);
	temp = adjustByCenter(imageBW,center);

	temp.convertTo(imageBWF,CV_64FC1);

	for(int i=0;i<imageBWF.rows;++i){
		for(int j=0;j<imageBWF.cols;++j){
			imageRow.at<double>(0,i*imageBWF.cols+j)=imageBWF.at<double>(i,j);
		}
	}
	
	featuresTemp=(eigevectors*imageRow.t()).t();
	
	for(int k=0;k<resNum;++k)
	{
		for(int i=0;i<features.rows;++i){
			sum=0;
			for(int j=0;j<features.cols;++j){
					sum+=pow((features.at<double>(i,j)-featuresTemp.at<double>(0,j)),2);
			}
			if(i==0){
				min=sqrt((double)sum);
			}
			else{
				distance[i] = sqrt((double)sum);
				if(distance[i]<min){
					min=distance[i];
					closestImage=i;
				}
				else if(k==0)
					if(distance[i]>max)
					    max=distance[i];
			}
			
		}
		averages.insert(make_pair(distance[closestImage],classNames[closestImage]));
		distance[closestImage]=max;
	}

	iter = averages.begin();
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

	return results; 
}

void PCAMethod::learn(map<string, vector<Mat> >& learningData, void* param)
{
	map < string, vector<Mat> >:: iterator classId;
	Mat image,imageF,temporary,temporaryMean,centeredTemp;
	int numComponents =14;
	bool flag=true;
	CvPoint center;

	meanImage=Mat::zeros(learningData.begin()->second[0].rows,learningData.begin()->second[0].cols,CV_8UC1);
	
	cvtColor(learningData.begin()->second.at(0),temporary, CV_BGR2GRAY);
	center=findCenter(temporary);
	temporaryMean=adjustByCenter(temporary,center);
	
	int rows=temporaryMean.rows;
	int cols=temporaryMean.cols;

	for(int i=0;i<rows;++i){
		for(int j=0;j<cols;++j){
			for(classId = learningData.begin(); classId != learningData.end();++classId)
			{
				for(int picture = 0; picture < (int) classId->second.size();++picture ){
					cvtColor(classId->second.at(picture),temporary , CV_BGR2GRAY);
					center=findCenter(temporary);
					temporaryMean=adjustByCenter(temporary,center);

					if(flag && (temporaryMean.at<uchar>(i,j)==0)){
						flag=false;
					}
				}
				if(flag){
					meanImage.at<uchar>(i,j)=255;
				}
				flag=true;
			}
		}
	}

	Mat centeredMatrix;
	int row=0;
	for(classId = learningData.begin(); classId != learningData.end();++classId)
	{
		for(int picture = 0; picture < (int) classId->second.size();++picture )
		{
			classNames[(int) classId->second.size()] = classId->first;
			cvtColor(classId->second.at(picture),temporary,CV_BGR2GRAY);

			center=findCenter(temporary);
			image=adjustByCenter(temporary,center);
			
			centeredMatrix=image-meanImage;
			for(int i=0;i<centeredMatrix.rows;++i)
				for(int j=0;j<centeredMatrix.cols;++j)
					matrixRow.at<uchar>(row,i*centeredMatrix.cols+j)=centeredMatrix.at<uchar>(i,j);
			row++;
		}
	}

	matrixRow.convertTo(pcaMatrix,CV_64FC1);

	performPCA(numComponents);

}

void PCAMethod::performPCA(int numComponents)
{
	PCA pca(pcaMatrix, Mat(), CV_PCA_DATA_AS_ROW, numComponents);
 
	Mat mean = pca.mean.clone();
	Mat eigenvalues = pca.eigenvalues.clone();
	Mat eigenvectors = pca.eigenvectors.clone();

	Mat features = (eigenvectors*pcaMatrix.t()).t();
}

CvPoint PCAMethod::findCenter(Mat image)
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

Mat PCAMethod::adjustByCenter(Mat image, CvPoint center)
{
	return image(Range(center.x-485,center.x+495),Range(center.y-195,center.y+195));
}
