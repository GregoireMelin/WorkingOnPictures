

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>


using namespace cv;
using namespace std;

//Variables globales
int currentFrame;
VideoCapture cap;


void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step, const Scalar& color)
{
  for(int y = 0; y < cflowmap.rows; y += step)
  {
    for(int x = 0; x < cflowmap.cols; x += step)
      {
        const Point2f& fxy = flow.at<Point2f>(y, x);
        line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
        circle(cflowmap, Point(x,y), 2, color, -1);
      }
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


  Mat frame;
  Mat frame2;
  Mat difference;
  namedWindow("video",1);
  cap >> frame;

  int nbFrame = int(cap.get(CV_CAP_PROP_FRAME_COUNT));
  currentFrame = 0;
  for (int f=1; f<nbFrame; f++)
  {
    currentFrame=currentFrame+1;
    cap >> frame;

    cap >> frame2;

    difference = frame - frame2;
    cvtColor(difference, difference, CV_BGR2GRAY);
    //Seuillage
    threshold(difference, difference, 50, 255, 0);

    if(!frame.data)
      break;

    imshow("video",difference);

    if(waitKey(30) >= 0)
      break;
  }

  Mat background;


  return EXIT_SUCCESS;
}
