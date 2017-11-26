#include <iostream>
#include <cstdlib>

#include <morphology.hpp>

using namespace cv;
using namespace std;

void
process(const char* se, const char* ims, const char* imd)
{
	// Lire l'image ims en niveau de gris
	Mat img_in = imread(ims, 0);
	if (!img_in.data) {
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}
	// Lire l'element structurant se en niveau de gris
	Mat img_se = imread(se, 0);
	if (!img_se.data) {
		cout<<"Could not open or find the image"<<endl;
		exit(EXIT_FAILURE);
	}

	// Realise la fermeture
	Mat img_out = img_in.clone();
	mm(img_se, img_in, img_out, maximum);
	mm(img_se, img_out, img_in, minimum);

	// Sauvegarder l'image imd
	imwrite(imd, img_in);
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
