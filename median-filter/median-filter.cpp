#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
int r;


void tricroissant( int tab[], int tab_size)
{
  int i=0;
  int tmp=0;
  int j=0;

  for(i = 0; i < tab_size; i++)
    {
      for(j = 1; j < tab_size; j++)
        {
          if(tab[i] < tab[j])
            {
              tmp = tab[i];
              tab[i] = tab[j];
              tab[j] = tmp;
              j--;
            }
        }
    }
  tmp = tab[0];
  for(i = 0; i < tab_size; i++)
    tab[i] = tab[i+1];
  tab[tab_size-1] = tmp;
}

void process(const int r,const char* ims, const char* imd)
{
  if(ims==NULL)
  {
    cout<<"Objet vide"<<endl;
    exit(EXIT_FAILURE);
  }

  else
  {
    Mat image = imread(ims,CV_LOAD_IMAGE_GRAYSCALE);
    Mat matroi;
    Mat medianBlurPictureOCV;
    Mat medianBlurPicture=image.clone();
    int surrounding=2*r+1;
    float l,w,x,y;
    if(image.empty())
      cout<<"Image vide"<<endl;
    else
    {
      int fenetre[(2*r+1)*(2*r+1)];
       for ( int j = 0; j < image.rows; ++j)
       {
          for ( int i = 0; i < image.cols; ++i)
             {
               if(j <= surrounding/2-1 )
               {
                 w = j + surrounding/2+1;
                 y = 0;
               }
               else
               {
                 if(j > image.rows - surrounding)
                 {

                   if(image.rows-j>= surrounding/2+1)
                     w = image.rows - j;
                   else
                     w= surrounding/2+1;

                 }
                 else
                   w = surrounding;
                 y = j - surrounding/2;
               }
               if(i <= surrounding/2-1 )
               {
                 l = i + surrounding/2+1;
                 x = 0;
               }
               else
               {
                 if(i > image.cols -surrounding)
                 {
                   if(image.cols-i>=surrounding/2+1)
                     l = image.cols - i;
                   else
                     l=surrounding/2+1;
                 }
                 else
                   l = surrounding;
                 x = i - surrounding/2;
               }

               int q=0;
               matroi = image(Rect(x,y,l,w));

               for(int n=0;i<l;n++)
               {
                 for(int p=0;j<w;p++)
                 {
                   //fenetre[q]= matroi.at<uchar>(n,p);
                  cout<< fenetre[q]<<endl;
                  q++;
                 }
               }

               tricroissant(fenetre, l*w);

               medianBlurPicture.at<uchar>(j,i)=fenetre[2*r+1];
             }
           }
        }
      medianBlur(image,medianBlurPictureOCV,1);
      imwrite(imd, medianBlurPicture);
      namedWindow(imd,WINDOW_AUTOSIZE);
      imshow(imd,medianBlurPicture);
      namedWindow("OPEN CV : Median Blur",WINDOW_AUTOSIZE);
      imshow("OPEN CV : Median Blur",medianBlurPictureOCV);
      namedWindow(ims,WINDOW_AUTOSIZE);
      imshow(ims,image);
    }
    waitKey(0);
}
void usage (const char *s)
{
    cout<<"Usage: "<<s<<" usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}

#define param 3
int main( int argc, char* argv[])
{
  if(argc != (param+1))
    usage(argv[0]);
  process(atoi(argv[1]),argv[2],argv[3]);
  return EXIT_SUCCESS;
}
