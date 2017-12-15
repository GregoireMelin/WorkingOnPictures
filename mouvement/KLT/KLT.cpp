
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

  if( argc <1 )  {
    printf("Usage: main video_name \n");
    exit(0);     
  }


  VideoCapture capture(argv[1]);



  if(!capture.isOpened()) {  // check if we succeeded
    cout << "error opening video file" << endl;
    return -1;
  }



  namedWindow("tracking",1);


  Mat frame;
  Mat frameg, prev_frameg;
  capture >> frame; // get a new fame 
  cvtColor(frame, frameg, CV_BGR2GRAY);
  prev_frameg = frameg.clone();  

  std::vector<Point2f> features_prev, features;
  goodFeaturesToTrack(frameg, // the image 
			  features,   // the output detected features
			  50,  // the maximum number of features 
			  0.01,     // quality level
			  10      // min distance between two features
			  );

  

  size_t i;
  for( i = 0; i < features.size(); i++ )
    {
      circle( frame, features[i], 3, Scalar(0,255,0), -1, 8);
    }
  imshow("tracking", frame);
  waitKey();

  double nframes= capture.get(CV_CAP_PROP_FRAME_COUNT);
  for (int f=1; f<nframes; f++)
    {

      capture >> frame; // get a new frame from camera
      cvtColor(frame, frameg, CV_BGR2GRAY);
      features_prev = features;
      
      // Find position of features in new image
      vector<uchar> status;
      vector<float> err;
      calcOpticalFlowPyrLK(prev_frameg, frameg, features_prev, features,
			   status,    // tracking success
			   err      // tracking error
			   );

      prev_frameg = frameg.clone();  
      size_t i;
      int size = 0;
      for( i = 0; i < features.size(); i++ )
	{
	  if( !status[i] )
	    continue;
	  size++;
	  circle( frame, features[i], 3, Scalar(0,255,0), -1, 8);
	}
      imshow("tracking", frame);
      cout << "Number of tracked features " << size << endl;
      waitKey();

    }



}


