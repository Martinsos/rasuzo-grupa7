#include "stdafx.h"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat roi_retci(Mat src,int* prvi_redak,int* zadnji_redak);

Mat roi_stupci(Mat src,int* prvi_stupac,int* zadnji_stupac);

void prilagodba(Mat* image_adj, Mat* temp_adj,Mat image, Mat temp,int prvi_img,int zadnji_img,int prvi_tmp,int zadnji_tmp,int vrsta);

double podudaranje(Mat image, Mat temp);

vector<Mat> ucitavanje(String slika_put,int broj_slika);

vector<double> usporedivanje(Mat temp,vector<Mat> slike);

vector<Mat> pca_usporedivanje(Mat temp,vector<Mat> slike);

Mat norm_0_255(const Mat& src);

Mat asRowMatrix(const vector<Mat>& src, int rtype, double alpha, double beta);

Mat prilagodba_slike(Mat slika, int max_redak, int max_stupac);

int main(int argc, char** argv)
{
	Mat image, temp, image_gray, temp_gray;

	int prvi_redak_img=0, zadnji_redak_img=0, prvi_redak_tmp=0, zadnji_redak_tmp=0;
	int prvi_stupac_img=0, zadnji_stupac_img=0, prvi_stupac_tmp=0, zadnji_stupac_tmp=0;
	
	Mat image_roi_r, temp_roi_r,image_roi, temp_roi;

	vector<Mat> slike, slike_roi;

	namedWindow("Template",CV_WINDOW_AUTOSIZE);
	namedWindow("Srednja",CV_WINDOW_AUTOSIZE);
	namedWindow("NajslicnijaSlika",CV_WINDOW_AUTOSIZE);

	int najslicnija_slika=0; //broj naslicnije slike
	double max=0;

	vector<vector<float>> vrijednosti_pca;
	vector<double> vrijednosti;

	int opcija=1;

	int broj_slika;//broj slika koji se ucitava
	int slika_usporedbe; //slika koje ce se usporedivati template
	
	//putanja za sve ostale slike
	String images_path="D:\\FER\\Raspoznavanje uzoraka\\Projekt\\slike\\siluete\\slika";

	//ucitavanje slika u vector
	cout<<"upisi broj slika"<<endl;
	cin>>broj_slika;	
	slike=ucitavanje(images_path,broj_slika);

	cout<<"upisi broj slike koja ce se usporedivati"<<endl;
	cin>>slika_usporedbe;

	temp=slike.at(slika_usporedbe-1);
	
	
	//usporedivanje sa svakom pojedinom slikom
	//vrijednosti=usporedivanje(temp,slike);
	
	int max_redak=0, max_stupac=0;
	
	slike_roi=pca_usporedivanje(temp,slike);
	
	for (int i=0; i<slike_roi.size();++i){
		if(slike_roi[i].rows>max_redak){
			max_redak=slike_roi[i].rows;
		}
		if(slike_roi[i].cols>max_stupac){
			max_stupac=slike_roi[i].cols;
		}
	}

	vector<Mat> prilagodene_slike;
	for (int i=0; i<slike_roi.size();++i){
		prilagodene_slike.push_back(prilagodba_slike(slike_roi[i],max_redak,max_stupac));
	}

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


	temp=centrirane_slike[(slika_usporedbe-1)]; //novi template
	imshow("Template",temp);

	centrirane_slike.erase(centrirane_slike.begin()+(slika_usporedbe-1)); //micemo template iz skupa slika
	
	//prebaciti sve slike u redak
	Mat redak=Mat::zeros(centrirane_slike.size(),centrirane_slike[0].rows*centrirane_slike[0].cols,CV_8UC1);
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
	redak.convertTo(redak_f,CV_32FC1);

	int num_components = 11;
 
    // Perform a PCA:
	PCA pca(redak_f, Mat(), CV_PCA_DATA_AS_ROW, num_components);
 
    // And copy the PCA results:
	Mat mean = pca.mean.clone();
	Mat eigenvalues = pca.eigenvalues.clone();
	Mat eigenvectors = pca.eigenvectors.clone();

	Mat features = (eigenvectors*redak_f.t()).t(); //znacajke

	//stavljanje slike predloska u redak
	Mat temp_redak=Mat::zeros(1,temp.rows*temp.cols,CV_8UC1),temp_rf;
	for(int i=0;i<temp.rows;++i){
		for(int j=0;j<temp.cols;++j){
			temp_redak.at<uchar>(0,i*temp.cols+j)=temp.at<uchar>(i,j);
		}
	}
	
	temp_redak.convertTo(temp_rf,CV_32FC1);

	Mat feature_temp = (eigenvectors*temp_rf.t()).t(); //znacajke za predlozak
	int min,suma=0,udaljenost;	
	//racunanje euklidske udaljenosti izmedu znacajki svake pojedine slike i slike s kojom se usporeduje
	for(int i=0;i<features.rows;++i){
		suma=0;
		udaljenost=0;
		for(int j=0;j<num_components;++j){
				suma+=pow((features.at<float>(i,j)-feature_temp.at<float>(0,j)),2);
		}
		if(i==0){
			min=sqrt(suma);
		}
		else{
			udaljenost = sqrt(suma);
			if(udaljenost<min){
				min=udaljenost;
				najslicnija_slika=i;
			}
		}
	}
	
	/* ovo je onaj stari nacin

	//odredivanje najveceg postotka slicnosti
	for(int i=0;i<vrijednosti.size();++i)
		if(vrijednosti.at(i)>max)
		{
			max=vrijednosti.at(i);
			najslicnija_slika=i;
		}
	//ispis broj slike 
	cout<<"Naslicnija slika je"<<endl;
	cout<<najslicnija_slika<<endl;*/

	najslicnija_slika+=1;
	imshow("Najslicnija slika",prilagodene_slike[(najslicnija_slika-1)]);
	cout<<najslicnija_slika<<endl;
	waitKey(0);
	return(0);
}

