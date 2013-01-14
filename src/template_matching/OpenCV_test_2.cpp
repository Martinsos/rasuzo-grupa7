#include "stdafx.h"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

template<class T> class Image
{
  private:
  IplImage* imgp;
  public:
  Image(IplImage* img=0) {imgp=img;}
  ~Image(){imgp=0;}
  void operator=(IplImage* img) {imgp=img;}
  inline T* operator[](const int rowIndx) {
    return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));}
};

typedef Image<unsigned char>  BwImage;

int PCA_metoda(vector<Mat> slike, Mat temp, int num_components, bool zastavica_za_PCA_svi_pikseli);

int templateMatching_metoda(vector<Mat> slike, Mat temp);

vector<Mat> podesavanje_po_centroidu(vector<Mat> slike);

double podudaranje(Mat image, Mat temp);

vector<Mat> ucitavanje(String slika_put,int broj_slika);

Mat srednja_slika(vector<Mat> slike);

CvPoint nadi_centroid(Mat slika);

double cross_correlation( IplImage* img1, IplImage* img2 );

int main(int argc, char** argv)
{
	Mat image, temp, image_gray, temp_gray;

	Mat image_roi_r, temp_roi_r,image_roi, temp_roi;

	vector<Mat> slike, slike_roi, slike_t;

	int najslicnija_slika=0; //broj naslicnije slike
	double max=0;

	vector<vector<float>> vrijednosti_pca;
	vector<double> vrijednosti;

	int broj_slika, broj_slika_usporedbe;//broj slika koji se ucitava
	int slika_usporedbe; //slika koje ce se usporedivati template
	int num_components;
	//putanja za sve ostale slike
	String images_path="D:\\FER\\Raspoznavanje uzoraka\\Projekt\\slike\\siluete\\slika";
	String template_path="D:\\FER\\Raspoznavanje uzoraka\\Projekt\\slike\\siluete\\slika_t";

	//ucitavanje slika u vector
	cout<<"upisi broj slika"<<endl;
	cin>>broj_slika;	
	slike=ucitavanje(images_path,broj_slika);

	//ucitavanje testnih slika u vector
	cout<<"upisi broj testnih slika"<<endl;
	cin>>broj_slika_usporedbe;	
	slike_t=ucitavanje(template_path,broj_slika_usporedbe);

	cout<<"upisi broj vektora koji se koristi za PCA"<<endl;
	cin>>num_components;

	for (int o=0; o<slike_t.size(); o++){
	temp=slike_t.at(o);
	najslicnija_slika=0;

	//najslicnija_slika=templateMatching_metoda(slike,temp);

	bool zastavica_za_PCA_svi_pikseli=true;		// ako je TRUE oznaèava da pretvaramo sve piksele slike u redak, 
												// ako je FALSE onda radimo zbroj bijelih piksela po redku slike i stavljamo u jednu matricu
	
	najslicnija_slika=PCA_metoda(slike,temp,num_components,zastavica_za_PCA_svi_pikseli);

	
	/*namedWindow("Template",CV_WINDOW_AUTOSIZE);
	imshow("Template",temp);
	namedWindow("Najslicnija slika",CV_WINDOW_AUTOSIZE);
	imshow("Najslicnija slika",slike[(najslicnija_slika-1)]);*/
	cout<<najslicnija_slika<<endl;
	}
	waitKey(0);
	return(0);
}

//ucitavanje svih silueta koje zelimo koristiti za usporedivanje
vector<Mat> ucitavanje(String slike_put,int broj_slika){
	String stalni_put=slike_put;
	ostringstream convert;
	String broj,put;
	Mat slika;
	vector<Mat> vector_slike;

	for(int i=1;i<=broj_slika;++i){
		put=stalni_put;
		convert.clear();
		convert.str(std::string());
		convert<<i;
		broj=convert.str();
		put+=broj;
		put+=".jpg";
		slika=imread(put,0);
		vector_slike.push_back(slika.t());
	}
	return vector_slike;
}
//funkcija koja izdvaja ROI po retcima

//funkcija koja gleda u koliko se tocaka podudaraju dvije siluete
double podudaranje(Mat image, Mat temp){
	double broj_podudaranja=0;
	int retci=image.rows, stupci=image.cols;

	bool zastavica_za_podudaranje=false; // ako je true onda podudaranje radi cross-korelacijom
	
	if (zastavica_za_podudaranje==true)	// radi cross-korelaciju
	{ IplImage* img_Ipl = new IplImage(image);
	  IplImage* temp_Ipl = new IplImage(temp);
	  return cross_correlation(img_Ipl,temp_Ipl);
	}
	
	else	// radi podudaranje broja piksela
	{
	 if(image.rows>temp.rows)
	  {
		retci=temp.rows;
	  }
	 if(image.cols>temp.cols)
	  {
		stupci=temp.cols;
	  }
	 for(int i=0;i<retci;i++)
		 for(int j=0;j<stupci;j++)
		  {
			 if(image.at<uchar>(i,j)==temp.at<uchar>(i,j))
				broj_podudaranja++;
		  }

      return broj_podudaranja/(retci*stupci);
	}
		
	
}

