
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

  Mat frame,frame2,difference;
  cap >> frame;
  namedWindow("video",1);
  namedWindow("video2",1);
  namedWindow("video3",1);
  cap >> frame;

  int nbFrame = int(cap.get(CV_CAP_PROP_FRAME_COUNT));
  currentFrame = 0;
  for (int f=1; f<nbFrame; f++)
  {
    currentFrame=currentFrame+1;
    cap >> frame;
    cap >> frame2;
    if(!frame.data || !frame2.data)
      break;

    //Faire la difference des images pixel a pixel
    difference = frame - frame2;
    cvtColor(difference, difference, CV_BGR2GRAY);
    //Seuillage
    threshold(difference, difference, 50, 255, 0);

    //Affichage
    imshow("video",difference);
    if(waitKey(30) >= 0)
      break;
  }

  Mat background;
  currentFrame = 0;


  for (int f=1; f<nbFrame; f++)
  {
    currentFrame=currentFrame+1;
    cap >> frame;
    cap >> frame2;

    cvtColor(frame, frame, CV_BGR2GRAY);
    cvtColor(frame2, frame2, CV_BGR2GRAY);

    BackgroundSubtractorMOG2 BGModel;

    BGModel(frame2, background, -1);

    difference = frame - background;

    threshold(difference, difference, 50, 255, 0);

    if(!frame.data)
      break;

    imshow("video2",difference);
    if(waitKey(30) >= 0)
      break;

  }

  Mat background3;
  currentFrame = 0;



  for (int f=1; f<nbFrame; f++)
  {
    currentFrame=currentFrame+1;
    cap >> frame;
    cap >> frame2;

    cvtColor(frame, frame, CV_BGR2GRAY);
    cvtColor(frame2, frame2, CV_BGR2GRAY);

    BackgroundSubtractor BGModel2;

    BGModel2(frame2, background3, -1);

    difference = frame - background3;

    threshold(difference, difference, 50, 255, 0);

    if(!frame.data)
      break;

    imshow("video3",difference);
    if(waitKey(30) >= 0)
      break;

  }

  return EXIT_SUCCESS;
}