Mat prilagodba_slike(Mat slika, int max_redak, int max_stupac){
	Mat prilagodna=Mat::zeros(max_redak+1,max_stupac+1,CV_8UC1);
	int pocetak_retci=floor((max_redak-slika.rows)/2);
	int pocetak_stupci=floor((max_stupac-slika.cols)/2);
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


vector<double> usporedivanje(Mat temp,vector<Mat> slike)
{
	int prvi_redak_img=0, zadnji_redak_img=0, prvi_redak_tmp=0, zadnji_redak_tmp=0;
	int prvi_stupac_img=0, zadnji_stupac_img=0, prvi_stupac_tmp=0, zadnji_stupac_tmp=0;
	int broj_piksela_retci=0;
	Mat image_roi_r, temp_roi_r,image_roi, temp_roi, image;
	vector<double> vrijednosti;
	vector<int> broj_piksela;
	for(int i=0;i<slike.size();++i)
	{
		image=slike.at(i);	
		//odreduje se ROI po retcima
		image_roi_r = roi_retci(image,&prvi_redak_img,&zadnji_redak_img);
		temp_roi_r = roi_retci(temp,&prvi_redak_tmp,&zadnji_redak_tmp);
	
		//gleda se veca od dvije slike i po njoj se namjesta velicina manje da budu iste velicine
		prilagodba(&image_roi_r, &temp_roi_r, image, temp, prvi_redak_img,zadnji_redak_img,prvi_redak_tmp,zadnji_redak_tmp,1);
		
		//trazi se roi po stupcima
		image_roi = roi_stupci(image_roi_r,&prvi_stupac_img,&zadnji_stupac_img);
		temp_roi = roi_stupci(temp_roi_r,&prvi_stupac_tmp,&zadnji_stupac_tmp);
		
		//prilagodba velicine po stupcima
		prilagodba(&image_roi,&temp_roi,image_roi_r,temp_roi_r,prvi_stupac_img,zadnji_stupac_img,prvi_stupac_tmp,zadnji_stupac_tmp,2);
	
		vrijednosti.push_back(podudaranje(image_roi,temp_roi));

	}
	return vrijednosti;
}

vector<Mat> pca_usporedivanje(Mat temp,vector<Mat> slike)
{
	int prvi_redak_img=0, zadnji_redak_img=0, prvi_redak_tmp=0, zadnji_redak_tmp=0;
	int prvi_stupac_img=0, zadnji_stupac_img=0, prvi_stupac_tmp=0, zadnji_stupac_tmp=0;
	int max_stupac=0, max_redak=0;
	float broj_piksela_retci=0;
	Mat image_roi_r, temp_roi_r,image_roi, temp_roi, image;
	vector<Mat> vrijednosti;
	vector<float> broj_piksela;
	for(int i=0;i<slike.size();++i)
	{
		image=slike.at(i);
		//odreduje se ROI po retcima
		image_roi_r = roi_retci(image,&prvi_redak_img,&zadnji_redak_img);
		if(i==0)
			temp_roi_r = roi_retci(temp,&prvi_redak_tmp,&zadnji_redak_tmp);
	
		//gleda se veca od dvije slike i po njoj se namjesta velicina manje da budu iste velicine
		prilagodba(&image_roi_r, &temp_roi_r, image, temp, prvi_redak_img,zadnji_redak_img,prvi_redak_tmp,zadnji_redak_tmp,1);
		
		//trazi se roi po stupcima
		image_roi = roi_stupci(image_roi_r,&prvi_stupac_img,&zadnji_stupac_img);
		if(i==0)
			temp_roi = roi_stupci(temp_roi_r,&prvi_stupac_tmp,&zadnji_stupac_tmp);
		
		//prilagodba velicine po stupcima
		//prilagodba(&image_roi,&temp_roi,image_roi_r,temp_roi_r,prvi_stupac_img,zadnji_stupac_img,prvi_stupac_tmp,zadnji_stupac_tmp,2);
		

		vrijednosti.push_back(image_roi);
	}
	return vrijednosti;
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
		vector_slike.push_back(slika);
	}
	return vector_slike;
}
//funkcija koja izdvaja ROI po retcima
Mat roi_retci(Mat src,int* prvi_redak, int* zadnji_redak)
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
Mat roi_stupci(Mat src,int* prvi_stupac, int* zadnji_stupac)
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
void prilagodba(Mat* image_adj,Mat* temp_adj, Mat image, Mat temp,int prvi_img,int zadnji_img,int prvi_tmp,int zadnji_tmp,int vrsta){
	int razlika=abs((zadnji_img-prvi_img)-(zadnji_tmp-prvi_tmp));
	int dodatak=floor(razlika/2); //vratiti natrag an ceil ako nece raditi
	if(vrsta==1){
		if((zadnji_img-prvi_img)>(zadnji_tmp-prvi_tmp))
		{
			*temp_adj=temp(Range(prvi_tmp-dodatak,zadnji_tmp+dodatak),Range::all());
		}
		else if((zadnji_img-prvi_img)<(zadnji_tmp-prvi_tmp))
		{
			*image_adj=image(Range(prvi_img-dodatak,zadnji_img+dodatak),Range::all());
		}
	}
	else{
		if((zadnji_img-prvi_img)>(zadnji_tmp-prvi_tmp))
		{
			*temp_adj=temp(Range::all(),Range(prvi_tmp-dodatak,zadnji_tmp+dodatak));
		}
		else if((zadnji_img-prvi_img)<(zadnji_tmp-prvi_tmp))
		{
			*image_adj=image(Range::all(),Range(prvi_img-dodatak,zadnji_img+dodatak));
		}
	}
}

//funkcija koja gelda u koliko se tocaka podudaraju dvije siluete
double podudaranje(Mat image, Mat temp){
	double broj_podudaranja=0;
	int retci=image.rows, stupci=image.cols;
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