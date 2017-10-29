#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat makeHist(Mat image)
{
   Mat valHist = Mat::zeros(1,256, CV_32F);
   int intensity;
   for ( int j = 0; j < image.rows; ++j)
   {
       for ( int i = 0; i < image.cols; ++i)
       {
         intensity = (float)image.at<uchar>(j, i);
         valHist.at<float>(intensity) = valHist.at<float>(intensity)+1;
       }
   }
   return valHist;
}

Mat makeHistNorm(Mat image)
{
   Mat hist;
   hist=makeHist(image);
   /*for ( int i = 0; i < hist.rows; ++i)
   {
     hist.at<float>(i) = hist.at<float>(i);
   }*/
   return hist;
}

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
    Mat image=imread(imsname,0);
    Mat valHist=makeHist(image);
    Mat valHistNorm=makeHistNorm(image);
    //Mat valHistCumul;


    for ( int i = 0; i < image.cols; ++i)
    {
           cout<<"HN : "<<valHist.at<int>(i)<<" "<<endl;;
           //cout<<"HNo : "<<valHistNorm.at<double>(i)<<endl;

    }
    //waitKey(0);
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
