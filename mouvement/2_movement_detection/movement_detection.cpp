
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
void difference(VideoCapture cap)
{
  Mat frame,frame2,difference;
  cap >> frame;
  namedWindow("video",1);

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

    //Seuillage : Parametre modifiable

    threshold(difference, difference, 50, 255, 0);
    //threshold(difference, difference, 60, 255, 0); // --> OK : Balle perceptible
    //threshold(difference, difference, 90, 255, 0); // --> Non : Une partie de la balle n'est plus visible
    //threshold(difference, difference, 20, 255, 0); --> OK : Balle perceptible
    //threshold(difference, difference, 0, 255, 0); --> Bruit


    //Affichage
    imshow("video",difference);
    if(waitKey(30) >= 0)
      break;
  }
}
//Difference de fond
void difference_background(VideoCapture cap)
{
  Mat difference, frame, frame2;
  Mat background;
  cap >> frame;
  namedWindow("video2",1);



  int nbFrame = int(cap.get(CV_CAP_PROP_FRAME_COUNT));
  currentFrame = 0;
  for (int f=1; f<nbFrame; f++)
  {
    currentFrame=currentFrame+1;
    cap >> frame;
    cap >> frame2;
    if(!frame.data || !frame2.data)
      break;

    cvtColor(frame, frame, CV_BGR2GRAY);
    cvtColor(frame2, frame2, CV_BGR2GRAY);

    BackgroundSubtractorMOG2 BGModel;
    BGModel(frame2, background, -1);
    difference = frame - background;
    threshold(difference, difference, 50, 255, 0);

    imshow("video2",difference);
    if(waitKey(30) >= 0)
      break;

  }
}
//Difference de fond avec melange de gaussienne
void difference_back_MOG(VideoCapture cap)
{
  int nbFrame = int(cap.get(CV_CAP_PROP_FRAME_COUNT));
  Mat difference, frame, frame2,background3;
  int currentFrame = 0;
  namedWindow("video3",1);
  for (int f=0; f<nbFrame; f++)
  {
    currentFrame=currentFrame+1;
    cap >> frame;
    cap >> frame2;
    if(!frame.data || !frame.data)
      break;
    cvtColor(frame, frame, CV_BGR2GRAY);
    cvtColor(frame2, frame2, CV_BGR2GRAY);
    BackgroundSubtractor BGModel2;
    BGModel2(frame2, background3, -1);
    difference = frame - background3;
    threshold(difference, difference, 50, 255, 0);
    imshow("video3",difference);
    if(waitKey(30) >= 0)
      break;
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

  //TODO :
  //difference(cap);
  difference_background(cap);
  //difference_back_MOG(cap);

  return EXIT_SUCCESS;
}

