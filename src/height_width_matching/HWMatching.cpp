#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<queue>
#include<vector>
#include<algorithm>
#include<queue>
#include<utility>
#include "HWMatching.hpp"

using namespace std;
using namespace cv;

void getFeatures(Mat&);
void printUsage();
void printFeatureValues();
int getHeadWidth();
int getHeadHeight();
int getMaxHeight();
int getMaxWidth();

double sqr( double x ) {
  return x*x;
}

Mat a;
vector <int> heights;
vector <int> widths;
int maxWidth, maxHeight; //though it can be extracted from heights&widths
int headHeight;

struct Example {
  // featureName -> featureValue
  map< string, double > features;

  string name;
};

#define K_HEAD_HEIGHT "k_head_height"
#define K_HEAD_WIDTH "k_head_width"
#define K_MAX_HEIGHT "k_max_height"
#define K_MAX_WIDTH "k_max_width"

vector <Example> learningSet;

vector< pair<string, double> > HWMatching::classify(Mat img, int resNum) {

  vector< pair<string, double> > ret;

  Mat testMat = img.t();
  Example testExample;
  getFeatures( testMat );
  testExample.features[ K_HEAD_HEIGHT ] = getHeadHeight();
  testExample.features[ K_HEAD_WIDTH ] = getHeadWidth();
  testExample.features[ K_MAX_HEIGHT ] = getMaxHeight();
  testExample.features[ K_MAX_WIDTH ] = getMaxWidth();
    
  vector < pair<double, string> > error;
  for( int i = 0; i < learningSet.size(); i++ ) {
    double errorSum = 0.0;

    errorSum += sqr( testExample.features[ K_MAX_HEIGHT ] -
                     learningSet[ i ].features[ K_MAX_HEIGHT ] );
    errorSum += sqr( testExample.features[ K_MAX_WIDTH ] - 
                     learningSet[ i ].features[ K_MAX_WIDTH ] );
    
    errorSum += sqr( testExample.features[ K_HEAD_HEIGHT ] -
                     learningSet[ i ].features[ K_HEAD_HEIGHT ] );
    errorSum += sqr( testExample.features[ K_HEAD_WIDTH ] - 
                     learningSet[ i ].features[ K_HEAD_WIDTH ] );
    
    error.push_back( make_pair( errorSum, learningSet[ i ].name ) );
  }

  sort( error.begin(), error.end() );

  for( int i = 0; i < error.size(); i++ ) {
    ret.push_back( make_pair( error[ i ].second, error[ i ].first ) );
  } 


  return ret;
}


void HWMatching::learn(map< string, vector<Mat> >& learningData) {

  map< string, vector<Mat> >::iterator iter;

  for (iter = learningData.begin(); iter != learningData.end(); iter++) {
    string realClassId = iter->first;
    vector<Mat>& imgs = iter->second;

    for( int i = 0; i < imgs.size(); i++ ) {
      a = imgs[ i ];
      a = a.t();

      getFeatures( a );
      
      Example current;
      current.name = realClassId;

      current.features[ K_HEAD_HEIGHT ] = getHeadHeight();
      current.features[ K_HEAD_WIDTH ] = getHeadWidth();
      current.features[ K_MAX_HEIGHT ] = getMaxHeight();
      current.features[ K_MAX_WIDTH ] = getMaxWidth(); 

      learningSet.push_back( current );

    }
  }

}

/*int main(int argc, char **argv) {

  if( argc < 2 ) {
    printUsage();
    return 1;
  }

  a = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  a = a.t();

  getFeatures( a );

  printFeatureValues();

  cout << "Head width: " << getHeadWidth() << endl;
  cout << "Head height: " << getHeadHeight() << endl;
  cout << getMaxWidth() << endl;
  cout << getMaxHeight() << endl;

  return 0;

}*/

void printUsage() {
  cout << "Usage: ./progname <path_to_siluethe>";
}

