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
Mat frame, frame_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

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

  Mat canny;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  Mat drawing;

  int morpho_size = 2;
  Mat element = getStructuringElement(MORPH_ELLIPSE,Size(2 * morpho_size + 1, 2 * morpho_size + 1),Point(morpho_size, morpho_size));

  double nframes= cap.get(CV_CAP_PROP_FRAME_COUNT);
  for (int f=1; f<nframes; f++)
  {
    cap >> frame; // get a new frame from camera
    if(!frame.data)
      break;
    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    //Leger flou sur l'image pour enlever le bruit de l'image
    blur( frame_gray, frame_gray, Size(3,3));
    //Detection des "contours grace a un filtre de Canny
    Canny( frame_gray, canny, thresh, thresh*2, 3 );

    //OUVERTURE : dilatation(erosion())
    erode(frame_gray,frame_gray,element);
    dilate(frame_gray,frame_gray,element);

    //FERMETURE : erosion(dilatation)
    dilate(frame_gray,frame_gray,element);
    erode(frame_gray,frame_gray,element);

    findContours( canny, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    drawing = Mat::zeros( canny.size(), CV_8UC3);
    for( unsigned int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }
    imshow("video",drawing);

    if(waitKey(30) >= 0)
      break;
  }

  return EXIT_SUCCESS;
}
