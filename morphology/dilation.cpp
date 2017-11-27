#include <iostream>
#include <cstdlib>

#include <morphology.hpp>

using namespace cv;
using namespace std;

void
process(const char* se, const char* ims, const char* imd)
{
	Mat img = imread(ims, 0);
	if (!img.data) {
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}
	Mat img_se = imread(se, 0);
	if (!img_se.data) {
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}

//DILATATION
	Mat img_f = img.clone();
	mm(img_se, img, img_f, maximum);
	imwrite(imd, img_f);
  imshow("Original", img);
	imshow("Dilatation",img_f);
	waitKey(0);
}

void
usage (const char *s)
{
	std::cerr<<"Usage: "<<s<<" im-se ims imd"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 3
int
main( int argc, char* argv[] )
{
	if(argc != (param+1))
		usage(argv[0]);
	process(argv[1], argv[2], argv[3]);
	return EXIT_SUCCESS;
}
