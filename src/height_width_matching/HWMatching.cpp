#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<queue>
#include<vector>
#include<algorithm>
#include<queue>
#include<utility>

using namespace std;
using namespace cv;

void getFeatures(Mat&);
void printUsage();
void printFeatureValues();

Mat a;
vector <int> heights;
vector <int> widths;
int maxWidth, maxHeight; //though it can be extracted from heights&widths

int main(int argc, char **argv) {

  if( argc < 2 ) {
    printUsage();
    return 1;
  }

  a = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  getFeatures( a );

  printFeatureValues();

  return 0;

}

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
  for( int i = 0; i < heights.size(); i++ ) {
    cout << heights[ i ] << " ";
  }
  cout << endl;

  cout << "Max height: " << maxHeight << endl;
  cout << "Max width: " << maxWidth << endl;
  cout << endl;
}
