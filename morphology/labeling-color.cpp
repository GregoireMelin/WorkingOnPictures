#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int
_find(int p, int* roots)
{
	while(roots[p] != p)
		p = roots[p];
	return p;
}

int
_union(int r0, int r1, int* roots)
{
	if(r0 == r1) return r0;
	if(r0 == -1) return r1;
	if(r1 == -1) return r0;
	if(r0 <	r1){
		roots[r1] = r0;
		return r0;
	}else{
		roots[r0]=r1;
		return r1;
	}
}

int
_add(int p, int r, int* roots)
{
	if(r==-1)
		roots[p]=p;
	else
		roots[p]=r;
	return roots[p];
}

void
process(const char* imsname, const char* regname, const char* colorname)
{

	Mat ims = imread(imsname,0);
	if(!ims.data){
		cerr<<"Image not found, exit"<<endl;
		exit(EXIT_FAILURE);
	}

	//cvtColor(ims, ims, CV_BGR2GRAY);
	Mat imd_eq=ims.clone();
	equalizeHist(ims,imd_eq);
	Mat img_label_gray=ims.clone();
	imwrite("cell-r.png",imd_eq);
	imwrite(regname,ims);
	imwrite(colorname,ims);



	int* roots = new int[ims.total()];
	int rows = ims.rows;
	int cols = ims.cols;
	int p = 0;
	int r = -1;
	uchar* ps = ims.data;

	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			r = -1;

			if( j>0 && (*(ps-1)==(*ps)) )
				r = _union( _find(p-1, roots), r, roots);

			if( (i>0 && j>0) && (*(ps-1-cols)==(*ps)) )
				r = _union( _find(p-1-cols, roots), r, roots);

			if( i>0 && (*(ps-cols) == (*ps)) )
				r = _union(_find(p-cols, roots), r, roots);

			if( (j<(cols-1) && i>0) && (*(ps+1-cols)==(*ps)) )
				r = _union(_find(p+1-cols, roots), r, roots);

			r = _add(p, r, roots);

			p++;
			ps++;
		}
	}

	for(p=0; p<rows*cols; p++){
		roots[p] = _find(p, roots);
	}

	int l=0;
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			int p = i*cols+j;
			if(roots[p]==p)
				roots[p] = l++;
			else
				roots[p] = roots[roots[p]];
			img_label_gray.at<uchar>(i,j)=roots[p];
		}


	}
	//cout<<"labeling: "<< l << " components detected"<<endl;
	imshow("Image en niveau de gris",img_label_gray);
	Mat image_label_gray_eq=img_label_gray.clone();
	equalizeHist(image_label_gray_eq,image_label_gray_eq);
	imshow("Image en niveau de gris",image_label_gray_eq);
	waitKey(0);

	delete [] roots;
	waitKey(0);
}

void
usage (const char *s)
{
	std::cerr<<"Usage: "<<s<<" ims"<<std::endl;
	exit(EXIT_FAILURE);
}

#define param 3
int
main( int argc, char* argv[] )
{
	if(argc != (param+1))
		usage(argv[0]);
	process(argv[1],argv[2],argv[3]);
	return EXIT_SUCCESS;
}
