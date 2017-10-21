#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void process(const char* ims, const int i0,const int j0,const int h,const int w)
{
  if(ims==NULL)
  {
    cout<<"Objet vide"<<endl;
    exit(EXIT_FAILURE);
  }

  else
  {
    Mat image = imread(ims,1);
    Mat imageCrop(h, w,CV_8UC3);
    //Decoupe par la fonction Rect
    Mat imageCropCV= image(Rect(i0,j0,w,h)).clone();
    Mat imageCropDiff;

    if(image.empty())
      cout<<"Image vide"<<endl;

    //Decoupe pixel par pixel
    for( int i = i0; i < i0+h; i++ )
    {
      for( int j = j0; j < j0+w ; j++)
      {
        imageCrop.at<Vec3b>(i-i0,j-j0) = image.at<Vec3b>(i,j);
      }
    }

    //Production image diff
    subtract(imageCrop,imageCropCV,imageCropDiff);


    // Enregistrement des matrices sous forme d'image
    imwrite("crop.png",imageCrop);
    imwrite("crop-cv.png",imageCropCV);
    imwrite("crop-diff.png",imageCropDiff);
    //Affichage de l'image
    namedWindow(ims,WINDOW_AUTOSIZE);
    imshow(ims,image);
    //Affichage de l'image decoupee pixel par pixel
    namedWindow("crop.png",WINDOW_AUTOSIZE);
    imshow("crop.png",imageCrop);
    //Af//Fonction par defaut de parametrefichage de l'image decoupee avec la fonction rectangle
    namedWindow("crop-cv.png",WINDOW_AUTOSIZE);
    imshow("crop-cv.png",imageCropCV);
    //Affichage de l'image resultant de la difference entre crop.png et crop-cv.png
    namedWindow("diff =((crop) - (crop-cv))",WINDOW_AUTOSIZE);
    imshow("diff =((crop) - (crop-cv))",imageCropDiff);

    waitKey(0);
  }
}

void usage (const char *s)
{
    cout<<"Usage: "<<s<<"usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}

#define param 5
int main( int argc, char* argv[])
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]));
  return EXIT_SUCCESS;
}
