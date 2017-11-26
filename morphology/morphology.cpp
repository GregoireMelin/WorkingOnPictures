#include <morphology.hpp>

using namespace cv;

void
mm(Mat se, Mat ims, Mat imd, void (*pf)(uchar, uchar*))
{
	int size = se.rows, halfsize = size / 2;

	for (int i = 0; i < ims.rows; ++i) {
		for (int j = 0; j < ims.cols; ++j){
			uchar val =ims.at<unsigned char>(i, j);
			for (int u = 0; u < size; ++u)
				for (int v = 0; v < size; ++v){
					int iloc = i-halfsize+u, jloc = j-halfsize+v;
					if (iloc >= 0 && iloc < ims.rows)
						if (jloc >= 0 && jloc < ims.cols){
							if (se.at<unsigned char>(u, v) == 255){
								pf(ims.at<unsigned char>(iloc, jloc), &val);
							}
						}
				}
			imd.at<unsigned char>(i, j) = val;
		}
	}
}

void
maximum(uchar val, uchar* max)
{
	if (val > *max) *max = val;
}

void
minimum(uchar val, uchar* min)
{
	if (val < *min) *min = val;
}
