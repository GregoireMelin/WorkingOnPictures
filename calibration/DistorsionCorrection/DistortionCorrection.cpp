#include <stdio.h>
#include <iostream>
#include <sstream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "DistortionAux.hpp"


using namespace cv;
using namespace std;

// Forward declarations
void readme();


// Variables

int K1=1000, K2=5000, K3=1000, P1=100, P2=100;
int F=320, X0=320, Y0=240;
int F_out=320,  X0_out=320, Y0_out=240;
double k1=0, k2=0, k3=0, p1=0, p2=0;
double f=320, _x0=320, _x0_out=320, _y0=240, _y0_out=240;
double f_out=320;

Mat img_in, img_out, img_in2, img_out2;
std::string img_in_name="";

Mat K = (Mat_<double>(3, 3) << f , 0.0 , f , 0.0 , _x0 , _y0 , 0.0 , 0.0 , 1.0);
Mat k = (Mat_<double>(1, 5) << k1, k2, p1, p2, k3); // Distortion coefficients  (k_1, k_2, p_1, p_2[, k_3[, k_4, k_5, k_6]])
Mat Kout = K.clone();

Point2f pos1 = Point2f(100,320), pos2 = Point2f(540,320);

// Functions

// Redisplay img_in and img_out with overlays
void redisplay() {
	// 1. Copy images to draw onto them
	img_in.copyTo(img_in2);
	img_out.copyTo(img_out2);

	// 2. Warp points pos1 and pos2 onto img_out space
	vector<Point2f> pts_in, pts_out;
	vector<Point2f> pos_tmp_in = vector<Point2f>(2), pos_tmp_out = vector<Point2f>(2);
	pos_tmp_in[0]=pos1;
	pos_tmp_in[1]=pos2;

	undistortPointsK(pos_tmp_in, pos_tmp_out, K, k, Kout);

	//std::cout << "pos1_in ="<<pos1<<", pos2_in ="<<pos2<<std::endl;
	//std::cout << "pos1_out="<<pos_tmp_out[0]<<", pos2_out="<<pos_tmp_out[1]<<std::endl;

	// 3. Sample a straight line in img_out space, and warp it back to img_in space

	sampleLine(pts_out, 100, pos_tmp_out[0], pos_tmp_out[1]);

	distortPointsK(pts_out, pts_in, Kout, k, K);

	// 5. Draw line and warped line onto images

	drawPolyline(img_in2, pts_in, false, CV_RGB(255,0,0));
	circle(img_in2,pos1, 5, CV_RGB(255,0,0),2);
	circle(img_in2,pos2, 5, CV_RGB(255,0,0),2);
	drawPolyline(img_out2, pts_out, false, CV_RGB(255,0,0));

	circle(img_in2,Point2f(X0,Y0), 5, CV_RGB(0,255,0),2);
	circle(img_out2,Point2f(X0_out,Y0_out), 5, CV_RGB(0,255,0),2);

	// 6. Overlay status text, and display images

	std::stringstream ss;
	
	// Image names
	ss.str(""); ss.clear();
	ss << img_in_name <<std::ends;
	putText(img_in2, ss.str(), Point(10,20), FONT_HERSHEY_COMPLEX_SMALL, 0.7, CV_RGB(0,0,255));
	ss.str(""); ss.clear();
	ss << "undistord("<<img_in_name<<")"<<std::ends;
	putText(img_out2, ss.str(), Point(10,20), FONT_HERSHEY_COMPLEX_SMALL, 0.7, CV_RGB(0,0,255));
	ss.str(""); ss.clear();

	// Parameters
	ss << "k1="<<k1<<"   k2="<<k2<<"   p1="<<p1<<"   p2="<<p2<<"   k3="<<k3<<std::ends;
	putText(img_out2, ss.str(), Point(10,470), FONT_HERSHEY_COMPLEX_SMALL, 0.7, CV_RGB(255,0,0));
	ss.str(""); ss.clear();
	ss << "f=" <<f<< "   x0="<<_x0<<"   y0="<<_y0<<std::ends;
	putText(img_in2, ss.str(), Point(10,450), FONT_HERSHEY_COMPLEX_SMALL, 0.7, CV_RGB(255,0,0));
	ss.str(""); ss.clear();
	ss << "f=" <<f_out<< "   x0="<<_x0_out<<"   y0="<<_y0_out<<std::ends;
	putText(img_out2, ss.str(), Point(10,450), FONT_HERSHEY_COMPLEX_SMALL, 0.7, CV_RGB(255,0,0));

	// 7. Display images
	imshow( "Image Out", img_out2 );
	imshow( "Image In", img_in2 );

}

