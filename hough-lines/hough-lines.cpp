#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat accumulateur(Mat image, int ts)
{
	Mat H=Mat::zeros(round(sqrt(image.cols*image.cols+image.rows*image.rows)),180, CV_8UC1);
	for (int i = 0; i<image.rows; i++)
		for (int j = 0; j<image.cols; j++)
			if(image.at<uchar>(i,j)>=ts)
			{
				for (int theta = 0; theta <H.cols; theta++) //theta : 0 --> 180
				{
					int rho = abs(round(i*cos(theta*(CV_PI/180))+j*sin(theta*(CV_PI/180))));
					if (rho>=0 && rho<H.rows)
					H.at<uchar>(rho,theta)+=1;
				}
			}
		return(H);
}

Mat coloringLineDetected(Mat image, Mat H, Mat H_pt,Vec3b color)
{

	Mat dest = Mat::zeros(image.rows, image.cols, CV_8UC3);
	int new_rho,new_theta;
	for (int rho = 0; rho < H.rows; rho++)
	{
			for (int theta = 0 ; theta < H.cols; theta++)
			{
					if (H_pt.at<uchar>(rho, theta) == 255)
					{
							new_theta = theta;
							new_rho = rho;
							for (int i = 0; i < image.rows; i++)
									for (int j = 0 ; j < image.cols; j++)
											if (new_rho == (int) abs(round(i*cos(new_theta*(CV_PI/180)) + j*sin(new_theta*(CV_PI/180)))))
													dest.at<Vec3b>(i,j) = color;
					}
			}
	}
	return(dest);
}

void process(const char* ims, int th_mag)
{
	//Chargement de l'image en niveaux de gris
	Mat image = imread(ims,CV_LOAD_IMAGE_GRAYSCALE);

  //Verification de la preence de l'image
  if (!image.data)
	{
			cout << "Could not open the file" <<endl;
			exit (EXIT_FAILURE);
	}

  	//Calcul de rho=i*cos(theta)+j*sin(theta) et creation histogramme
		Mat H=accumulateur(image,th_mag);

    //Egalisation d'histogramme
    Mat H_equalized = H.clone();
    equalizeHist(H, H_equalized);
    Mat H_pt,dest;
    double min, max;
    //Determination du minimum et du maximum local
    cv::minMaxLoc(H, &min, &max);
    //Seuillage
    threshold(H, H_pt, max-4, 255, CV_THRESH_BINARY);
    //ROUGE : R = 255; G = 0; B = 0
    Vec3b red(0,0,255);
		//COLORATION DES LIGNES DETECTEES EN ROUGE
		dest=coloringLineDetected(image,H,H_pt,red);
    //AFFICHAGES et ENREGISTREMENTS
    /*imwrite("result.png",H);
    imwrite("result_equalized.png",H_equalized);
    imwrite("result_max.png",H_pt);
    imwrite("result_detected_lines",dest);*/
    imshow("Resultat", H);
    imshow("Resultat avec Histogramme egalise", H_equalized);
    imshow("Maximum locaux", H_pt);
    imshow("Lignes detectees", dest);
    waitKey(0);
}

void usage (const char *s)
{
	cout<<"Usage: "<<s<<"ims th_mag"<<endl;
	exit(EXIT_FAILURE);
}

#define param 2
int main( int argc, char* argv[] )
{
	if (argc != (param+1))
		usage (argv[0]);
	process (argv[1], atof(argv[2]));
	return EXIT_SUCCESS;
}
