#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;


void process(const char* imsname)
{
  if(imsname==NULL)
  {
    cout<<"Objet vide"<<endl;
    exit(EXIT_FAILURE);
  }
  else
  {
    //lecture de l'image
    Mat image = imread(imsname,0);
    Mat valHist = Mat::zeros(1,256, CV_32F);
    Mat valHistNorm = Mat::zeros(1,256, CV_32F);
    Mat valHistCumul = Mat::zeros(1,256, CV_32F);

    int intensity;
    float sum=0;
    for ( int j = 0; j < image.rows; ++j)
       {
         for ( int i = 0; i < image.cols; ++i)
         {
           intensity = (float)image.at<uchar>(j, i);
           valHist.at<float>(intensity) = valHist.at<float>(intensity)+1;
           sum=sum+valHist.at<float>(intensity);
           valHistNorm.at<float>(intensity)= valHist.at<float>(intensity)/(image.rows*image.cols);
           //cout<< "Non Norm. = "<<valHist.at<float>(intensity)<<" ,Norm = "<<valHistNorm.at<float>(intensity)<<endl;
           valHistCumul.at<float>(intensity)=valHist.at<float>(intensity)+sum;
           cout<<"non Cumul. ="<<valHist.at<float>(intensity)<<" ,Cumul = "<<valHistCumul.at<float>(intensity)<<endl;
         }
       }

    waitKey(0);
    }
}

void usage (const char *s)
{
    cout<<"Usage: "<<s<<"usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}
#define param 1
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1]);

  return EXIT_SUCCESS;
}
