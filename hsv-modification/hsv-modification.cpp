#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;


void process(const int h,const int s,const int v, const char* ims,const char* imd)
{
  if(ims==NULL)
  {
    cout<<"Objet vide"<<endl;
    exit(EXIT_FAILURE);
  }
  else
  {
    //lecture de l'image
    Mat image = imread(ims,1);
    Mat imageHSV;

    //Conversion de l'image en HSV
    cvtColor(image,imageHSV,CV_BGR2HSV);

    //Stockage des valeurs hsv dans les matrices h_ch, s_ch, et v_ch
    Mat hsv_ch[3];
    split(imageHSV,hsv_ch);
    Mat h_ch = hsv_ch[0];
    Mat s_ch = hsv_ch[1];
    Mat v_ch = hsv_ch[2];

    //Affichage canal h
    namedWindow("h",WINDOW_AUTOSIZE);
    imshow("h",h_ch);

    //Affichage canal s
    namedWindow("s",WINDOW_AUTOSIZE);
    imshow("s",s_ch);

    //Affichage canal v
    namedWindow("v",WINDOW_AUTOSIZE);
    imshow("v",v_ch);

    Mat imageHSV_copy = imageHSV.clone();
    Mat image_modified;
    Vec3b value_ch;

    //modification des valeurs des canaux hsv dans imageHSV_copy
    for ( int j = 0; j < imageHSV.rows; ++j)
    {
      for ( int i = 0; i < imageHSV.cols; ++i)
              {
                value_ch = imageHSV_copy.at<Vec3b>(j, i);

                value_ch.val[0] +=  h;
                value_ch.val[1] +=  s;
                value_ch.val[2] +=  v;

                imageHSV_copy.at<Vec3b>(j, i) = value_ch;

              }
    }

    cvtColor(imageHSV_copy,image_modified,CV_HSV2BGR);

    //ecriture et affichage de l'image modifiee
    imwrite(imd, image_modified);
    namedWindow(imd,WINDOW_AUTOSIZE);
    imshow(imd,image_modified);
    }
}

void usage (const char *s)
{
    cout<<"Usage: "<<s<<"usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}
#define param 5
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),argv[4],argv[5]);

  return EXIT_SUCCESS;
}
