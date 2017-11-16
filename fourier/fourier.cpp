#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
Mat image;

void process(const char* ims, double freq)
{
  if(ims==NULL)
  {
  //lecture de l'image
  exit(EXIT_FAILURE);
  cout<<"Objet vide"<<endl;
  }
    else
  {
       image=imread(ims,0);
       //Optimisation taille image via zero padding
       Mat imageZeroPadded;
       int m = getOptimalDFTSize( image.rows );
       int n = getOptimalDFTSize( image.cols );
       copyMakeBorder(image, imageZeroPadded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));    Mat planes[] = {Mat_<float>(imageZeroPadded), Mat::zeros(imageZeroPadded.size(), CV_32F)};
       //Creation du plan complexe
       Mat complexI;
       merge(planes, 2, complexI);
       dft(complexI, complexI);

       //Calcul Gain + passage en log
       split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
       magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
       phase(planes[0], planes[1], planes[1]);
       Mat magI = planes[0];
       Mat phaseI=planes[1];
       magI += Scalar::all(1);
       log(magI, magI);

       //Crop le spectre pour qu'il ait un nombre pair de colonnes
       magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));


       int cx = magI.cols/2;
       int cy = magI.rows/2;

       Mat q0(magI, Rect(0, 0, cx, cy));
       Mat q1(magI, Rect(cx, 0, cx, cy));
       Mat q2(magI, Rect(0, cy, cx, cy));
       Mat q3(magI, Rect(cx, cy, cx, cy));

       Mat tmp;
       q0.copyTo(tmp);
       q3.copyTo(q0);
       tmp.copyTo(q3);

       q1.copyTo(tmp);
       q2.copyTo(q1);
       tmp.copyTo(q2);

       //transforme en matrice lisible
       normalize(magI, magI, 0, 1, NORM_MINMAX);


       imshow(ims,image);
       imwrite("magnitude.png", magI);
       imshow("spectrum magnitude", magI);
       imshow("spectrum phase",phaseI);
       waitKey(0);
  }
}

void usage (const char *s)
{
    cout<<"Usage: "<<s<<"usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}

#define param 2
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1],atoi(argv[2]));

  return EXIT_SUCCESS;
}
