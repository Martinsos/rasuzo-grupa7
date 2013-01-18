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
    vector<pair<string, double> > averages;
    set<pair<double,string> >::iterator iter;

    cvtColor(img, temporaryBW, CV_BGR2GRAY);

    center=findCenter(temporaryBW);


    Mat testImage=adjustByCenter(temporaryBW, center);

    
    for(int i=0;i<(int)templateImages.size();i++){
        double match = matching(templateImages[i], testImage);
        values.push_back(matching(templateImages[i], testImage));
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
            
            averages.push_back(make_pair(classNames[closestImage],values[closestImage]));
            values[closestImage]=-1;
    }

    return averages;
}

void TemplateMatching::learn(map < string, vector<Mat> >& learningData, void* param)
{
    
    map < string, vector<Mat> >:: iterator classId;
    set<pair<double, string> > averages;
    CvPoint center;

    for(classId = learningData.begin(); classId != learningData.end();++classId)
    {
            
            for(int picture = 0; picture < (int) classId->second.size();++picture )
            {
                    classNames.push_back(classId->first);

                    Mat img = classId->second[picture];
                    Mat temporaryBW, temporary;
                                            
                    cvtColor(img, temporaryBW, CV_BGR2GRAY);
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
    Mat returnImage;    

    int range_x_up=center.x-485;

    int range_x_down=center.x+495;
    int range_y_up=center.y-195;
    int range_y_down=center.y+195;
    if(range_x_up<0){
            range_x_down+=abs(range_x_up);
            range_x_up=0;
    }else if(range_x_down>image.rows){
            range_x_up-=abs(image.rows - range_x_down);
            range_x_down=image.rows-1;
    }
    if(range_y_up<0){
            range_y_down+=abs(range_y_up);
            range_y_up=0;
    }else if(range_y_down>image.cols){
            range_y_up-=abs(image.cols - range_y_down);
            range_y_down=image.cols-1;
    }
    returnImage = image(Range(range_x_up,range_x_down),Range(range_y_up,range_y_down));

    return returnImage;
}
