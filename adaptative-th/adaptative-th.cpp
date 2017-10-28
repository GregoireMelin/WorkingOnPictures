#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;


void process(const char* ims,const int radius, const int constant)
{
  if(ims==NULL)
  {
    cout<<"Objet vide"<<endl;
    exit(EXIT_FAILURE);
  }
  else
  {
    //lecture de l'image
    Mat image = imread(ims,0);
    int surrounding = 2*radius+1; 

    Mat th = image.clone();
    Mat matroi;
    float pixelval;

    float mean;
    float l;
    float w;
    float x;
    float y;

    //Seuillage adapatatif local "manuel" selon une matrice carré de taille surrounding * surrounding
    for ( int j = 0; j < image.rows; ++j)
    {
      for ( int i = 0; i < image.cols; ++i)
          {

            pixelval = image.at<uchar>(j, i);

            //determination du voisinage en fonction des coordonnees du pixel
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
              {
                w = surrounding;
              }
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
              {
                l = surrounding;
              }
              x = i - surrounding/2;
            }

            //découpage du voisinage du pixel selectionne selon une matrice carre surrounding*surrounding
            matroi = image(Rect(x,y,l,w));
            mean =  sum(matroi).val[0]/(surrounding * surrounding );

            //Seuillage
            if(pixelval <= (mean - constant))
            {
              th.at<uchar>(j,i) =  0;
            }
            else
            {
              th.at<uchar>(j,i) =  255;
            }

          }
    }

    imwrite("th.png", th);
    imshow("th",th);

    //Seuillage via la fonction AdaptativeThreshold
    Mat th_mean;

    adaptiveThreshold(image,th_mean, 255, 0, 0, surrounding, constant);

    imwrite("th-ocv-mean.png", th_mean);
    imshow("original",image);

    imshow("th-ocv-mean", th_mean);

    //Calcul de la difference entre les deux pecedentes images seuillees
    Mat diff =  th - th_mean;
    imwrite("diff.png", diff);
    imshow("diff.png",diff);

    //Seuillage via l'option ADAPTATIVE_THRESH_GAUSSIAN_C
    Mat th_gauss;
    adaptiveThreshold(image,th_gauss, 255, 1, 0, surrounding, constant);

    imwrite("th-ocv-gauss.png", th_gauss);
    imshow("gauss", th_gauss);
    waitKey(0);
    }
}

void usage (const char *s)
{
    cout<<"Usage: "<<s<<"usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}
#define param 3
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1],atoi(argv[2]),atoi(argv[3]));

  return EXIT_SUCCESS;
}
