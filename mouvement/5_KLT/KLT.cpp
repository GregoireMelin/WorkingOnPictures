
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

int main( int argc, char** argv )
{
  if( argc <1 )
  {
    printf("Usage: main video_name \n");
    exit(0);
  }

  //---- OUVERTURE DE LA VIDEO ---- //
  VideoCapture capture(argv[1]); //1er argument = nom de la video chargee
  if(!capture.isOpened())
  {
    cout << "error opening video file" << endl;
    return -1;
  }
  //creation de la fenetre acceuillant l'affichage
  namedWindow("tracking",1);


  // ---- TRAITEMENT DE LA VIDEO : TRACKING DES POINTS D'INTERETS ---- //


  /* TRAVAIL SUR LA PREMIERE IMAGE */

  Mat frame; //Stockage de la frame courante et de la frame precedente en niveau de gris
  Mat frameg, prev_frameg;//dans les objets frameg et prev_frameg
  capture >> frame;
  cvtColor(frame, frameg, CV_BGR2GRAY); //Conversion BGR en niveau de gris
  prev_frameg = frameg.clone();

  std::vector<Point2f> features_prev, features; //Creation des objets acceuillant les points d'interet d'une image

  //DETECTION DES MOTIFS STOCKES DANS features
  goodFeaturesToTrack(frameg,features,50,0.01,10);
  size_t i;
  for( i = 0; i < features.size(); i++ )
    circle( frame, features[i], 3, Scalar(0,255,0), -1, 8);   //On marque d'un cercle les feature reperees sur la premiere image de la video

// ---------------------------------------------
  //Fonction goodFeaturesToTrack() :
  //The function finds the most prominent corners in the image or in the specified image region.
    //frameg : the image
    //features : the output detected features
    //50 : the maximum number of features
    //0.01 : quality level
    //10 : min distance between two features
// ---------------------------------------------

  imshow("tracking", frame); //Affichage de la frame travaillee
  waitKey();

  /* TRACKING DES POINTS D'INTERETS SUR LA SUITE DE LA VIDEO */
  double nframes= capture.get(CV_CAP_PROP_FRAME_COUNT);
  for (int f=1; f<nframes; f++)
    {
      capture >> frame; // get a new frame from camera
      cvtColor(frame, frameg, CV_BGR2GRAY);
      features_prev = features;

      // Calculates an optical flow for a sparse feature set using the iterative Lucas-Kanade method with pyramids.
      vector<uchar> status;
      vector<float> err;
      // Find position of features in new image
      calcOpticalFlowPyrLK(prev_frameg, frameg, features_prev, features, status,err);
			//err : tracking error
      //status : tracking success
      prev_frameg = frameg.clone();

      //Dessiner un point VERT sur la feature detectee
      size_t i; //type qui peut contenir un nombre assez grand pour représenter la taille du plus grand objet creable ~ u_int
      int size = 0;
      for( i = 0; i < features.size(); i++ )
	    {
	       if(!status[i])
	         continue;
	       size++;
	       circle( frame, features[i], 3, Scalar(0,255,0), -1, 8);
	    }

      //Affichage de la frame courante avec les features pointees
      imshow("tracking", frame);
      cout << "Number of tracked features " << size << endl;

      //waitKey(); //Passage a la frame suivante
    }
}
