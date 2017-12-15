
/* Calibration */


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

// Print camera parameters to the output file
static void saveCameraParams( string outputFileName,Size imageSize,
			      Size  boardSize, float squareSize,  Mat& cameraMatrix, Mat& distCoeffs,
            const vector<Mat>& rvecs, const vector<Mat>& tvecs,
          	const vector<float>& reprojErrs, const vector<vector<Point2f> >& imagePoints)
{
  FileStorage fs( outputFileName, FileStorage::WRITE );

  time_t t;
  time( &t );
  struct tm *t2 = localtime( &t );
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
	{
    fs << "per_view_reprojection_errors" << Mat(reprojErrs);
	}

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
