#include <morphology.hpp>

using namespace cv;

void
mm(Mat se, Mat ims, Mat imd, void (*pf)(uchar, uchar*))
{
	int size = se.rows;
	int halfsize = size / 2;

	for (int i = 0; i < ims.rows; ++i) {
		for (int j = 0; j < ims.cols; ++j){
			uchar value =ims.at<uchar>(i, j);
			for (int u = 0; u < size; ++u)
				for (int v = 0; v < size; ++v){
					int local_i = i-halfsize+u, local_j = j-halfsize+v;
					if (local_i >= 0 && local_i < ims.rows)
						if (local_j >= 0 && local_j < ims.cols){
							if (se.at<uchar>(u, v) == 255)
								pf(ims.at<uchar>(local_i, local_j), &value);
						}
				}
			imd.at<uchar>(i, j) = value;
		}
	}
}
//DILATATION
void
maximum(uchar val, uchar* max)
{
	if (val > *max)
		*max = val;
}
//EROSION
void
minimum(uchar val, uchar* min)
{
	if (val < *min)
		*min = val;
}
