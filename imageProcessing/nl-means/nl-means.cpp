#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;


Mat image;
Mat imageNL;
Mat getMatRoi(Mat image, int j, int i, int surrounding) //retourne une matrice carré centré sur le pixel
{
  float l;
  float w;
  float x;
  float y;

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

  if(i <= surrounding/2-1)
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

  Mat matRoi = image(Rect(x,y,l,w)); //Obtention de la matrice carré surrounding * surrounding centrée sur le pixel i,j
  return matRoi;
}





double computeD(Mat patchP, Mat patchQ)
{
   double D;
   float sum = 0;
   double surroundingP = patchP.rows * patchP.cols;
   double surroundingQ = patchQ.rows * patchQ.cols;

   if(surroundingP == surroundingQ)
   {
     for ( int j = 0; j < patchP.rows; ++j)
       {
         for ( int i = 0; i < patchP.cols; ++i)
           {
             sum += pow((patchP.at<uchar>(j, i) - patchQ.at<uchar>(j,i)),2);
           }
         }
   }

   else if(surroundingP > surroundingQ)
   {
     for ( int j = 0; j < patchP.rows; ++j)
       {
         for ( int i = 0; i < patchP.cols; ++i)
           {
             if(j < patchQ.rows && i < patchQ.cols)
             {
               sum += pow(patchP.at<uchar>(j, i) - patchQ.at<uchar>(j,i),2);
             }
             else
             {
               sum += pow(patchP.at<uchar>(j, i),2);
             }
           }
         }
   }
   else
   {
     for ( int j = 0; j < patchQ.rows; ++j)
       {
         for ( int i = 0; i < patchQ.cols; ++i)
           {
             if(j < patchP.rows && i < patchP.cols)
             {
               sum += pow(patchP.at<uchar>(j, i) - patchQ.at<uchar>(j,i),2);
             }
             else
             {
               sum += pow(patchQ.at<uchar>(j, i),2);
             }
           }
         }
   }

  D = (1/(surroundingP * surroundingQ)) * sum;
   return D;
}


double computeW(Mat patchP, Mat patchQ, double sigma)
{
  double W;
  float distance = computeD(patchP,patchQ);
  W = exp((-distance)/(2 * pow(sigma,2)));
  //cout<<W<<endl;
  return W;
}

void process(double sigma, const char* ims, const char* imd)
{
  if(ims==NULL)
  {
  //lecture de l'image
  exit(EXIT_FAILURE);
  cout<<"Objet vide"<<endl;
  }
    else
  {
    //lecture de l'image en niveau de gris
    image = imread(ims, CV_LOAD_IMAGE_GRAYSCALE);

    int pixelFinalVal;
    float pixelValNeighb;

    double num;
    double den;

    imageNL = image.clone();

    Mat matRoi;
    Mat patchP;
    Mat patchQ;

    //Filtrage nl-means de image dont le resultat est stocke dans imageNL
    for ( int j = 0; j < image.rows; ++j)
      {
        for ( int i = 0; i < image.cols; ++i)
          {

           matRoi = getMatRoi(image, j, i, 11);
           Mat matRoiXL = getMatRoi(image, j, i, 15);
           patchP = getMatRoi(image,j,i,7);
           num = 0;
           den = 0;

           for ( int l = 0; l < matRoi.rows; ++l)
             {
               for ( int k = 0; k < matRoi.cols; ++k)
                 {
                    pixelValNeighb = matRoi.at<uchar>(l, k);
                    patchQ = getMatRoi(matRoi, l,k,7);

                    num += pixelValNeighb  * computeW(patchP,patchQ,sigma);
                    den += computeW(patchP,patchQ,sigma);
                 }
              }


              pixelFinalVal = num/den;


              imageNL.at<uchar>(j,i) = pixelFinalVal;
         }
    }

    //affichage de l'image original
    imshow(ims,image);

    //ecriture et affichage de l'image modifiee
    imwrite(imd, imageNL);
    imshow(imd, imageNL);

  }

    waitKey(0);
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
  process(atof(argv[1]),argv[2],argv[3]);

  return EXIT_SUCCESS;
}
