
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
    
    // Block matching stereo correspondence algorithm class StereoBM
    //{
        /*enum { NORMALIZED_RESPONSE = CV_STEREO_BM_NORMALIZED_RESPONSE,
            BASIC_PRESET=CV_STEREO_BM_BASIC,
            FISH_EYE_PRESET=CV_STEREO_BM_FISH_EYE,
            NARROW_PRESET=CV_STEREO_BM_NARROW };*/

        //StereoBM();
        // the preset is one of ..._PRESET above.
        // ndisparities is the size of disparity range,
        // in which the optimal disparity at each pixel is searched for.
        // SADWindowSize is the size of averaging window used to match pixel blocks
        //    (larger values mean better robustness to noise, but yield blurry disparity maps)
        /*StereoBM(int preset, int ndisparities=0, int SADWindowSize=21);
        // separate initialization function
        init(int preset, int ndisparities=0, int SADWindowSize=21);
        // computes the disparity for the two rectified 8-bit single-channel images.
        // the disparity will be 16-bit signed (fixed-point) or 32-bit floating-point image of the same size as left.
        void operator()( InputArray left, InputArray right, OutputArray disparity, int disptype=CV_16S );*/

        //Ptr<CvStereoBMState> state;
    //};

    Ptr<CvStereoBMState> state;
    StereoBM(CV_STEREO_BM_BASIC, numberOfDisparities, SADWindowSize);

    //.............................
    Mat disp, disp8;
    //disp=......

    StereoBM::operator()(img1, img2, disp, CV_16S );

    disp.convertTo(disp8, CV_8U, 255/(numberOfDisparities*16.));
    //

    /*left – Left 8-bit single-channel image.
    right – Right image of the same size and the same type as the left one.
    disparity – Output disparity map. It has the same size as the input images. When disptype==CV_16S, the map is a 16-bit signed single-channel image, containing disparity values scaled by 16. To get the true disparity values from such fixed-point representation, you will need to divide each disp element by 16. If disptype==CV_32F, the disparity map will already contain the real disparity values on output.
    disptype – Type of the output disparity map, CV_16S (default) or CV_32F.*/

    //------------------------------ Display and Save result -------------------
    //.............................
    imshow(disparity_filename, disp8);
    imwrite(disparity_filename, disp8);

    return 0;
}
