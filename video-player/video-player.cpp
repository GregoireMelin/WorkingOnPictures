#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

//Variables globales
int trackbarPosition;
int currentFrame;
VideoCapture cap;

void usage (const char *s)
{
    cout<<"Usage: "<<s<<"usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}

void on_trackbar_change(int, void*)
{
  if(trackbarPosition != currentFrame)
  {
    cout<<"Go To Frame "<<trackbarPosition<<endl;
    currentFrame = trackbarPosition;
    cap.set(CV_CAP_PROP_POS_FRAMES, trackbarPosition);
  }
}

#define param 2
int main( int argc, char* argv[] )
{
  if(argc > 1)
    cap.open(string(argv[1]));
  else
    cap.open(0);

  Mat frame;
  namedWindow("video",1);
  cap >> frame;

  double fps = cap.get(CV_CAP_PROP_FPS);
  int nbFrame = int(cap.get(CV_CAP_PROP_FRAME_COUNT));
  trackbarPosition = 0;
  currentFrame = 0;


  cout<<"Height= "<< frame.cols<<endl;
  cout<<"Width = "<< frame.rows<<endl;
  for(;;)
  {
    trackbarPosition = trackbarPosition + 1;
    currentFrame=currentFrame+1;

    cout<<"FPS = "<<fps<<endl;
    cout<<"Number of frame = "<<nbFrame<<endl;


    if (trackbarPosition<1000)
      cout<<"Frame : "<< trackbarPosition << endl;
    else
      cout<<"Frame : > 1000" << endl;

    cap >> frame;

    if(!frame.data)
      break;

    imshow("video",frame);
    createTrackbar("trackbar","video",&trackbarPosition,nbFrame,on_trackbar_change);
    cout<<"__________________"<<endl;

    if(waitKey(30) >= 0)
      break;
  }

  return EXIT_SUCCESS;

}
