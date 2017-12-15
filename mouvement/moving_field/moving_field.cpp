
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/video/tracking.hpp"

#include <iostream>
#include <sstream>

#include <fstream>
#include <cstdlib>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;

//Variables globales
int currentFrame;
VideoCapture cap;

void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,const Scalar& color)
{
  for(int y = 0; y < cflowmap.rows; y += step)
    for(int x = 0; x < cflowmap.cols; x += step)
    {
      const Point2f& fxy = flow.at<Point2f>(y, x);
      line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), color);
      circle(cflowmap, Point(x,y), 2, color, -1);
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

  //AFFICHAGE DE LA VIDEO
  if(argc > 1)
    cap.open(string(argv[1]));
  else
    cap.open(0);

  Mat flow, frame;
  Mat  flowUmat, prevgray;

  for (;;)
  {
    cap >> frame;
    Mat img;
    Mat original;
    // capture frame from video file
    cap.retrieve(img, CV_CAP_OPENNI_BGR_IMAGE);
    resize(img, img, Size(640, 480));

    // save original for later
    img.copyTo(original);

    // just make current frame gray
    cvtColor(img, img, COLOR_BGR2GRAY);

    if (prevgray.empty() == false)
    {
     // calculate optical flow
     calcOpticalFlowFarneback(prevgray, img, flowUmat, 0.4, 1, 12, 2, 8, 1.2, 0);
     // copy Umat container to standard Mat
     flowUmat.copyTo(flow);

     // By y += 5, x += 5 you can specify the grid
     for (int y = 0; y < original.rows; y += 5)
     {
       for (int x = 0; x < original.cols; x += 5)
       {
         // get the flow from y, x position * 10 for better visibility
         const Point2f flowatxy = flow.at<Point2f>(y, x) * 10;
         // draw line at flow direction
         line(original, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)), Scalar(255,0,0));                                                  // draw initial point
         circle(original, Point(x, y), 1, Scalar(0, 0, 0), -1);
       }
      }
    }
    else
    {
      img.copyTo(prevgray);
    }
    namedWindow("prew", WINDOW_AUTOSIZE);
    imshow("prew", original);
    img.copyTo(prevgray);

 }
  return EXIT_SUCCESS;
}
