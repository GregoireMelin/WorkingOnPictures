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
       copyMakeBorder(image, imageZeroPadded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

       Mat planes[] = {Mat_<float>(imageZeroPadded), Mat::zeros(imageZeroPadded.size(), CV_32F)};
       Mat complexI;
       merge(planes, 2, complexI);
       dft(complexI, complexI);
       split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
       Mat magI,phaseI;
       cartToPolar(planes[0],planes[1],magI,phaseI,true);


//CALCUL GAIN
       //Passage en log
       log(magI, magI);
       //Crop le spectre pour qu'il ait un nombre pair de colonnes
       magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
       int cx = magI.cols/2;
       int cy = magI.rows/2;
       //Recentrage
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

//CALCUL PHASE
      //Passage en log
      log(phaseI, phaseI);
      //Crop le spectre pour qu'il ait un nombre pair de colonnes
      phaseI = phaseI(Rect(0, 0, phaseI.cols & -2, phaseI.rows & -2));
      int cxx = phaseI.cols/2;
      int cyy = phaseI.rows/2;
      //Recentrage
      Mat q4(phaseI, Rect(0, 0, cxx, cyy));
      Mat q5(phaseI, Rect(cxx, 0, cxx, cyy));
      Mat q6(phaseI, Rect(0, cyy, cxx, cyy));
      Mat q7(phaseI, Rect(cxx, cyy, cxx, cyy));

      Mat tmp2;
      q4.copyTo(tmp2);
      q7.copyTo(q4);
      tmp.copyTo(q7);

      q5.copyTo(tmp2);
      q6.copyTo(q5);
      tmp.copyTo(q6);

      //transforme en matrice lisible
      normalize(phaseI, phaseI, 0, 1, NORM_MINMAX);

//AFFICHAGES ET ENREGISTREMENT
       imshow(ims,image);
       imwrite("magnitude.png", magI);
       imwrite("phase.png",phaseI);

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
