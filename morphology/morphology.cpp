#include <morphology.hpp>

using namespace cv;



void
mm(Mat se, Mat ims, Mat imd, void (*pf)(uchar, uchar*))
{
  //(void)se;
  //(void)ims;
  //(void)imd;
  //(void)pf;
  for(int i=0;i<ims.rows;i++)
  {
    for(int j=0;j<ims.cols;j++)
    {
      uchar val=ims.at<uchar>(i,j);
      for(int u=0;u<se.rows;u++)
      {
        for(int v=0;v<se.cols;v++)
        {
          pf(ims.at<uchar>(i+u,j+v),&val);
        }
      }
      imd.at<uchar>(i,j)=val;
    }
  }
}
//DILATION
void
maximum(uchar val, uchar* max)
{
  //(void)val;
  //(void)max
  if(val>*max)
    *max=val;
}

//EROSION
void
minimum(uchar val, uchar* min)
{
  //(void)val;
  //(void)min;
  if(val<*min)
    *min=val;
}
