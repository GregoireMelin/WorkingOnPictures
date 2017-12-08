#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>

#include <opencv2/nonfree/features2d.hpp>



#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

void process(const char* ims1, const char* ims2)
{
  if(ims1==NULL || ims2==NULL)
  {
    cout<<"Objet vide"<<endl;
    exit(EXIT_FAILURE);
  }
  else
  {
    //lecture de l'image

    Mat image = imread(ims1,1);
    Mat image2 = imread(ims2,1);

    Mat imageGray = imread(ims1,0);
    Mat image2Gray = imread(ims2,0);


    SurfFeatureDetector detector( 400 );


    //Création de vecteurs qui vont stocker les points d'interets pour chaque image
    vector<KeyPoint> keypointsImg1, keypointsImg2;

    //Stockage des points d'intérêts détectés sur les images
    detector.detect( image, keypointsImg1 );
    detector.detect( image2, keypointsImg2 );

    //Création des images où sont dessinées les points d'interets
    Mat surf1;
    Mat surf2;

    drawKeypoints( imageGray, keypointsImg1, surf1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    drawKeypoints( image2Gray, keypointsImg2, surf2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

    //Affichage des images générées
    imshow("Surf image 1", surf1 );
    imshow("Surf image 2", surf2 );

    //Extraction des descriptions des points d'intérêts de l'image au sein des matrices descriptor1 et descriptor2
    SurfDescriptorExtractor extractor;

    Mat descriptor1, descriptor2;

    extractor.compute( imageGray, keypointsImg1, descriptor1 );
    extractor.compute( image2Gray, keypointsImg2, descriptor2 );

    //------------------------------------------------------------------------------------------
    //-------------------Appariements Brute Force-------------------------------------------------
    //Determine les vecteurs correspondant aux points d'interets entre les deux images
    BFMatcher matcher(NORM_L2);
    std::vector< DMatch > bruteForceVectors;
    matcher.match( descriptor1, descriptor2, bruteForceVectors );

    //Dessine l'image montrant un appariement brute force entre les points d'interets de l'image 1 et 2
    Mat bruteForcePic;
    drawMatches( imageGray, keypointsImg1, image2Gray, keypointsImg2, bruteForceVectors, bruteForcePic );

    //Affiche l'image générée
    imshow("Appariements Brute Force", bruteForcePic );
    //------------------------------------------------------------------------------------------
    //-------------------Appariements Seuillees-------------------------------------------------

    //Determination des distances minimales et maximales entre les points d'interets
    double maxDistance = 0;
    double minDistance = 10000;

    for( int i = 0; i < descriptor1.rows; i++ )
    {
      double distance = bruteForceVectors[i].distance;
      if( distance < minDistance )
        minDistance = distance;
      if( distance > maxDistance )
        maxDistance = distance;
    }

  //Determination des vecteurs parmis les vecteurs brutes forces dont la distance est inférieure à 3 fois minDistance
  std::vector< DMatch > thresholdVectors;

  for( int i = 0; i < descriptor1.rows; i++ )
  {
    if( bruteForceVectors[i].distance <= max(3*minDistance, 0.02) )
      {
        thresholdVectors.push_back( bruteForceVectors[i]);
      }
  }

  //Creation et affichage de l'image montrant l'appariement seuillee
  Mat thresholdImg;
  drawMatches( imageGray, keypointsImg1, image2Gray, keypointsImg2,
               thresholdVectors, thresholdImg, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::DEFAULT );

  imshow( "Appariements Seuillées", thresholdImg );

  //------------------------------------------------------------------------------------------
  //-------------------Homographie -----------------------------------------------------------


  std::vector<Point2f> vectorsImage1;
  std::vector<Point2f> vectorsImage2;
  int size = thresholdVectors.size();

  for( int i = 0; i < size; i++ )
  {
    //-- Get the keypoints from the good matches
    vectorsImage2.push_back( keypointsImg1[ thresholdVectors[i].queryIdx ].pt );
    vectorsImage1.push_back( keypointsImg2[ thresholdVectors[i].trainIdx ].pt );
  }

    Mat H = findHomography( vectorsImage1, vectorsImage2, CV_RANSAC);

    Size size2 = Size(imageGray.cols + image2Gray.cols, imageGray.rows + image2Gray.rows);

    Mat image2Homo;
    warpPerspective(image2, image2Homo, H, size2,  INTER_LINEAR, BORDER_CONSTANT);
    imshow("image 2 déformée par homographie", image2Homo);

    Mat panorama = image2Homo.clone();

    for (int i = 0; i < image.rows; ++i)
    {
      for (int j = 0; j <image.cols; ++j)
      {
         panorama.at<Vec3b>(i, j) = image.at<Vec3b>(i, j);

      }
    }

    imshow("panorama", panorama);


    Vec3b colour;
    int cropLength;
    for( int j = 0; j < panorama.cols; j++)
    {
      colour = panorama.at<Vec3b>(0,j);
      if(colour == panorama.at<Vec3b>(0,panorama.cols-1) )
      {
        cropLength = j-1;
        cout<<cropLength<<endl;
        break;
      }
    }


    Mat panoramaFinal(image.rows, cropLength,CV_8UC3);

     for( int i = 0; i < panoramaFinal.rows; i++ )
     {
       for( int j = 0; j < panoramaFinal.cols; j++)
       {
         panoramaFinal.at<Vec3b>(i,j) = panorama.at<Vec3b>(i,j);
       }
     }

    imshow("panorama final", panoramaFinal);

    imshow(ims1, image);
    imshow(ims2, image2);
    waitKey(0);
    }
}

void usage (const char *s)
{
    cout<<"Usage: "<<s<<"usage : ERROR"<<endl;
    exit(EXIT_FAILURE);
}
#define param 2
int main( int argc, char* argv[] )
{
  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1], argv[2]);

  return EXIT_SUCCESS;
}
