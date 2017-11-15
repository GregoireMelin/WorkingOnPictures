#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;


Mat image;
Mat imageBil;
Mat imageBil_ocv;

double computeG(int diff, double sigma)
{
   double G;
   G = exp(-pow(diff ,2)/(2 * pow(sigma,2)));
   if(G < 0.00001)
   {
     G = 0;
   }
   return G;
}


void process(double sigmaS, double sigmaG ,const char* ims, const char* imd)
{
  if(ims==NULL)
  {
  //lecture de l'image
  exit(EXIT_FAILURE);
  cout<<"Objet vide"<<endl;
  }
    else
  {
    //lecture de l'image
    image = imread(ims, CV_LOAD_IMAGE_GRAYSCALE);

    int surrounding = 15;
    float pixelVal;
    float pixelValNeighb;
    int pixelFinalVal;


    //parametres qui vont definir le roi du voisinage
    float x;
    float y;
    float l;
    float w;

    double diffC;
    double diffS;
    double Gs;
    double Gc;
    double num;
    double den;

    imageBil = image.clone();
    Mat matRoi;
    //Seuillage bilateral de image dont le resultat est stocke dans imageBil
    for ( int j = 0; j < image.rows; ++j)
      {
        for ( int i = 0; i < image.cols; ++i)
          {

           pixelVal = image.at<uchar>(j, i);

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

          //  cout<<i<<" "<<j<<" x: "<< x <<" y: "<<y<<" w: " << w << " l: "<<l<<endl;

           //découpage du voisinage du pixel selectionne selon une matrice carre surrounding*surrounding
           matRoi = image(Rect(x,y,l,w));

           num = 0;
           den = 0;
           Gs = 0;
           Gc = 0;
           diffS = 0;
           diffC = 0;

           for(int k = 0; k < w; k++)
           {
             for(int q = 0; q < l; q++)
             {
              pixelValNeighb = matRoi.at<uchar>(k, q);

               diffS = abs( i - (x + q)) + abs(j -(y + k));
               diffC = abs(pixelVal - pixelValNeighb);

               Gs = computeG(diffS, sigmaS);
               Gc = computeG(diffC, sigmaG);

               num += Gs * Gc * pixelValNeighb;
               den += Gs * Gc;
             }
           }

           pixelFinalVal = num/den;

          imageBil.at<uchar>(j,i) = pixelFinalVal;

         }
    }






    //affichage de l'image original
    imshow(ims,image);

    //ecriture et affichage de l'image modifiee
    imwrite(imd, imageBil);
    imshow(imd, imageBil);


    //affichage de l'image obtenu via la methode employee par opencv
    bilateralFilter(image, imageBil_ocv, 15, sigmaG, sigmaS); // application de la methode bilateralFilter de opencv
    imshow("bilateral_opencv",imageBil_ocv);

    //visualisation de la difference entre la methode implementee et la methode opencv
    string newPicName = imd;
    imshow("Difference: bilateral_opencv  - " + newPicName, imageBil_ocv - imageBil);

  }

    waitKey(0);

}

void usage (const char *s)
{
    cout<<"Usage: "<<s<<"usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}

#define param 4
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(atof(argv[1]),atof(argv[2]),argv[3],argv[4]);

  return EXIT_SUCCESS;
}
