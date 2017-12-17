/* Calibration */
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// STL INCLUDES
#include <iostream>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>

using namespace std;
using namespace cv;

class Settings
{
	public: Settings() : goodInput(false) {}
    enum Pattern { NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
    enum InputType {INVALID, CAMERA, VIDEO_FILE, IMAGE_LIST};

    void write(FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" << "BoardSize_Width"  << boardSize.width
                  << "BoardSize_Height" << boardSize.height
                  << "Square_Size"         << squareSize
                  << "Calibrate_Pattern" << patternToUse
                  << "Calibrate_NrOfFrameToUse" << nrFrames
                  << "Calibrate_FixAspectRatio" << aspectRatio
                  << "Calibrate_AssumeZeroTangentialDistortion" << calibZeroTangentDist
                  << "Calibrate_FixPrincipalPointAtTheCenter" << calibFixPrincipalPoint

                  << "Write_DetectedFeaturePoints" << bwritePoints
                  << "Write_extrinsicParameters"   << bwriteExtrinsics
                  << "Write_outputFileName"  << outputFileName

                  << "Show_UndistortedImage" << showUndistorsed

                  << "Input_FlipAroundHorizontalAxis" << flipVertical
                  << "Input_Delay" << delay
                  << "Input" << input
           << "}";
    }
    void read(const FileNode& node)                          //Read serialization for this class
    {
        node["BoardSize_Width" ] >> boardSize.width;
        node["BoardSize_Height"] >> boardSize.height;
        node["Calibrate_Pattern"] >> patternToUse;
        node["Square_Size"]  >> squareSize;
        node["Calibrate_NrOfFrameToUse"] >> nrFrames;
        node["Calibrate_FixAspectRatio"] >> aspectRatio;
        node["Write_DetectedFeaturePoints"] >> bwritePoints;
        node["Write_extrinsicParameters"] >> bwriteExtrinsics;
        node["Write_outputFileName"] >> outputFileName;
        node["Calibrate_AssumeZeroTangentialDistortion"] >> calibZeroTangentDist;
        node["Calibrate_FixPrincipalPointAtTheCenter"] >> calibFixPrincipalPoint;
        node["Input_FlipAroundHorizontalAxis"] >> flipVertical;
        node["Show_UndistortedImage"] >> showUndistorsed;
        node["Input"] >> input;
        node["Input_Delay"] >> delay;
        interprate();
    }
    void interprate()
    {
        goodInput = true;
        if (boardSize.width <= 0 || boardSize.height <= 0)
        {
            cerr << "Invalid Board size: " << boardSize.width << " " << boardSize.height << endl;
            goodInput = false;
        }
        if (squareSize <= 10e-6)
        {
            cerr << "Invalid square size " << squareSize << endl;
            goodInput = false;
        }
        if (nrFrames <= 0)
        {
            cerr << "Invalid number of frames " << nrFrames << endl;
            goodInput = false;
        }

        if (input.empty())      // Check for valid input
                inputType = INVALID;
        else
        {
            if (input[0] >= '0' && input[0] <= '9')
            {
                stringstream ss(input);
                ss >> cameraID;
                inputType = CAMERA;
            }
            else
            {
                if (isListOfImages(input) && readStringList(input, imageList))
                    {
                        inputType = IMAGE_LIST;
                        nrFrames = (nrFrames < (int)imageList.size()) ? nrFrames : (int)imageList.size();
                    }
                else
                    inputType = VIDEO_FILE;
            }
            if (inputType == CAMERA)
                inputCapture.open(cameraID);
            if (inputType == VIDEO_FILE)
                inputCapture.open(input);
            if (inputType != IMAGE_LIST && !inputCapture.isOpened())
                    inputType = INVALID;
        }
        if (inputType == INVALID)
        {
            cerr << " Inexistent input: " << input;
            goodInput = false;
        }

        flag = 0;
        if(calibFixPrincipalPoint) flag |= CV_CALIB_FIX_PRINCIPAL_POINT;
        if(calibZeroTangentDist)   flag |= CV_CALIB_ZERO_TANGENT_DIST;
        if(aspectRatio)            flag |= CV_CALIB_FIX_ASPECT_RATIO;


        calibrationPattern = NOT_EXISTING;
        if (!patternToUse.compare("CHESSBOARD")) calibrationPattern = CHESSBOARD;
        if (!patternToUse.compare("CIRCLES_GRID")) calibrationPattern = CIRCLES_GRID;
        if (!patternToUse.compare("ASYMMETRIC_CIRCLES_GRID")) calibrationPattern = ASYMMETRIC_CIRCLES_GRID;
        if (calibrationPattern == NOT_EXISTING)
            {
                cerr << " Inexistent camera calibration mode: " << patternToUse << endl;
                goodInput = false;
            }
        atImageList = 0;

    }
    Mat nextImage()
    {
        Mat result;
        if( inputCapture.isOpened() )
        {
            Mat view0;
            inputCapture >> view0;
            view0.copyTo(result);
        }
        else if( atImageList < (int)imageList.size() )
            result = imread(imageList[atImageList++], CV_LOAD_IMAGE_COLOR);

        return result;
    }

