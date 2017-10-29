#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

float intensity;
float thresholdOtsu;

Mat image;
Mat makeHist(Mat image)
{
   Mat valHist = Mat::zeros(1,256, CV_32F);
   int intensity;
   for ( int j = 0; j < image.rows; ++j)
   {
       for ( int i = 0; i < image.cols; ++i)
       {
         intensity = (float)image.at<uchar>(j, i);
         valHist.at<float>(intensity) = valHist.at<float>(intensity)+1;
       }
   }
   return valHist;
}

Mat makeHistNorm(Mat hist)
{
   Mat histN = Mat::zeros(1,256, CV_32F);
   for(int i=0;i<hist.cols;++i)
   {
     histN.at<float>(i)=hist.at<float>(i)/(image.rows*image.cols);
   }
   return histN;
}

Mat makeHistcum(Mat hist)
{
  Mat Histcum = Mat::zeros(1,256, CV_32F);
  float intensity;
  Histcum.at<float>(0) = hist.at<float>(0);

  for ( int i = 1; i < hist.cols; ++i)
    {
        intensity = Histcum.at<float>(i-1) + hist.at<float>(i);
        Histcum.at<float>(i) = intensity;
    }
    return Histcum;
}




float getVarInter(int t, Mat hist, Mat histC, Mat histCN)
{
  float mu1=0;
  float mu=0;
  float mu2=0;
  float var=0;

  float p1=histCN.at<float>(t);
  float p2=1-p1;
  if(p2<0.0001)
    p2=0;
  float denom=0;
  //Calcul de mu1
  for ( int i = 0; i < t; ++i)
    {

        mu1=mu1+i*hist.at<float>(i);
        denom = denom + hist.at<float>(i);

    }
    if(denom<0.0001)
      denom=0;
  //Calcul de mu de l'image
  for(int i=0;i<histC.cols;i++)
  {
    mu=mu+i*hist.at<float>(i);
  }
  mu=mu/(image.cols*image.rows);
  mu1=float(mu1)/denom;
  mu2=(mu-p1*mu1)/p2;

  //Calcul de la variance interclasse
  var=p1*p2*(mu1-mu2)*(mu1-mu2);

  return var;
}

int getThreshold(Mat hist, Mat histC,Mat histCN)
{
  float max=-1;
  int intensite=0;
  float currentMax;
  for(int i=0;i<histC.cols;++i)
  {
    currentMax=getVarInter(i, hist, histC,histCN);
    if(currentMax>max)
      {
        max=currentMax;
        intensite=i;
      }
  }
  return(intensite);
}
Mat ThresholdPic(Mat img, float t)
{
     Mat TP = image.clone();
     for(int i=0;i<img.rows;++i)
     {
       for(int j=0;j<img.cols;++j)
       {
         if((float)img.at<uchar>(i,j)>t)
          {
            TP.at<uchar>(i,j)=255;
          }
          else
          {
            TP.at<uchar>(i,j)=0;
          }
       }
     }
     return TP;
}
void process(const char* imsname)
{
  if(imsname==NULL)
  {
  //lecture de l'image
  exit(EXIT_FAILURE);
  cout<<"Objet vide"<<endl;
  }
    else
  {
    image=imread(imsname,0);

    //Creation de l'histograme
    Mat valHist=makeHist(image);
    //Creation de l'histogramme normalise
    Mat valHistNorm=makeHistNorm(valHist);
    //Creation de l'histogramme cumule
    Mat valHistcum=makeHistcum(valHist);
    //Cration de l'histograme cumule normalise
    Mat valHistcumN=makeHistcum(valHistNorm);

    int thr= getThreshold(valHist, valHistcum,valHistcumN);
    cout << "Manual Threshold : "<<thr<<endl;

    //utilisation de la methode threshold()
    Mat otsu_th_ocv;
    threshold(image, otsu_th_ocv, 0, 255, THRESH_BINARY+THRESH_OTSU);
    double thresh_val = threshold(image, otsu_th_ocv, 0, 255, CV_THRESH_BINARY + CV_THRESH_OTSU);
    imwrite("otsu-th-ocv.png",otsu_th_ocv);
    imshow("otsu-th-ocv.png",otsu_th_ocv);

    cout << "OpenCV Threshold : "<<thresh_val<<endl;

    Mat th = ThresholdPic(image,thr);

    imwrite("otsu.png", th);
    imshow("otsu.png",th);

    Mat diff = th - otsu_th_ocv;
    imshow("diff",diff);

  //  waitKey(0);
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
