#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

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
    Mat imageResized;

    if(image.empty())
      cout<<"Image vide"<<endl;
    else
      resize(image,imageResized,Size(),100,100);
      imwrite("resized.png",imageResized);
      namedWindow("resized.png",WINDOW_AUTOSIZE);
      imshow("resized.png",imageResized);
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