//funkcija koja raèuna cross korelaciju izmeðu dvije slike
double cross_correlation( IplImage* img1, IplImage* img2 )
{
	double corr;

	int M = img1->width;
	int N = img1->height;

	BwImage img_1( img1 ); // korištenje opencv omotaèa za pristupanje pikselima slike
	BwImage img_2( img2 );

	CvScalar img1_avg = cvAvg( img1, NULL ); //raèunanje prosjeènih vrijednosti slika
	CvScalar img2_avg = cvAvg( img2, NULL );

	double sum_img1_img2 = 0;
	double sum_img1_2 = 0;
	double sum_img2_2 = 0;

	for( int m=0; m<M; ++m )		// raèunanje cross korelacije
	 {
	  for( int n=0; n<N; ++n )
	   {
        sum_img1_img2   = sum_img1_img2 + (img_1[m][n]-img1_avg.val[0])*(img_2[m][n]-img2_avg.val[0]);  
        sum_img1_2      = sum_img1_2 + (img_1[m][n]-img1_avg.val[0])*(img_1[m][n]-img1_avg.val[0]);
        sum_img2_2      = sum_img2_2 + (img_2[m][n]-img2_avg.val[0])*(img_2[m][n]-img2_avg.val[0]);
	   }
	 }

	corr = sum_img1_img2/sqrt(sum_img1_2*sum_img2_2);

	return corr;
}

CvPoint nadi_centroid(Mat slika){
	int suma_x=0,suma_y=0,n=0;
	int x,y;
	for (int i=0;i<slika.rows;++i){
		for(int j=0;j<slika.cols;++j){
			if(slika.at<uchar>(i,j)!=0){
				suma_x+=i;
				suma_y+=j;
				n++;
			}
		}
	}
	x=suma_x/n; y=suma_y/n;

	return cvPoint(x,y);
}

