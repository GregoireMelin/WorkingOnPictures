#include <morphology.hpp>

using namespace cv;

void
mm(Mat se, Mat ims, Mat imd, void (*pf)(uchar, uchar*))
{
	int size = se.rows, halfsize = size / 2;

	for (int i = 0; i < ims.rows; ++i) {
		for (int j = 0; j < ims.cols; ++j){
			uchar val =ims.at<uchar>(i, j);
			for (int u = 0; u < size; ++u)
				for (int v = 0; v < size; ++v){
					int iloc = i-halfsize+u, jloc = j-halfsize+v;
					if (iloc >= 0 && iloc < ims.rows)
						if (jloc >= 0 && jloc < ims.cols){
							if (se.at<uchar>(u, v) == 255){
								pf(ims.at<uchar>(iloc, jloc), &val);
							}
						}
				}
			imd.at<uchar>(i, j) = val;
		}
	}
}
//DILATATION
void
maximum(uchar val, uchar* max)
{
	if (val > *max) *max = val;
}
//EROSION
void
minimum(uchar val, uchar* min)
{
	if (val < *min) *min = val;
}
