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
    //Image source + destinations
    Mat image = imread(imsname,1);
    vector<Mat> channels;

    split(image, channels);
    //Affichage de canaux RGB
	  imshow("B", channels[0]);
	  imshow("G", channels[1]);
	  imshow("R", channels[2]);

    //Conversion en niveaux de gris
    Mat image_gray;
    cvtColor(image, image_gray, cv::COLOR_BGR2GRAY);
    imshow("RGBtoGray", image_gray);

    //Conversion en YCbCbCr
    Mat image_YCbCr;
    cvtColor(image, image_YCbCr, COLOR_BGR2YCrCb);
    vector<Mat> channels_YCbCr;
    split(image_YCbCr, channels_YCbCr);

    //Affichage des canaux YCbCr
    imshow("Y", channels_YCbCr[0]);
    imshow("Cb", channels_YCbCr[1]);
    imshow("Cr", channels_YCbCr[2]);

    //Conversion et affichage de l'image en YCbCr en RGB
    Mat image_YCbCr2RGB;
    cvtColor(image_YCbCr, image_YCbCr2RGB, cv::COLOR_YCrCb2BGR);
    imshow("RGBtoYCbCrtoRGB", image_YCbCr2RGB);
    imshow("original",image);

    Mat GrayMinusY = image_gray - channels_YCbCr[0];
    imshow("YmoinsGray", GrayMinusY);

    Mat RGBMinusRGBconverted = image - image_YCbCr;
    imshow("RGBmoinsRGBtoYCbCrtoRGB", RGBMinusRGBconverted);

    cout<<"Type de la matrice image grise "<< image_gray.channels()<<" "<< image_gray.depth()<<endl;
    cout<<"Type de la matrice Y "<< channels_YCbCr[0].channels()<<" "<< channels_YCbCr[0].depth()<<endl;
    cout<<"Type de la matrice image: "<< image.channels()<<" "<< image.depth()<<endl;
    cout<<"Type de la matrice RGB to YCbCr to RGB: "<<image_YCbCr2RGB.channels()<<" "<< image_YCbCr2RGB.depth()<<endl;
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
