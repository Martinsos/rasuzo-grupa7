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

int main(int argc, char** argv)
{
	Mat image, temp, image_gray, temp_gray;

	int prvi_redak_img=0, zadnji_redak_img=0, prvi_redak_tmp=0, zadnji_redak_tmp=0;
	int prvi_stupac_img=0, zadnji_stupac_img=0, prvi_stupac_tmp=0, zadnji_stupac_tmp=0;
	
	Mat image_roi_r, temp_roi_r,image_roi, temp_roi;

	vector<Mat> slike;

	namedWindow("Template",CV_WINDOW_AUTOSIZE);
	namedWindow("NajslicnijaSlika",CV_WINDOW_AUTOSIZE);

	int najslicnija_slika=0; //broj naslicnije slike
	double max=0;

	vector<double> vrijednosti;

	int broj_slika;//broj slika koji se ucitava
	int slika_usporedbe; //slika koje ce se usporedivati template

	//putanja slika koja se usporeduje s ostalima
	//temp = imread("D:\\FER\\Raspoznavanje uzoraka\\Projekt\\slike\\siluete\\slika12.jpg",0);

	//putanja za sve ostale slike
	String images_path="D:\\FER\\Raspoznavanje uzoraka\\Projekt\\slike\\siluete\\slika";

	//ucitavanje slika u vector
	cout<<"upisi broj slika"<<endl;
	cin>>broj_slika;	
	slike=ucitavanje(images_path,broj_slika);

	cout<<"upisi broj slike koja ce se usporedivati"<<endl;
	cin>>slika_usporedbe;

	temp=slike.at(slika_usporedbe-1);
	imshow("Template",temp);
	
	//slika se mice iz skupa za usporedivanje
	slike.erase(slike.begin()+(slika_usporedbe-1));
	
	//usporedivanje sa svakom pojedinom slikom
	vrijednosti=usporedivanje(temp,slike);
	
	//odredivanje najveceg postotka slicnosti
	for(int i=0;i<vrijednosti.size();++i)
		if(vrijednosti.at(i)>max)
		{
			max=vrijednosti.at(i);
			najslicnija_slika=i;
		}
	//ispis broj slike 
	najslicnija_slika+=1;
	cout<<"Naslicnija slika je"<<endl;
	cout<<najslicnija_slika<<endl;
	imshow("Najslicnija slika",slike.at(najslicnija_slika-1));
	waitKey(0);
	return(0);
}
vector<double> usporedivanje(Mat temp,vector<Mat> slike)
{
	int prvi_redak_img=0, zadnji_redak_img=0, prvi_redak_tmp=0, zadnji_redak_tmp=0;
	int prvi_stupac_img=0, zadnji_stupac_img=0, prvi_stupac_tmp=0, zadnji_stupac_tmp=0;
	Mat image_roi_r, temp_roi_r,image_roi, temp_roi, image;
	vector<double> vrijednosti;
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
	int dodatak=ceil(razlika/2);
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