
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;

// STL INCLUDES
#include <iostream>
#include <algorithm>
using namespace std;

int main(int argc, char** argv)
{
  std::cout << " Usage: ./stereomatch <image1> <image2> <number of disparities> <patch size> <output image>" << std::endl; 

  if(argc != 6){
    cout << "ERROR: Wrong number of input parameters" << endl;
    return -1;
  }
    const char* img1_filename = argv[1];
    const char* img2_filename = argv[2];
    const char* disparity_filename = argv[5];

    
    int SADWindowSize = atoi(argv[4]), numberOfDisparities = atoi(argv[3]);
    if (  numberOfDisparities < 1 || numberOfDisparities % 16 != 0 )
      {
	cout << "Command-line parameter error: The max disparity must be a positive integer divisible by 16" << endl;
        return -1;
      }
    if ( SADWindowSize < 1 || SADWindowSize % 2 != 1 )
      {
	cout << "Command-line parameter error: The block size  must be a positive odd number" << endl;
	return -1;
      }

    Mat img1 = imread(img1_filename,-1);
    Mat img2 = imread(img2_filename,-1);
    imshow(" ",img1);
    imshow(" 2 ",img2);
    waitKey();


    //------------------------------ Compute Disparity  -------------------
    //.............................
    Mat disp, disp8;
    //disp=......
    //disp.convertTo(disp8, CV_8U, 255/(numberOfDisparities*16.));
    
    //------------------------------ Display and Save result -------------------
    //.............................


    return 0;
}