void _sliders_to_K() {
	// 1. Convert slider integer values into real parameters
	f  = (float)F / 1.0;
	_x0  = (float)X0;
	_y0  = (float)Y0;
	
	f_out  = (float)F_out / 1.0;
	_x0_out = (float)X0_out;
	_y0_out = (float)Y0_out;
	
	k1 = ((float)K1 - 10000.0) * 10.0 / 10000.0;
	k2 = ((float)K2 - 10000.0) * 10.0 / 10000.0;
	k3 = ((float)K3 - 10000.0) * 10.0 / 10000.0;
	p1 = ((float)P1 - 10000.0) * 10.0 / 10000.0;
	p2 = ((float)P2 - 10000.0) * 10.0 / 10000.0;

	// 2. Define global camera parameters from slider values
	K.at<double>(0,0) = f;
	K.at<double>(1,1) = f;
	K.at<double>(0,2) = _x0;
	K.at<double>(1,2) = _y0;

	k.at<double>(0) = k1;
	k.at<double>(1) = k2;
	k.at<double>(2) = p1;
	k.at<double>(3) = p2;
	k.at<double>(4) = k3;

	Kout.at<double>(0,0) = f_out;
	Kout.at<double>(1,1) = f_out;
	Kout.at<double>(0,2) = _x0_out;
	Kout.at<double>(1,2) = _y0_out;

	//std::cout << "K = "<<K<<std::endl;
	//std::cout << "k = "<<k<<std::endl;
}
void _K_to_sliders() {
	f = K.at<double>(0,0);
	_x0 = K.at<double>(0,2);
	_y0 = K.at<double>(1,2);

	k1 = k.at<double>(0);
	k2 = k.at<double>(1);
	p1 = k.at<double>(2);
	p2 = k.at<double>(3);
	k3 = k.at<double>(4);

	F = f;
	X0=_x0;
	Y0=_y0;
	
	F_out = f; // Copy OUT K that from IN
	X0_out=_x0;
	Y0_out=_y0;
		
	K1=(int)(k1*1000.0+10000);
	K2=(int)(k2*1000.0+10000);
	K3=(int)(k3*1000.0+10000);
	P1=(int)(p1*1000.0+10000);
	P2=(int)(p2*1000.0+10000);

	setTrackbarPos("f (pix)","Image In",F);
	setTrackbarPos("x0","Image In",X0);
	setTrackbarPos("y0","Image In",Y0);

	setTrackbarPos("f (pix)","Image Out",F_out);
	setTrackbarPos("x0","Image Out",X0_out);
	setTrackbarPos("y0","Image Out",Y0_out);

	setTrackbarPos("k1","Distortion",K1);
	setTrackbarPos("k2","Distortion",K2);
	setTrackbarPos("p1","Distortion",P1);
	setTrackbarPos("p2","Distortion",P2);
	setTrackbarPos("k3","Distortion",K3);

	//_sliders_to_K();
}

// Each time a slider is updated
void on_change(int ignored_value) {
	// convert slider values to K and k
	_sliders_to_K();

	// 3. Undistort img_in into img_out, and display

	undistort(img_in, img_out, K, k, Kout);
	//distort(img_out, img_in2, K, k);

	redisplay();
}

std::string dec2bin2(uint16_t n){
	unsigned nbits = sizeof(typeof(n))*8;
    char result[nbits+1];
    unsigned index=nbits;
    result[index]='\0';
    do {
    	result[--index]='0'+(n&1);
    	n>>=1;
    } while (index);
    return std::string(result+index);
}

// Catch mouse clicks to update overlayed segment endpoints
// Left click: update pos1
// Right click: update pos2
void mouse_callback( int event, int x, int y, int flags, void* param )
{
	switch( event ){

	case CV_EVENT_LBUTTONDOWN:
	case CV_EVENT_RBUTTONDOWN:
	case CV_EVENT_MBUTTONDOWN:
	
		//cout << "event="<<event<<endl;
		//cout << "flags="<<dec2bin2(uint16_t(flags))<<endl;
	
		if ((flags & CV_EVENT_FLAG_CTRLKEY) ||
		    (event == CV_EVENT_RBUTTONDOWN)) {
			pos2 = Point2f(x,y);
			std::cout << "pos2="<<pos1<<std::endl;
			redisplay();
		} else {
			pos1 = Point2f(x,y);
			std::cout << "pos1="<<pos1<<std::endl;
			redisplay();
		}
		
		break;
	}
}



