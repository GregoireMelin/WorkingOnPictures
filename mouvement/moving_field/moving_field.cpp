
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


  Mat frame;
  namedWindow("video",1);
  cap >> frame;

  int nbFrame = int(cap.get(CV_CAP_PROP_FRAME_COUNT));
  currentFrame = 0;
  for (int f=1; f<nbFrame; f++)
  {
    cap >> frame;

    if(!frame.data)
      break;

    imshow("video",frame);

    if(waitKey(30) >= 0)
      break;
  }
  return EXIT_SUCCESS;
}