void getFeatures(Mat& a) {

  vector <int> rowFirstWhite( a.rows, -1 ); 
  // appearance of first white pxl in row at []
  vector <int> rowLastWhite( a.rows, -1 ); 
  // appearance of last white pxl in row at []
  vector <int> colFirstWhite( a.cols, -1 ); 
  // appearance of first white pxl in col at []
  vector <int> colLastWhite( a.cols, -1 ); 
  // appearance of last white pxl in col at []
 
  for( int i = 0; i < a.rows; i++ ) {
    for( int j = 0; j < a.cols; j++ ) {
      int pxlValue = (int)( a.at<uchar>(i, j) );
      if( pxlValue > 128 ) { //white or nuance of white
        
        if( rowFirstWhite[ i ] < 0 ) {
          rowFirstWhite[ i ] = j;
        }
        rowLastWhite[ i ] = j;

        if( colFirstWhite[ j ] < 0 ) {
          colFirstWhite[ j ] = i;
        }
        if( i > colLastWhite[ j ] ) {
          colLastWhite[ j ] = i;
        }
	
      }
    }
  }

  maxHeight = 0;
  heights.clear();
  for( int i = 0; i < rowFirstWhite.size(); i++ ) {
    int height = rowLastWhite[ i ] - rowFirstWhite[ i ];
    if( height ) {
      heights.push_back( height ); 
    }
    if( height > maxHeight ) {
      maxHeight = height;
    }
  }  

  maxWidth = 0;
  widths.clear();
  for( int i = 0; i < colFirstWhite.size(); i++ ) {
    int width = colLastWhite[ i ] - colFirstWhite[ i ];
    if( width ) { 
      widths.push_back( width );
    }
    if( width > maxWidth ) {
      maxWidth = width;
    }
  }  

}

void printFeatureValues() {
  cout << "Heights of white columns of the picture: " << endl;
  for( int i = 0; i < heights.size(); i++ ) {
    cout << heights[ i ] << " ";
  }
  cout << endl;
 
  cout << "Widths of white rows of the picture: " << endl;
  for( int i = 0; i < widths.size(); i++ ) {
    cout << widths[ i ] << " ";
  }
  cout << endl;

  cout << "Max height: " << maxHeight << endl;
  cout << "Max width: " << maxWidth << endl;
  cout << "Head height: " << getHeadHeight << endl;
  cout << "Head width" << getHeadWidth << endl;
  cout << endl;
}

int getHeadWidth() {

  headHeight = -1;

  if( widths.size() == 0 ) {
    cout << "getFeatures wasn't called first or no\
             widths extracted from silhouette" << endl;
    return -1;
  }

  int i = 0;
  int maxHeadWidth = 10;
  while( widths[ i + 1 ] >= widths[ i ] || 
         abs( widths[ i + 1 ] - maxHeadWidth ) < (int)( maxHeadWidth * 0.03 )  ) {
    if( widths[ i + 1 ] > widths[ i ] ) {
      maxHeadWidth = widths[ i + 1 ];
    }
    i++;
  }

  float tolerance = 0.2;

  int limit = maxHeadWidth;
  for( ; i < widths.size() ; ) {
    if( widths[ i ] < (int)(limit * (1. - tolerance)) ) {
      break;
    } else if( widths[ i ] > maxHeadWidth ) {
      if( widths[ i ] > (int)(limit * (1. + tolerance)) ) {
        break;
      }
      maxHeadWidth = widths[ i ];
    }
    i++;
  }  

  headHeight = i;

  return maxHeadWidth;
}

int getHeadHeight() {
  return headHeight;
}

int getMaxHeight() {
   
  if( heights.size() == 0 ) {
    cout << "getFeatures wasn't called first or no\
             heights extracted from silhouette" << endl;
    return -1;
  }
  return maxHeight;
}

int getMaxWidth() {
   
  if( widths.size() == 0 ) {
    cout << "getFeatures wasn't called first or no\
             widths extracted from silhouette" << endl;
    return -1;
  }
  return maxWidth;
}

