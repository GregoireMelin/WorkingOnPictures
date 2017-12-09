#include <iostream>
#include <cstdlib>
#include <math.h>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat coloringLineDetectedInRed(Mat image_input,Mat H, Mat H_pt, Mat dest)
{
	//Definition du rouge
	 Vec3b red(0,0,255);
	//parametre permettant de recentrer l'accumulateur
	double centering_i = image_input.rows/2.0;
	double centering_j = image_input.cols/2.0;
	//Colorisation
	for (int rho = 0; rho < H.rows; rho++)
	{
		for (int theta = 0 ; theta < H.cols; theta++)
		{
			if (H_pt.at<uchar>(rho, theta) == 255)
				for ( int i = 0; i < image_input.rows; i++)
				{
					for ( int j = 0 ; j < image_input.cols; j++)
					{
						double new_theta = (double)(theta-90)*CV_PI/180;
						if (round(rho-sqrt((image_input.cols * image_input.cols)+(image_input.rows * image_input.rows))/2.0) == round(((double)j-centering_j)*cos(new_theta) + ((double)i-centering_i)*sin(new_theta)))
							dest.at<Vec3b>(i,j) = red;
					}
				}
		 }
	}
	 return(dest);
}
Mat computeHoughTransform(Mat image_input, int th, Mat dest)
{
	//parametre permettant de recentrer l'accumulateur
	double centering_i = image_input.rows/2.0;
	double centering_j = image_input.cols/2.0;

	for ( int i = 0; i < image_input.rows; i++)
	{
		for ( int j = 0; j < image_input.cols; j++)
		{
			if (image_input.at<uchar>(i,j)>=th)
			{
				for (int theta = 0; theta <dest.cols; theta++ )
				{
					double new_theta = (double)(theta-90)*CV_PI/180;
					double rho = ((double)j-centering_j)*cos(new_theta) + ((double)i-centering_i)*sin(new_theta);
					int new_rho = round(rho+sqrt((image_input.cols * image_input.cols)+(image_input.rows * image_input.rows))/2.0);

					if (new_rho >= dest.rows)
					{
						dest.at<uchar>(dest.rows-1,theta)+=1;
					}

					if (new_rho < 0)
					{
						dest.at<uchar>(0,theta)+=1;
					}

					else
					{
						dest.at<uchar>(new_rho,theta)+=1;
					}
				}
			}
		}
	}
	 return(dest);
}
void process (const char* ims, const unsigned th_mag)
{
	// Lire l'image ims en niveau de gris
	Mat image_input = imread(ims,0);
	if (!image_input.data){
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}

	//Transformee de Hough
	Mat H = Mat::zeros((sqrt((image_input.cols * image_input.cols)+(image_input.rows * image_input.rows))/2.0)*2, 180, CV_8UC1);
	computeHoughTransform(image_input,th_mag,H);

	//Egalisation d'histogramme
	Mat H_eq = H.clone();
	equalizeHist(H, H_eq);

	//Seuillage de l'accumulateur
	double min, max;
	minMaxLoc(H, &min, &max);
	Mat H_pt;
	threshold(H, H_pt, max-4, 255, CV_THRESH_BINARY);

	//Colorisation des lignes detectees
	Mat result = Mat::zeros(image_input.rows, image_input.cols, CV_8UC3);
	coloringLineDetectedInRed(image_input,H,H_pt,result);

	//Affichages & Enregistrements
	imwrite("accumulateur.png", H);
	imwrite("accumulateur_hist_eq.png", H_eq);
	imwrite("lignes_detectees.png", result);
	/*imshow("accumulateur.png", H);
	imshow("accumulateur_hist_eq.png", H_eq);
	imshow("lignes_detectees.png", result);
	waitKey(0);*/
}

void usage (const char *s)
{
	cerr<<"Usage: "<<s<<" ims-name th"<<endl;
	exit(EXIT_FAILURE);
}

 #define param 2

 int main (int argc, char* argv[])
 {
	 if (argc !=(param+1))
	 	usage (argv[0]);
	 process (argv[1], atoi(argv[2]));
	 return EXIT_SUCCESS;
 }
