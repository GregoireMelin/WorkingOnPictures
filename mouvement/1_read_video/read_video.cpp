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
    currentFrame = trackbarPosition;
    cap.set(CV_CAP_PROP_POS_FRAMES, trackbarPosition);
  }
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
  namedWindow("video",1);
  cap >> frame;

  int nbFrame = int(cap.get(CV_CAP_PROP_FRAME_COUNT));
  trackbarPosition = 0;
  currentFrame = 0;

  for(;;)
  {
    trackbarPosition = trackbarPosition + 1;
    currentFrame=currentFrame+1;
    cap >> frame;
    if(!frame.data)
      break;

    imshow("video",frame);
    createTrackbar("trackbar","video",&trackbarPosition,nbFrame,on_trackbar_change);

    if(waitKey(30) >= 0)
      break;
  }

  return EXIT_SUCCESS;
}