int load_img_in(const char *img_name) {
	img_in = imread( img_name); //, CV_LOAD_IMAGE_GRAYSCALE );

	if( !img_in.data )
	{ std::cout<< " --(!) Error reading image '"<<img_name<<"' " << std::endl; return -1; }

	resize(img_in, img_in, cvSize(640,480), 0,0);
	std::cout << "Loaded image '"<<img_name<<"'"<<std::endl;
	std::cout << "Width="<<img_in.cols<<", Height="<<img_in.rows<<std::endl;

	img_in_name = string(img_name);

	// Allocate images with same size
	img_out = img_in.clone();
	img_in2 = img_in.clone();
	img_out2 = img_in.clone();

	return 0;
}

/** @function main */
int main( int argc, char** argv )
{
	int img_id = 0;
	int img_id_max = 0;

	if( argc < 2 )
	{ readme(); return -1; }

	img_id_max = argc-1;

	std::cout << "K = "<<K<<std::endl;
	std::cout << "k = "<<k<<std::endl;

	if (load_img_in(argv[img_id+1]))
		return 1;

	// Create windows
	cvNamedWindow( "Image In", CV_GUI_EXPANDED );
	cvNamedWindow( "Image Out", CV_GUI_EXPANDED );
	cvNamedWindow( "Distortion", CV_GUI_EXPANDED );

	// Create trackbars
	cvCreateTrackbar( "f (pix)",  "Image In", &F , 5000, on_change ); waitKey(1);
	cvCreateTrackbar( "x0",  "Image In", &X0 , 640, on_change );  waitKey(1);
	cvCreateTrackbar( "y0",  "Image In", &Y0 , 480, on_change );  waitKey(1);

	cvCreateTrackbar( "f (pix)",  "Image Out", &F_out , 5000, on_change );  waitKey(1);
	cvCreateTrackbar( "x0",  "Image Out", &X0_out , 640, on_change );  waitKey(1);
	cvCreateTrackbar( "y0",  "Image Out", &Y0_out , 480, on_change );  waitKey(1);

	cvCreateTrackbar( "k1", "Distortion", &K1, 20000, on_change );  waitKey(1);
	cvCreateTrackbar( "k2", "Distortion", &K2, 20000, on_change );  waitKey(1);
	cvCreateTrackbar( "k3", "Distortion", &K3, 20000, on_change ); waitKey(1);
	cvCreateTrackbar( "p1", "Distortion", &P1, 20000, on_change ); waitKey(1);
	cvCreateTrackbar( "p2", "Distortion", &P2, 20000, on_change );  waitKey(1);

	_K_to_sliders();

	// Display images
	imshow( "Image In", img_in );
	imshow( "Image Out", img_out );
	
	waitKey(1);

	// Define click callback of img_in window
	cvSetMouseCallback( "Image In", mouse_callback);

	while (1)
	{
		char key=waitKey(0);
		if (key=='q') {
			break;
		} else if (key=='l') {
			std::cout << "Loading 'camera.yml'..."<<std::endl;

			string filename="camera.yml";

			FileStorage fs;
			fs.open(filename, FileStorage::READ);

			fs["camera_matrix"] >> K;
			fs["distortion_coefficients"] >> k;

			std::cout<<"Loaded:"<<std::endl<<"K="<<K<<std::endl<<"k="<<k<<std::endl;

			if (K.empty()) {
				std::cout << "Could not load K..."<<std::endl;
				continue;
			}
			if (k.empty()) {
				std::cout << "Could not load k..."<<std::endl;
				continue;
			}

			//K.convertTo(K, CV_32FC1);
			//k.convertTo(k,CV_32FC1);

			_K_to_sliders();
			
			on_change(0);
						
			waitKey(1);

		} else if (key=='s') {
			std::cout << "Saving 'camera.yml'..."<<std::endl;

			string filename="camera.yml";

			FileStorage fs(filename, FileStorage::WRITE);

			fs << "camera_matrix" << K;
			fs << "distortion_coefficients" << k;

		} else if ((key==' ')||(key=='n')) { // Space or N (next)
			// Cycle through input images
			img_id = (img_id+1)%img_id_max;

			while (load_img_in(argv[img_id+1])) {
				std::cout << "press 'q' to cancel, other key to try again"<<std::endl;
				key=waitKey(0);
				if (key=='q')
					break;
			}
			on_change(0);
			
			waitKey(1);
		} else if (key=='p') { // P (prev)
			// Cycle through input images
			img_id = (img_id+img_id_max-1)%img_id_max;

			while (load_img_in(argv[img_id+1])) {
				std::cout << "press 'q' to cancel, other key to try again"<<std::endl;
				key=waitKey(0);
				if (key=='q')
					break;
			}
			on_change(0);
			
			waitKey(1);
		}
	}


	return 0;
}

/** @function readme */
void readme()
{ std::cout << " Usage: ./DistortionCorrection <img1> [<img2> ...]" << std::endl; }

