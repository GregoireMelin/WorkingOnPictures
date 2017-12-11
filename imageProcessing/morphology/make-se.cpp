#include <iostream>
#include <cstdlib>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

enum Shape {SQUARE = 0, DIAMOND = 1, DISK = 2, LINE_V = 3, DIAG_R = 4, LINE_H = 5, DIAG_L = 6, CROSS = 7, PLUS = 8};

void process(const int shape ,const int halfsize, const char* name)
{
  //Initialisation des variables
  int size = 2 * halfsize + 1;
  Mat se;
  Shape selectedShape = static_cast<Shape>(shape);
  se = Mat::zeros(size, size, CV_8UC1);

  //Cas du carre
  if(selectedShape == SQUARE)
    se = Mat::ones(size, size, CV_8UC1);
  else if(selectedShape== LINE_V || selectedShape== LINE_H || selectedShape== PLUS )
  {
    if(selectedShape== LINE_V || selectedShape== PLUS)
    {
      for(int j =0; j< se.rows; j++)
        se.at<uchar>(j, halfsize) = 1;
    }
    if(selectedShape== LINE_H || selectedShape== PLUS)
    {
      for(int i=0; i< se.cols; i++)
          se.at<uchar>(halfsize, i) = 1;
    }
  }

  //Cas des lignes diagonales + cas de la croix
  else if(selectedShape== DIAG_R || selectedShape== DIAG_L || selectedShape== CROSS )
  {
    if(selectedShape== DIAG_R || selectedShape== CROSS)
    {
      int i = size -1;
      for(int j =0; j< se.rows; j++)
      {
        se.at<uchar>(j, i) = 1;
        i--;
      }

    }
    if(selectedShape== DIAG_L || selectedShape== CROSS)
    {
      int i = 0;
      for(int j =0; j< se.rows; j++)
      {
        se.at<uchar>(j, i) = 1;
        i++;
      }
    }
  }

  //Cas du losange
  else if (selectedShape== DIAMOND)
  {
    int start = halfsize;
    int nb = 1;
    bool bottomHalf = false;

    for(int j =0; j< se.rows; j++)
    {
      for (int i = 0; i < nb; i++)
      {
        se.at<uchar>(j, start + i) = 1;
      }

      if(start == 0)
      {
        bottomHalf = true;
      }

      if(!bottomHalf)
      {
        start-=1;
        nb+=2;
      }
      else
      {
        start +=1;
        nb -=2;
      }
    }
  }

  //Cas du disque
  else if (selectedShape== DISK)
  {
    for(int j =0; j< se.rows; j++)
    {
      for (int i = 0; i < se.cols; i++)
      {
        int x = i - halfsize;
        int y = j - halfsize;
        if(sqrt(pow(x,2) + pow(y,2)) <= halfsize )
          se.at<uchar>(j, i) = 1;
      }
    }
  }
  //Binarisation de l'image
  threshold(se, se, 0, 255, CV_THRESH_BINARY);
  //Enregistrement et affichage
  imwrite(name, se);
  imshow("Shape", se);
  //waitKey(0);
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
  process(atoi(argv[1]),atoi(argv[2]),argv[3]);
  return EXIT_SUCCESS;
}
