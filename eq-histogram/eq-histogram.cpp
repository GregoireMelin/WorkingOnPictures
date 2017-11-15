#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat image;
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

Mat makeHistNorm(Mat hist)
{
   Mat histN = Mat::zeros(1,256, CV_32F);
   for(int i=0;i<hist.cols;++i)
   {
     histN.at<float>(i)=hist.at<float>(i)/(image.rows*image.cols);
   }
   return histN;
}

Mat makeHistcum(Mat hist)
{
  Mat Histcum = Mat::zeros(1,256, CV_32F);
  float intensity;
  Histcum.at<float>(0) = hist.at<float>(0);

  for ( int i = 1; i < hist.cols; ++i)
    {
        intensity = Histcum.at<float>(i-1) + hist.at<float>(i);
        Histcum.at<float>(i) = intensity;
    }
    return Histcum;
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
    image=imread(imsname,0);

    //Creation de l'histograme
    Mat valHist=makeHist(image);
    //Creation de l'histogramme normalise
    Mat valHistNorm=makeHistNorm(valHist);
    //Creation de l'histogramme cumule
    Mat valHistcum=makeHistcum(valHist);
    //Cration de l'histograme cumule normalise
    Mat valHistcumN=makeHistcum(valHistNorm);


  /*  for ( int i = 0; i < valHist.cols; ++i)
    {
      cout<< "H["<<i<<"] "<<valHist.at<float>(i)<<" "<<endl;
      cout<< "HN["<<i<<"] "<<valHistNorm.at<float>(i)<<" "<<endl;
      cout<< "Hcum["<<i<<"] "<<valHistcum.at<float>(i)<<" "<<endl;
      cout<< "HcumN["<<i<<"] "<<valHistcumN.at<float>(i)<<" "<<endl;
    }*/

    float imax= 255;
    int intensity;

    //Equilibrage d'histogamme "a la main"
     Mat image_egalisee= image.clone();
     for(int i=0;i<image.rows;++i)
     {
       for(int j=0;j<image.cols;++j)
             {
               intensity=image_egalisee.at<uchar>(i,j);
               intensity=imax*valHistcumN.at<float>(0,intensity);
               image_egalisee.at<uchar>(i,j) = (uchar)intensity;
             }
     }

    imwrite("eq.png", image_egalisee);
    imshow("eq.png", image_egalisee);

    //Utilisation de la fonction equalizeHist
    Mat eq_ocv;
    equalizeHist( image, eq_ocv );

    imwrite("eq-ocv.png", eq_ocv);
    imshow("eq-ocv.png",eq_ocv);

    //Creation de l'image de difference
    Mat diff = eq_ocv - image_egalisee;
    imwrite("diff.png",diff);
    imshow("diff.png",diff);

    imshow("eq-ex",image);
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