    static bool readStringList( const string& filename, vector<string>& l )
    {
        l.clear();
        FileStorage fs(filename, FileStorage::READ);
        if( !fs.isOpened() )
            return false;
        FileNode n = fs.getFirstTopLevelNode();
        if( n.type() != FileNode::SEQ )
            return false;
        FileNodeIterator it = n.begin(), it_end = n.end();
        for( ; it != it_end; ++it )
            l.push_back((string)*it);
        return true;
    }

    static bool isListOfImages( const string& filename)
    {
        string s(filename);
        // Look for file extension
        if( s.find(".xml") == string::npos && s.find(".yaml") == string::npos && s.find(".yml") == string::npos )
            return false;
        else
            return true;
    }
	public:
    Size boardSize;            // The size of the board -> Number of items by width and height
    Pattern calibrationPattern;// One of the Chessboard, circles, or asymmetric circle pattern
    float squareSize;          // The size of a square in your defined unit (point, millimeter,etc).
    int nrFrames;              // The number of frames to use from the input for calibration
    float aspectRatio;         // The aspect ratio
    int delay;                 // In case of a video input
    bool bwritePoints;         //  Write detected feature points
    bool bwriteExtrinsics;     // Write extrinsic parameters
    bool calibZeroTangentDist; // Assume zero tangential distortion
    bool calibFixPrincipalPoint;// Fix the principal point at the center
    bool flipVertical;          // Flip the captured images around the horizontal axis
    string outputFileName;      // The name of the file where to write
    bool showUndistorsed;       // Show undistorted images after calibration
    string input;               // The input ->



