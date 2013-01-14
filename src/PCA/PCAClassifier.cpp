#include "PCAClassifier.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

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

vector<Mat> slike;
vector<string> imena;
vector< pair<string, double> > rezultat;

vector< pair<string, double> > PCAClassifier::classify(Mat img, int resNum)
{
    
	Mat image, temp, image_gray, temp_gray;

	Mat image_roi_r, temp_roi_r,image_roi, temp_roi;

	vector<Mat> slike_roi;

	int najslicnija_slika=0; //broj naslicnije slike
	double max=0;

	vector<vector<float> > vrijednosti_pca;
	vector<double> vrijednosti;

	int num_components = 11;	//broj komponenti koje PCA daje kao sužen izor
	
	temp=img;

	
	//templateMatching_metoda(slike,temp);		// ovdje se bira koja metoda se upotrebljava -1)templateMatching  ili 2)PCA
	
	PCA_metoda(slike,temp,num_components);

	vector< pair<string, double> > ret;

	for (int i=0; i<resNum;++i)
	{
     ret.push_back(ret.at(i));
		
	}
    return ret;

   
}

void PCAClassifier::learn(map< string, vector<Mat> >& learningData, void* param)
{
	//ucitavanje slika u vector
	for( map< string, vector<Mat> >::iterator it = learningData.begin(); it != learningData.end(); ++it ) {

	for( int i = 0; i < (it->second).size(); i++ ) 
    	slike.push_back( (it->second)[ i ] );
		imena.push_back( it->first );
    }
}


int PCAClassifier::PCA_metoda(vector<Mat> slike, Mat temp, int num_components){
	int max_redak=0, max_stupac=0;
	int najslicnija_slika;
	slike.push_back(temp);
	vector<Mat> prilagodene_slike=podesavanje_po_centroidu(slike);
	
	//imshow("Prilagodena slika",prilagodene_slike[0]);
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
	//imshow("srednja", srednja);
	vector<Mat> centrirane_slike;

	for(int i=0;i<prilagodene_slike.size();++i){
		centrirane_slike.push_back(prilagodene_slike[i]-srednja);
	}


	temp=centrirane_slike[centrirane_slike.size()-1]; //novi template
	

	centrirane_slike.erase(centrirane_slike.begin()+(centrirane_slike.size()-1)); //micemo template iz skupa slika

	bool zastavica_za_PCA_svi_pikseli=false;	// ako je TRUE oznaèava da pretvaramo sve piksele slike u redak, 
												// ako je FALSE onda radimo zbroj bijelih piksela po redku slike i stavljamo u jednu matricu
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
		
		udaljenost = sqrt((double)suma);
		rezultat.push_back( make_pair( (string) imena.at(i), udaljenost) );
			
	}
	
	struct sort_pred {
    bool operator()(const std::pair<string,double> &left, const std::pair<string,double> &right) {
        return left.second > right.second;
       }
    };

    std::sort(rezultat.begin(), rezultat.end(), sort_pred());

    return rezultat;
}

int PCAClassifier::templateMatching_metoda(vector<Mat> slike, Mat temp){
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
	
	
	//odredivanje vektora najveceg postotka slicnosti
	for(int i=0;i<vrijednosti.size();++i)
	{
	 rezultat.push_back( make_pair( (string) imena.at(i), vrijednosti.at(i)) );	
	}

	struct sort_pred {
    bool operator()(const std::pair<string,double> &left, const std::pair<string,double> &right) {
        return left.second > right.second;
       }
    };

    std::sort(rezultat.begin(), rezultat.end(), sort_pred());

	return rezultat;
}



Mat PCAClassifier::prilagodba_slike(Mat slika, int max_redak, int max_stupac){
	Mat prilagodna=Mat::zeros(max_redak+1,max_stupac+1,CV_8UC1);
	int pocetak_retci=(int)floor((float) (max_redak-slika.rows)/2);
	int pocetak_stupci=(int)floor((float) (max_stupac-slika.cols)/2);
	int kraj_retci=max_redak-pocetak_retci;
	int kraj_stupci=max_stupac-pocetak_stupci;
	bool zastavica=false;
	int n=-1,m=0;
	for(int i=0;i<max_redak+1;++i){
		if(i>=pocetak_retci && i<kraj_retci)
			n++;
		for(int j=0;j<max_stupac+1;++j){
			if(i>=pocetak_retci && i<kraj_retci)
				if(j>=pocetak_stupci && j<kraj_stupci){
					prilagodna.at<uchar>(i,j)=slika.at<uchar>(n,m);
					m++;
				}
				else m=0;
		}
	}
	return prilagodna;
}

