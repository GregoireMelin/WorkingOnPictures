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
    
    cout<<"hsv-modification"<<endl; //donne 1

    waitKey(0);
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
