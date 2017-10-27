#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;


void process(const char* ims,const int radius, const int constant)
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
    
    cout<<"adaptative-th"<<endl; 

    waitKey(0);
    }
}

void usage (const char *s)
{
    cout<<"Usage: "<<s<<"usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}
#define param 3
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1],atoi(argv[2]),atoi(argv[3]));

  return EXIT_SUCCESS;
}
