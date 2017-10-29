#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

float intensity;
float thresholdOtsu;

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

Mat makeHistNorm(Mat image)
{
   Mat hist;
   hist=makeHist(image);
   hist=hist/(image.rows*image.cols);
   return hist;
}

void process(const char* imsname)
{
  if(imsname==NULL)
  {
    cout<<"Objet vide"<<endl;
    exit(EXIT_FAILURE);
  }
  else
  {
    //lu en niveau de gris
    Mat image = imread(imsname,0);
    Mat hist;
    //cout<<image.channels()<<endl; //donne 1

    Mat histFunction;
    histFunction=makeHistNorm(image);
    float sum=0;
    for( int h = 0; h < 256; h++ )
     {

         float binVal = histFunction.at<float>(h);
         sum=sum+binVal;
         cout<<" "<<binVal<<endl;;
     }
     cout << sum <<endl;
         /*for ( int j = 0; j < image.rows; ++j)
       {
		      for ( int i = 0; i < image.cols; ++i)
              {
                	intensity=image.at<float>(j, i);
                  if(intensity < thresholdOtsu)
                  {
                    copy.at<float>(j,i) = 0;
                  }
                  else
                  {
                    copy.at<float>(j,i) = 1;
                  }
              }
        }

        //imwrite("otsu-th.png",otsu_th);


        //utilisation de la methode threshold()
        Mat otsu_th_ocv;
        threshold(image, otsu_th_ocv, 0, 255, THRESH_BINARY+THRESH_OTSU);
        imwrite("otsu-th-ocv.png",otsu_th_ocv);


        //calcul difference otsu-th.png et de otsu-th-ocv.png
       Mat difference;
        subtract(otsu_th,otsu_th_ocv,difference);

        namedWindow(imsname,WINDOW_AUTOSIZE);
        imshow(imsname,image);

        namedWindow("otsu-th.png",WINDOW_AUTOSIZE);
        imshow("otsu-th.png",otsu_th);

        namedWindow("otsu-th-ocv.png",WINDOW_AUTOSIZE);
        imshow("otsu-th-ocv.png",otsu_th_ocv);

        namedWindow("((otsu-th.png) - (otsu-th-ocv.png))",WINDOW_AUTOSIZE);
        imshow("((otsu-th.png) - (otsu-th-ocv.png))",otsu_th_ocv);*/

        // Initialisation parametre
        /*int histSize = 256;    // bin size
        float range[] = { 0, 255 };
        const float *ranges[] = { range };

        // Calcule l'histogramme
        calcHist( &image, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false );

        // Affichage console
         for( int h = 0; h < histSize; h++ )
          {
              float binVal = hist.at<float>(h);
              cout<<" "<<binVal;
          }

          //calculer taille en sommant nombre de pixel=

          cout<<"End first histo"<<endl;
          cout<<"taille is: "<<image.rows*image.cols<<endl;

          int sum;
           Mat valHist = Mat::zeros(1,256, CV_32F);
           int intensity;
           for ( int j = 0; j < image.rows; ++j)
             {
               for ( int i = 0; i < image.cols; ++i)
               {
                 intensity = (float)image.at<uchar>(j, i);
                 valHist.at<float>(intensity) = valHist.at<float>(intensity)+1;
                 sum++;
               }
             }

             cout<<"Hist emperic"<<endl;
             cout<<"somme is: "<< sum<< endl;

           for( int h = 0; h < 256; h++ )
            {
                float binVal = valHist.at<float>(h);
                cout<<" "<<binVal;
            }
            cout<<"last "<< valHist.at<float>(255)<<endl;*/
           waitKey(0);
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
