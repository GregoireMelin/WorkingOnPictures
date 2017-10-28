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
    Mat BD(image.rows,image.cols,CV_8UC3);
    Mat GD(image.rows,image.cols,CV_8UC3);
    Mat RD(image.rows,image.cols,CV_8UC3);

    //Split de l'image
    Mat channels[3];
    split(image,channels);
    Mat B = channels[0];
    Mat G = channels[1];
    Mat R = channels[2];

    //Affichage
    namedWindow("b",WINDOW_AUTOSIZE);namedWindow("g",WINDOW_AUTOSIZE);namedWindow("r",WINDOW_AUTOSIZE);
    imshow("b",BD);imshow("g",GD);imshow("r",RD);
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
