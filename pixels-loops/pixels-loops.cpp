#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

double e1;
double e2;
double period;

void process(const char* imsname)
{
  if(imsname==NULL)
  {
    cout<<"Objet vide"<<endl;
    exit(EXIT_FAILURE);
  }
  else
  {
    Mat image = imread(imsname,1);
    Mat imageF;
    image.convertTo(imageF, CV_32FC1);

    Mat imageResized;

    if(image.empty())
      cout<<"Image vide"<<endl;
    else
      resize(imageF,imageResized,Size(),2,2);
      imageResized.convertTo(imageResized, CV_32FC1);
      //AT
      Vec3f intensity;
      Mat MethAt = imageResized.clone();
      float blue;
      float green;
      float red;
      e1=(double)getTickCount();
	      for ( int j = 0; j < imageResized.rows; ++j)
       	      {
		      for ( int i = 0; i < imageResized.cols; ++i)
	              {
		                  intensity=MethAt.at<Vec3f>(j, i);
                      blue = intensity.val[0];
                      green = intensity.val[1];
                      red = intensity.val[2];

                      blue = 1/64 * blue * 64 + 64/2;
                      green = 1/64 * green * 64 + 64/2;
                      red = 1/64 * red * 64 + 64/2;

                      intensity.val[0] = blue;
                      intensity.val[1] = green;
                      intensity.val[2] = red;

                      MethAt.at<Vec3f>(j, i) = intensity;
	              }
              }
      e2=(double)getTickCount();
      period = (e2 - e1)/getTickFrequency();

      cout<<"Temps methode at: "<< period <<endl;

      //Operateur matrices
      Mat MethOpMat = imageResized.clone();



      e1=(double)getTickCount();

      MethOpMat = 1/64 * MethOpMat * 64 + 64/2;

      e2=(double)getTickCount();
      period = (e2 - e1)/getTickFrequency();

      cout<<"Temps methode operateur matriciel: "<< period <<endl;

      //Methode Iterateur
      Mat MethIt = imageResized.clone();
      Mat_<Vec3f>::iterator it, end;

      e1=(double)getTickCount();
      for (it = MethIt.begin<Vec3f>(), end = MethIt.end<Vec3f>(); it != end; ++it)
      {
        blue = (*it)[0];
        green = (*it)[1];
        red = (*it)[2];

        (*it)[0] = 1/64* blue * 64+64/2; //je prend la valeur associe au pointeur
        (*it)[1] = 1/64* green * 64+64/2;
        (*it)[2] = 1/64* red * 64+64/2;
      }
      e2=(double)getTickCount();
      period = (e2 - e1)/getTickFrequency();

      cout<<"Temps methode iterateur: "<< period <<endl;

      //Methode Pointeur
      Mat MethP = imageResized.clone();

      e1=(double)getTickCount();

      for ( int j = 0; j < imageResized.rows; ++j)
            {
              Vec3f * p = MethP.ptr<Vec3f>(j); //pointeur sur la ligne
        for ( int i = 0; i < imageResized.cols; ++i)
              {
                blue = (p[i][0]);
                green = (p[i][1]);
                red = (p[i][2]);

                (p[i][0]) = 1/64* blue * 64+64/2; //je prend la valeur associe au pointeur
                (p[i][1]) = 1/64* green * 64+64/2;
                (p[i][2]) = 1/64* red * 64+64/2;
              }
            }
            e2=(double)getTickCount();
            period = (e2 - e1)/getTickFrequency();

            cout<<"Temps methode pointeur: "<< period <<endl;




    }
}

void usage (const char *s)
{
    cout<<"Usage: "<<s<<"usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}
#define param 1
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1]);
  return EXIT_SUCCESS;
}
