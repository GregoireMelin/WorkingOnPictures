#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void process(const char* imsname, const char* imdname)
{
  if(imsname==NULL)
  {
    cout<<"Objet vide"<<endl;
    exit(EXIT_FAILURE);
  }
  else
  {
    Mat image = imread(imsname,1);
    if(image.empty())
      cout<<"Image vide"<<endl;
    else
      namedWindow(imsname,WINDOW_AUTOSIZE);
      imshow(imsname,image);
      //Affichage dimensions image (dans la sortie standard)
      cout<<"Longueur: "<<image.cols<<endl;
      cout<<"Largeur: "<<image.rows<<endl;
      //Creation de test.png
      Mat rectangle(100,200,CV_8UC3, Scalar(255,0,255)); //100 lignes-200 colonnes
      imwrite(imdname, rectangle);
      namedWindow(imdname,WINDOW_AUTOSIZE);
      imshow(imdname,rectangle);
      waitKey(0);


  }
}

void usage (const char *s)
{
    cout<<"Usage: "<<s<<"usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}

#define param 2
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1], argv[2]);
  return EXIT_SUCCESS;

}
