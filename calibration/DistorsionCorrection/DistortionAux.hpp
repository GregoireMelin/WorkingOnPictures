/*
 * DistortionAux.hpp
 *
 *  Created on: Oct 27, 2012
 *      Author: ubuntu
 */

#ifndef DISTORTIONAUX_HPP_
#define DISTORTIONAUX_HPP_

#include <stdio.h>
#include <iostream>
#include <sstream>
#include "opencv2/core/core.hpp"
//#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

// Record the execution time of some code, in milliseconds.
#define DECLARE_TIMING(s)  int64 timeStart_##s; double timeDiff_##s; double timeTally_##s = 0; int countTally_##s = 0
#define START_TIMING(s)    timeStart_##s = cvGetTickCount()
#define STOP_TIMING(s) 	   timeDiff_##s = (double)(cvGetTickCount() - timeStart_##s); timeTally_##s += timeDiff_##s; countTally_##s++
#define GET_TIMING(s) 	   (double)(timeDiff_##s / (cvGetTickFrequency()*1000.0))
#define GET_AVERAGE_TIMING(s)   (double)(countTally_##s ? timeTally_##s/ ((double)countTally_##s * cvGetTickFrequency()*1000.0) : 0)
#define CLEAR_AVERAGE_TIMING(s) timeTally_##s = 0; countTally_##s = 0

// Distortion
// void undistort(const Mat& src, Mat& dst, const Mat& cameraMatrix, const Mat& distCoeffs, const Mat& newCameraMatrix=Mat()); // Defined in OpenCV module calib3d
void distort(const cv::Mat& src, cv::Mat& dst, const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs, const cv::Mat& cameraMatrixDst=Mat());
void undistortPointsK(const vector<Point2f> & srcPts, vector<Point2f> & dstPts, const cv::Mat& cameraMatrixSrc, const cv::Mat& distCoeffs, const cv::Mat& cameraMatrixDst=Mat());
void distortPointsK(const vector<Point2f> & srcPts, vector<Point2f> & dstPts, const cv::Mat& cameraMatrixSrc, const cv::Mat& distCoeffs, const cv::Mat& cameraMatrixDst=Mat());

// Drawing
void sampleLine(vector<Point2f>& pts, int N, Point2f &pt1, Point2f &pt2);
void sampleRect(vector<Point2f>& pts, int N, Point2f &pt1, Point2f &pt2);
void drawPolyline(Mat im, const vector<Point2f> points, bool isClosed, const Scalar & color, int thickness = 1, int lineType =8, int shift =0);


#endif /* DISTORTIONAUX_HPP_ */
