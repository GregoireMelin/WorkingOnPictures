#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat image;

void fshift(Mat phaseI)
{
	phaseI = phaseI(Rect(0, 0, phaseI.cols & -2, phaseI.rows & -2));
	int cx = phaseI.cols/2;
	int cy = phaseI.rows/2;
	Mat q0(phaseI, Rect(0, 0, cx, cy));
	Mat q1(phaseI, Rect(cx, 0, cx, cy));
	Mat q2(phaseI, Rect(0, cy, cx, cy));
	Mat q3(phaseI, Rect(cx, cy, cx, cy));
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
	normalize(phaseI, phaseI, 0, 1, NORM_MINMAX);
}


Mat getIDFT(Mat complexI)
{
  Mat nouvelle;
  idft(complexI, nouvelle);
  Mat planess[] = {Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F)};
  split(nouvelle, planess);
  magnitude(planess[0], planess[1], nouvelle);
  normalize(nouvelle, nouvelle, 0, 1, NORM_MINMAX);
  return(nouvelle);
}


Mat getPhaseSpec(Mat complex, bool shift = true)
{
      Mat phaseI;
      Mat planes[] = {Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F)};
      split(complex, planes);
      phase(planes[0], planes[1], phaseI);
      phaseI += Scalar::all(1);
      //Passage en log
       log(phaseI, phaseI);
      //Crop le spectre pour qu'il ait un nombre pair de colonnes
      if(shift)
		 	  fshift(phaseI);

      return phaseI;
}


Mat getMagSpec(Mat complex,bool shift = true)
{
    Mat magI;
    Mat planes[] = {Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F)};
    split(complex, planes);
    magnitude(planes[0], planes[1], magI);
    magI += Scalar::all(1);
    log(magI, magI);
    //Crop le spectre pour qu'il ait un nombre pair de colonnes
    if(shift)
			fshift(magI);


  return magI;
}

Mat getDFT(Mat image)
{
  Mat imageZeroPadded;
  int m = getOptimalDFTSize( image.rows );
  int n = getOptimalDFTSize( image.cols );
  copyMakeBorder(image, imageZeroPadded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));

  Mat planes[] = {Mat_<float>(imageZeroPadded), Mat::zeros(imageZeroPadded.size(), CV_32F)};
  Mat complexI;
  merge(planes, 2, complexI);
  dft(complexI, complexI);
  split(complexI, planes);
  return complexI;
}
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
       //Lecture de l'image
       image=imread(ims,0);
       Mat magI,phaseI, complexI, nouvelle;
       //Calcul de la TFD
       complexI=getDFT(image);
       Mat planes[] = {Mat::zeros(complexI.size(), CV_32F), Mat::zeros(complexI.size(), CV_32F)};
       cartToPolar(planes[0],planes[1],magI,phaseI,true);
       magI=getMagSpec(complexI);
       phaseI=getPhaseSpec(complexI);

       //Mat mask;
       //Mat planess[] = {Mat::zeros(complexI.size(), CV_32F),Mat::zeros(complexI.size(), CV_32F)};
       //Mat kernel_spec;
       //planess[0] = mask; // real
       //planess[1] = mask; // imaginar
       //merge(planess, 2, kernel_spec);

       //mulSpectrums(complexI, kernel_spec, complexI, DFT_ROWS);
       /*float maxMag=planes[0];
       for(int i=0;i<sizeof(planes)/sizeof(planes[0]);i++)
       {
         if(planes > maxMag)
         {
           maxMag=planes[0,i];
         }
       }*/

			 freq=freq+1;
       nouvelle=getIDFT(complexI);
//AFFICHAGES ET ENREGISTREMENT
       imshow(ims,image);
       imwrite("magnitude.png", magI);
       imwrite("phase.png",phaseI);
       imshow("spectrum magnitude", magI);
       imshow("spectrum phase",phaseI);
			 imwrite("magnitude-modify.png", magI);
       imshow("IDFT", nouvelle);
       imwrite("inverse-modify.png",nouvelle);
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
