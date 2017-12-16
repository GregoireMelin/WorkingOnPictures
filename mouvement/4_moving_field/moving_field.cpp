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

  Mat res, img1, img2, img2Original, img2OriginalC;
  VideoWriter writer;
  cap >> img1;
  cvtColor(img1, img1, COLOR_BGR2GRAY);
  double fps = cap.get(CV_CAP_PROP_FPS);
  cv::Size tamano((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
  writer.open("result.avi", CV_FOURCC('M', 'J', 'P', 'G'), fps, tamano);
  for (;;)
  {
    cap >> img2;

    if (img2.empty())
      break;
    img2OriginalC=img2.clone();
    cvtColor(img2, img2, COLOR_BGR2GRAY);
    img2Original=img2.clone();
    calcOpticalFlowFarneback(img1, img2, res, .4, 1, 12, 2, 8, 1.2, 0);

    //Premiers tests :
    //calcOpticalFlowFarneback(img1, img2, res, .4, 1, 12, 2, 8, 1.2, 0);

    for (int y = 0; y < img2.rows; y += 5) {
      for (int x = 0; x < img2.cols; x += 5)
      {
        // get the flow from y, x position * 3 for better visibility
        const Point2f flowatxy = res.at<Point2f>(y, x) * 1;
        // draw line at flow direction
        line(img2OriginalC, Point(x, y), Point(cvRound(x + flowatxy.x), cvRound(y + flowatxy.y)), Scalar(255, 0, 0));
        // draw initial point
        circle(img2OriginalC, Point(x, y), 1, Scalar(0, 0, 0), -1);
      }
    }
    img2Original=img1.clone();
    imshow("RESULT",img2OriginalC);
    writer << img2OriginalC;
    if (cv::waitKey(1) == 27) break;

  }
  cap.release();
  return EXIT_SUCCESS;
}
