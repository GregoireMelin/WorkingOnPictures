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
    Mat image0 = imread(imsname,CV_LOAD_IMAGE_COLOR);
    Mat image;
    Vec3b intensity;
    Mat blueImage(image0.rows,image0.cols,CV_16UC3,Scalar(3));
    Mat greenImage(image0.rows,image0.cols,CV_16UC3,Scalar(3));
    Mat redImage(image0.rows,image0.cols,CV_16UC3,Scalar(3));
    for ( int j = 0; j < image.rows; ++j)
    {
      for ( int i = 0; i < image.cols; ++i)
              {
                    blueImage.at<uchar>(j,i,0) = intensity.val[0];
                    greenImage.at<uchar>(j,i,1) = intensity.val[1];
                    redImage.at<uchar>(j,i,2) = intensity.val[2];
              }
            }

    namedWindow("blue",WINDOW_AUTOSIZE);
    imshow("blue",blueImage);
    namedWindow("green",WINDOW_AUTOSIZE);
    imshow("green",greenImage);
    namedWindow("red",WINDOW_AUTOSIZE);
    imshow("red",redImage);
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