    int cameraID;
    vector<string> imageList;
    int atImageList;
    VideoCapture inputCapture;
    InputType inputType;
    bool goodInput;
    int flag;
	private: string patternToUse;
};

static void read(const FileNode& node, Settings& x, const Settings& default_value = Settings())
{
    if(node.empty())
        x = default_value;
    else
        x.read(node);
}
enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

bool runCalibrationAndSave(Settings& s, Size imageSize, Mat&  cameraMatrix, Mat& distCoeffs,
                           vector<vector<Point2f> > imagePoints );

// Print camera parameters to the output file
static void saveCameraParams( string outputFileName,Size imageSize,
			      Size  boardSize, float squareSize,  Mat& cameraMatrix, Mat& distCoeffs,
            const vector<Mat>& rvecs, const vector<Mat>& tvecs,
          	const vector<float>& reprojErrs, const vector<vector<Point2f> >& imagePoints)
{
  FileStorage fs( outputFileName, FileStorage::WRITE );

  time_t tm;
  time( &tm );
  struct tm *t2 = localtime( &tm );
  char buf[1024];
  strftime( buf, sizeof(buf)-1, "%c", t2 );

  fs << "calibration_time" << buf;

  if( !rvecs.empty() || !reprojErrs.empty() )
    fs << "nframes" << (int)std::max(rvecs.size(), reprojErrs.size());

  fs << "image_Width" << imageSize.width;
  fs << "image_Height" << imageSize.height;
  fs << "board_Width" << boardSize.width;
  fs << "board_Height" << boardSize.height;
  fs << "square_Size" << squareSize;


  fs << "flags" << 0;

  fs << "camera_matrix" << cameraMatrix;
  fs << "distortion_coefficients" << distCoeffs;


  if( !reprojErrs.empty() )
    fs << "per_view_reprojection_errors" << Mat(reprojErrs);

  if( !rvecs.empty() && !tvecs.empty() )
  {
      CV_Assert(rvecs[0].type() == tvecs[0].type());
      Mat bigmat((int)rvecs.size(), 6, rvecs[0].type());

      for( int i = 0; i < (int)rvecs.size(); i++ )
      {
	  		Mat r = bigmat(Range(i, i+1), Range(0,3));
	  		Mat t = bigmat(Range(i, i+1), Range(3,6));

	  		CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
	  		CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
	  		//*.t() is MatExpr (not Mat) so we can use assignment operator
	  		r = rvecs[i].t();
	  		t = tvecs[i].t();
      }

      cvWriteComment( *fs, "a set of 6-tuples (rotation vector + translation vector) for each view", 0 );
      fs << "extrinsic_parameters" << bigmat;
  }

  if( !imagePoints.empty() )
  {
    Mat imagePtMat((int)imagePoints.size(), imagePoints[0].size(), CV_32FC2);

    for( int i = 0; i < (int)imagePoints.size(); i++ )
    {
	  	Mat r = imagePtMat.row(i).reshape(2, imagePtMat.cols);
	  	Mat imgpti(imagePoints[i]);
	  	imgpti.copyTo(r);
    }

		fs << "image_points" << imagePtMat;

	}
}

//Lis un fichier ligne par ligne
static bool readStringList( const string& filename, vector<string>& l )
{
  l.resize(0);
  FileStorage fs(filename, FileStorage::READ);
  if( !fs.isOpened() )
    return false;
  FileNode n = fs.getFirstTopLevelNode();
  if( n.type() != FileNode::SEQ )
    return false;
  FileNodeIterator it = n.begin(), it_end = n.end();
  for( ; it != it_end; ++it )
    l.push_back((string)*it);
  return true;
}

/** @function main */
int main(int argc, char* argv[])
{
	Settings s;
  std::cout << " Usage: ./calibrate <calib_list.xml>  <image_list.xml> or: ./calibrate <calib_list.xml>   " << argc << std::endl;

  if(argc != 2 && argc != 3 ){
    cout << "ERROR: Wrong number of input parameters" << endl;
    return -1;
  }
  string output = "camera.yml";

  const char* calibFilename = argv[1];
  vector<string> calibList;
  readStringList(calibFilename, calibList);

  int nframes = 0;
  if( !calibList.empty() ) {
    nframes = (int)calibList.size();
    cout<< nframes <<  " images" << endl;
  }
  else {
    cout <<  "No images" << endl;
    return 1;
  }


  Size boardSize, imageSize;
  boardSize.width=9;
  boardSize.height=6;
  float squareSize=0.025;


  vector<vector<Point2f> > imagePoints;
  Mat cameraMatrix, distCoeffs;

  for(int i = 0; i < nframes ;i++)
	{

    cout << "image "<< i << endl;
    Mat im = imread(calibList[i], 1);

    //-----  If no more image, or got enough, then stop calibration and show result -------------
    if(!im.empty())
    {
			imageSize = im.size();
// ---- Compute Chessboard Corners ---- //
			if( imagePoints.size() > 0 )
				//runCalibrationAndSave(s, imageSize,  cameraMatrix, distCoeffs, imagePoints);
				vector<Mat> rvecs, tvecs;
		    vector<float> reprojErrs;
		    double totalAvgErr = 0;
				//bool ok = runCalibration(s,imageSize, cameraMatrix, distCoeffs, imagePoints, rvecs, tvecs,reprojErrs, totalAvgErr);
				bool ok = false;

				cout << (ok ? "Calibration succeeded" : "Calibration failed") << ". avg re projection error = "  << totalAvgErr ;

				if( ok )
						//saveCameraParams( s, imageSize, cameraMatrix, distCoeffs, rvecs ,tvecs, reprojErrs,imagePoints, totalAvgErr);

//.............................
			bool found;

			if ( found)
			{ // If done with success,
	  		Mat imGray;
	  		cvtColor(im, imGray, CV_BGR2GRAY);

	  		// ----  improve the found corners' coordinate accuracy for chessboard---- //
	  		//.............................


	  		// ---- Draw the corners  ---- //
	  		//.............................

	  		// --- Show image ---- //
	  		imshow("Image ", im);
	  		cv::waitKey();
			}
    }
  }

  if ( imagePoints.size() > 0 )
	{
    // ---- Calibrate the camera --- //
    // ---- Compute Object Points --- //
    vector<vector<Point3f> > objectPoints(1);
    for( int i = 0; i < boardSize.height; ++i )
		{
      for( int j = 0; j < boardSize.width; ++j )
			{
				objectPoints[0].push_back(Point3f(float( j*squareSize ), float( i*squareSize ), 0));
			}
    		objectPoints.resize(imagePoints.size(),objectPoints[0]);
		}

    // ---- Find intrinsic and extrinsic camera parameters ---- //
    cameraMatrix = Mat::eye(3, 3, CV_64F);
    distCoeffs = Mat::zeros(8, 1, CV_64F);
    //.............................
    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);
    cout << (ok ? "Calibration succeeded" : "Calibration failed")  << endl ;
    // ---- If calibration succeeded save camera parameters ---- //
    if( ok )
		{
      saveCameraParams(output,  imageSize, boardSize, squareSize,  cameraMatrix, distCoeffs, rvecs ,tvecs, reprojErrs,imagePoints);
		}

  }

  cout<<"calibration done"<< endl;
  if (argc > 2)
	{
  	//---- RECTIFICATION (QUESTION 2) Undistort images from camera claibration parameter ----- //
    const char* imageFilename = argv[2];
    vector<string> imageList;
    cout << imageFilename << endl;
    readStringList(imageFilename, imageList);
    nframes = 0;
    if( !imageList.empty() )
		{
      nframes = (int)imageList.size();
    	cout<< nframes <<  " images" << endl;
    }

		else
		{
      cout <<  "No images" << endl;
      return 1;
    }

    //.............................

  }

  return 0;
}