int PCA_metoda(vector<Mat> slike, Mat temp, int num_components, bool zastavica_za_PCA_svi_pikseli){
	int max_redak=0, max_stupac=0;
	int najslicnija_slika=0;
	slike.push_back(temp);
	vector<Mat> prilagodene_slike=podesavanje_po_centroidu(slike);
	
	//prolanazak srednje slike
	Mat srednja=Mat::zeros(prilagodene_slike[0].rows,prilagodene_slike[0].cols,CV_8UC1);
	bool zastavica=true;
	for(int i=0;i<prilagodene_slike[0].rows;++i){
		for(int j=0;j<prilagodene_slike[0].cols;++j){
			for(int k=0;k<prilagodene_slike.size();++k){
				if(zastavica && (prilagodene_slike[k].at<uchar>(i,j)==0)){
					zastavica=false;
				}
			}
			if(zastavica){
				srednja.at<uchar>(i,j)=255;
			}
			zastavica=true;
		}
	}
	
	vector<Mat> centrirane_slike;

	for(int i=0;i<prilagodene_slike.size();++i){
		centrirane_slike.push_back(prilagodene_slike[i]-srednja);
	}

	temp=centrirane_slike[centrirane_slike.size()-1]; //novi template
	
	centrirane_slike.erase(centrirane_slike.begin()+(centrirane_slike.size()-1)); //micemo template iz skupa slika

												
	Mat redak;
	if (zastavica_za_PCA_svi_pikseli==true){		//na PCA šaljemo sve piksele matrice
	  //prebaciti sve slike u redak
	  redak=Mat::zeros(centrirane_slike.size(),centrirane_slike[0].rows*centrirane_slike[0].cols,CV_8UC1);
	  Mat pomocna,pomocna_f;
		for(int i=0;i<centrirane_slike.size();++i){                 
		  centrirane_slike[i].copyTo(pomocna);
		  for(int n=0;n<pomocna.rows;++n){
			  for(int m=0;m<pomocna.cols;++m){
					  redak.at<uchar>(i,n*pomocna.cols+m)=pomocna.at<uchar>(n,m);	
			  }
		  }
		}
		Mat redak_f;
		redak.convertTo(redak_f,CV_64FC1);
	}
	else							//na PCA šaljemo samo zbroj bijelih piksela po redku
	{
		//prebaciti sve slike u redak kao broj bijelih piksela po redku
		redak=Mat::zeros(prilagodene_slike.size(),prilagodene_slike[0].rows,CV_8UC1);
		Mat pomocna,pomocna_f;
		int sum_cols=0;
		for(int i=0;i<prilagodene_slike.size();++i){                 
			prilagodene_slike[i].copyTo(pomocna);
			for(int n=0;n<pomocna.rows;++n){
				 sum_cols=0;
				 for(int m=0;m<pomocna.cols;++m){
					  if (pomocna.at<uchar>(n,m)!=0) sum_cols++;	
				 }
				 redak.at<uchar>(i,n)=sum_cols;
		   }
		}
		
	}
	
	Mat redak_f;
	Mat temp_redak,temp_rf;
	if (zastavica_za_PCA_svi_pikseli){
		redak.convertTo(redak_f,CV_64FC1);

		temp_redak=Mat::zeros(1,temp.rows*temp.cols,CV_8UC1);
		for(int i=0;i<temp.rows;++i){
		  for(int j=0;j<temp.cols;++j){
			  temp_redak.at<uchar>(0,i*temp.cols+j)=temp.at<uchar>(i,j);
		  }
	    }
	}
	else
	{
		Mat redak_d;
		redak.convertTo(redak_d,CV_64FC1);
		Mat srednji_redak=Mat::zeros(1,redak.cols,CV_64FC1);
		int suma_stupci=0;
		for(int j=0;j<redak.cols;++j){
			suma_stupci=0;
			for(int i=0;i<redak.rows;++i){
				suma_stupci+=redak.at<uchar>(i,j);
			}
			srednji_redak.at<double>(0,j)=suma_stupci/redak.rows;
		}

		Mat centralizirani_redak=Mat::zeros(redak.rows,redak.cols,CV_64FC1);
		for(int i=0;i<redak.rows;++i){
			centralizirani_redak.row(i)=redak_d.row(i)-srednji_redak.row(0);
		}
		centralizirani_redak.convertTo(redak_f,CV_64FC1);

		int sum_cols=0;
		temp_redak=Mat::zeros(1,temp.rows,CV_8UC1);
		for(int i=0;i<temp.rows;++i){
			  sum_cols=0;
			  for(int j=0;j<temp.cols;++j){
				  if(temp.at<uchar>(i,j)!=0) sum_cols++;
			  }
			  temp_redak.at<uchar>(0,i)=sum_cols;
		}
	}

	// Perform a PCA:
	PCA pca(redak_f, Mat(), CV_PCA_DATA_AS_ROW, num_components);
 
    // And copy the PCA results:
	Mat mean = pca.mean.clone();
	Mat eigenvalues = pca.eigenvalues.clone();
	Mat eigenvectors = pca.eigenvectors.clone();

	Mat features = (eigenvectors*redak_f.t()).t(); //znacajke

	temp_redak.convertTo(temp_rf,CV_64FC1);

	Mat feature_temp = (eigenvectors*temp_rf.t()).t(); //znacajke za predlozak
	double min,udaljenost;
	float suma=0;
	
	//racunanje euklidske udaljenosti izmedu znacajki svake pojedine slike i slike s kojom se usporeduje
	for(int i=0;i<features.rows;++i){
		suma=0;
		udaljenost=0;
		for(int j=0;j<num_components;++j){
				suma+=pow((features.at<double>(i,j)-feature_temp.at<double>(0,j)),2);
		}
		if(i==0){
			min=sqrt((double)suma);
		}
		else{
			udaljenost = sqrt((double)suma);
			if(udaljenost<min){
				min=udaljenost;
				najslicnija_slika=i;
			}
		}
	}
	najslicnija_slika+=1;

	return najslicnija_slika;
}

int templateMatching_metoda(vector<Mat> slike, Mat temp){
	//usporedivanje sa svakom pojedinom slikom
	vector<double> vrijednosti;
	double max = 0;
	int najslicnija_slika=0;
	slike.push_back(temp);
	vector<Mat> roi_slike=podesavanje_po_centroidu(slike);
	temp=roi_slike[roi_slike.size()-1];
	roi_slike.erase(roi_slike.begin()+(roi_slike.size()-1));
	for(int i=0;i<roi_slike.size();++i){
		vrijednosti.push_back(podudaranje(roi_slike[i],temp));
	}
	//odredivanje najveceg postotka slicnosti
	for(int i=0;i<vrijednosti.size();++i)
		if(vrijednosti.at(i)>max)
		{
			max=vrijednosti.at(i);
			najslicnija_slika=i;
		}
	najslicnija_slika+=1;
	return najslicnija_slika;
}

vector<Mat> podesavanje_po_centroidu(vector<Mat> slike){
	vector<Mat> vrijednosti;
	CvPoint centar;
	for(int i=0;i<slike.size();++i){
		centar=nadi_centroid(slike[i]);
		vrijednosti.push_back(slike[i](Range(centar.x-195,centar.x+195),Range(centar.y-485,centar.y+495)));
	}
	return vrijednosti;
}