//funkcija koja izdvaja ROI po retcima
Mat PCAClassifier::roi_retci(Mat src,int* prvi_redak, int* zadnji_redak)
{
	Mat mat_roi;
	*prvi_redak=0;
	*zadnji_redak=0;
	bool zastavica=true;
	float suma=0;

	for(int i=0;i<src.rows;++i)
		if(zastavica){
				if(*prvi_redak==0){
					for(int j=1;j<src.cols;++j)
						if(src.at<uchar>(i,j)!=0)
							*prvi_redak=i;
				}
				else if(*zadnji_redak==0)
				{
					suma=0;
					for(int j=1;j<src.cols;++j)
						suma=suma+src.at<uchar>(i,j);
					if(suma==0)
					{	
						*zadnji_redak=i;
						zastavica=false;
					}
				}
				else 
					break;
		}
		else 
			break;
	mat_roi = src(Range(*prvi_redak,*zadnji_redak),Range::all());
	return mat_roi;
}

//funkcija koja izdvaja ROI po stupcima
Mat PCAClassifier::roi_stupci(Mat src,int* prvi_stupac, int* zadnji_stupac)
{
	Mat mat_roi;
	*prvi_stupac=0;
	*zadnji_stupac=0;
	bool zastavica=true;
	float suma=0;

	for(int i=0;i<src.cols;++i)
		if(zastavica){
				if(*prvi_stupac==0){
					for(int j=1;j<src.rows;++j)
						if(src.at<uchar>(j,i)!=0)
							*prvi_stupac=i;
				}
				else if(*zadnji_stupac==0)
				{
					suma=0;
					for(int j=1;j<src.rows;++j)
						suma=suma+src.at<uchar>(j,i);
					if(suma==0)
					{	
						*zadnji_stupac=i;
						zastavica=false;
					}
				}
				else 
					break;
		}
		else 
			break;
	mat_roi = src(Range::all(),Range(*prvi_stupac,*zadnji_stupac));
	return mat_roi;
}

//prilagodba velicine, ako je vrsta=1 onda se prilagodava po retcima, a ako je 2 onda po stupcima
void PCAClassifier::prilagodba(Mat* image_adj,Mat* temp_adj, Mat image, Mat temp,int prvi_img,int zadnji_img,int prvi_tmp,int zadnji_tmp,int vrsta){
	int razlika=abs((zadnji_img-prvi_img)-(zadnji_tmp-prvi_tmp));
	int dodatak=(int)floor((float) razlika/2); //vratiti natrag an ceil ako nece raditi
	if(vrsta==1){
		if((zadnji_img-prvi_img)>(zadnji_tmp-prvi_tmp))
		{
			if((prvi_tmp-dodatak)<0)
				*temp_adj=temp(Range(prvi_img,zadnji_img),Range::all());
			else
				*temp_adj=temp(Range(prvi_tmp-dodatak,zadnji_tmp+dodatak),Range::all());
		}
		else if((zadnji_img-prvi_img)<(zadnji_tmp-prvi_tmp))
		{
			if((prvi_img-dodatak)<0)
				*image_adj=image(Range(prvi_tmp,zadnji_tmp),Range::all());
			else
				*image_adj=image(Range(prvi_img-dodatak,zadnji_img+dodatak),Range::all());
		}
	}
	else{
		if((zadnji_img-prvi_img)>(zadnji_tmp-prvi_tmp))
		{
			if((prvi_tmp-dodatak)<0)
				*temp_adj=temp(Range::all(),Range(prvi_img,zadnji_img));
			else
				*temp_adj=temp(Range::all(),Range(prvi_tmp-dodatak,zadnji_tmp+dodatak));
		}
		else if((zadnji_img-prvi_img)<(zadnji_tmp-prvi_tmp))
		{
			if((prvi_img-dodatak)<0)	
				*image_adj=image(Range::all(),Range(prvi_tmp,zadnji_tmp));
			else
				*image_adj=image(Range::all(),Range(prvi_img-dodatak,zadnji_img+dodatak));
		}
	}
}

//funkcija koja gleda u koliko se tocaka podudaraju dvije siluete
double PCAClassifier::podudaranje(Mat image, Mat temp){
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
double PCAClassifier::cross_correlation( IplImage* img1, IplImage* img2 )
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

CvPoint PCAClassifier::nadi_centroid(Mat slika){
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

vector<Mat> PCAClassifier::podesavanje_po_centroidu(vector<Mat> slike){
	vector<Mat> vrijednosti;
	CvPoint centar;
	for(int i=0;i<slike.size();++i){
		centar=nadi_centroid(slike[i]);
		vrijednosti.push_back(slike[i](Range(centar.x-195,centar.x+195),Range(centar.y-485,centar.y+495)));
	}
	return vrijednosti;
}
