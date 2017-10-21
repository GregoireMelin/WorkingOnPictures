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
    Mat imageResized;

    if(image.empty())
      cout<<"Image vide"<<endl;
    else

      resize(image,imageResized,Size(),80,80);


      // Methode at
  /*    e1=(double)getTickCount();
	     for ( int j = 0; j < imageResized.rows; ++j)
       {
		     for ( int i = 0; i < imageResized.cols; ++i)
	        {
		          Vec3b * pixel = &imageResized.at<Vec3b>(j, i);
		          op(&(*pixel)[0]);
              op(&(*pixel)[1]);
              op(&(*pixel)[2]);
	        }
        }
      e2=(double)getTickCount();
      period = (e2 - e1)/getTickFrequency();

      cout << period <<endl;

	// iterator
	t = (double)(getTickCount());
	Mat_<Vec3b>::iterator it, end;
	for (it = img_out2.begin<Vec3b>(), end = img_out2.end<Vec3b>(); it != end; ++it)
	{
		op(&(*it)[0]);
		op(&(*it)[1]);
		op(&(*it)[2]);
	}
	t = ((double)getTickCount() - t)/getTickFrequency();
	cout<<"Time with iterator: " << t*1000 << "ms" <<endl;*/
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
