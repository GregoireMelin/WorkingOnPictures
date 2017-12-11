
#include "DistortionAux.hpp"


/* DISTORTION auxiliary functions */

// Reverse operation that undistort:
// Take src "rectified image", and distort it to obtain dst "distort image"
void distort(const cv::Mat& src, cv::Mat& dst, const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs, const cv::Mat& cameraMatrixDst)
{
	Mat Kdst;

	if (cameraMatrixDst.empty()) {
		Kdst = cameraMatrix; // If same cameraMatrix for src and dst
	} else {
		Kdst = cameraMatrixDst; // dst has a different cameraMatrix
	}

	//cv::Mat distort_x = cv::Mat(src.size(), CV_32F);
	//cv::Mat distort_y = cv::Mat(src.size(), CV_32F);

	cv::Mat pixel_locations_src = cv::Mat(src.size(), CV_32FC2);


	for (int i = 0; i < src.size().height; i++) {

		for (int j = 0; j < src.size().width; j++) {

			pixel_locations_src.at<cv::Point2f>(i,j) = cv::Point2f(j,i);

		}

	}

	cv::Mat fractional_locations_dst = cv::Mat(src.size(), CV_32FC2);

	pixel_locations_src=pixel_locations_src.reshape(2, 1);
	fractional_locations_dst=fractional_locations_dst.reshape(2, 1);

	cv::undistortPoints(pixel_locations_src, fractional_locations_dst, cameraMatrix, distCoeffs); // OpenCV.imgproc function

	cv::Mat pixel_locations_dst = cv::Mat(src.size(), CV_32FC2);

	const float fx = Kdst.at<double>(0,0);
	const float fy = Kdst.at<double>(1,1);
	const float cx = Kdst.at<double>(0,2);
	const float cy = Kdst.at<double>(1,2);

	//pixel_locations_src.reshape(2, 1);
	fractional_locations_dst=fractional_locations_dst.reshape(2, src.size().height);

	// is there a faster way to do this?
	for (int i = 0; i < fractional_locations_dst.size().height; i++) {

		for (int j = 0; j < fractional_locations_dst.size().width; j++) {

			const float x = fractional_locations_dst.at<cv::Point2f>(i,j).x*fx + cx;
			const float y = fractional_locations_dst.at<cv::Point2f>(i,j).y*fy + cy;
			pixel_locations_dst.at<cv::Point2f>(i,j) = cv::Point2f(x,y);

		}

	}

	cv::remap(src, dst, pixel_locations_dst, cv::Mat(), CV_INTER_LINEAR);

}

void distortPointsK(const vector<Point2f> &srcPts, vector<Point2f> &dstPts, const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs, const cv::Mat& cameraMatrixDst) {
	Mat Kdst;

	if (cameraMatrixDst.empty()) {
		Kdst = cameraMatrix; // If same cameraMatrix for src and dst
	} else {
		Kdst = cameraMatrixDst; // dst has a different cameraMatrix
	}

	Mat zvec = Mat::zeros(3,1,CV_32FC1);
	vector<Point3f> src3(srcPts.size());

	float fx = cameraMatrix.at<double>(0,0);
	float fy = cameraMatrix.at<double>(1,1);
	float x0 = cameraMatrix.at<double>(0,2);
	float y0 = cameraMatrix.at<double>(1,2);

	for (uint32_t i=0; i<srcPts.size(); i++) {
		src3[i].x=(srcPts[i].x - x0)/fx;
		src3[i].y=(srcPts[i].y - y0)/fy;
		src3[i].z=1.0;
	}

	projectPoints(src3, zvec, zvec, Kdst, distCoeffs, dstPts ); // OpenCV.calib3d function
}

void undistortPointsK(const vector<Point2f> &srcPts, vector<Point2f> &dstPts, const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs, const cv::Mat& cameraMatrixDst) {
	Mat Kdst;

	if (cameraMatrixDst.empty()) {
		Kdst = cameraMatrix; // If same cameraMatrix for src and dst
	} else {
		Kdst = cameraMatrixDst; // dst has a different cameraMatrix
	}

	vector<Point2f> tmpPts;

	undistortPoints(srcPts, tmpPts, cameraMatrix, distCoeffs); // OpenCV.imgproc function

	float fx = Kdst.at<double>(0,0);
	float fy = Kdst.at<double>(1,1);
	float x0 = Kdst.at<double>(0,2);
	float y0 = Kdst.at<double>(1,2);

	dstPts.resize(srcPts.size());
	for (uint32_t i=0; i<srcPts.size(); i++) {
		dstPts[i].x = tmpPts[i].x * fx + x0;
		dstPts[i].y = tmpPts[i].y * fy + y0;
	}


}

/* DRAWING Auxiliary functions */

// Sample N uniformly spaced points between pt1 and pt2, and put them in pts
void sampleLine(vector<Point2f>& pts, int N, Point2f &pt1, Point2f &pt2) {
	pts = vector<Point2f>(N);
	for (int i=0; i<N; i++) {
		pts[i].x=(double)i / (N-1) * (pt2.x - pt1.x) + pt1.x;
		pts[i].y=(double)i / (N-1) * (pt2.y - pt1.y) + pt1.y;
	}
}

// Sample N uniformly spaced points on each edge of rectangle with corners pt1 and pt2, and put them in pts
void sampleRect(vector<Point2f>& pts, int N, Point2f &pt1, Point2f &pt2) {
	pts = vector<Point2f>(4*N);

	double left = pt1.x, right=pt2.x, top=pt1.y, bottom=pt2.y;

	for (int i=0; i<N; i++) {
		pts[i].x=(double)i / (N-1) * (right-left) + left;
		pts[i].y=(double)top;
		pts[i+2*N].x=(double)i / (N-1) * (left-right) + right;
		pts[i+2*N].y=(double)bottom;
	}
	for (int i=0; i<N; i++) {
		pts[i+N].x=(double)right;
		pts[i+N].y=(double)i / (N-1) * (bottom-top) + top;
		pts[i+3*N].x=(double)left;
		pts[i+3*N].y=(double)i / (N-1) * (top-bottom) + bottom;
	}
}

// Draw a vector<Point2f> polyline (wrapper for polylines)
void drawPolyline(Mat im, const vector<Point2f> points, bool isClosed, const Scalar & color, int thickness /*= 1/*/, int lineType /*= 8*/, int shift /*= 0*/){
	Point ** points_to_draw = new (Point*);
	*points_to_draw = new Point[points.size()];

	for (uint32_t i=0; i<points.size(); i++) {
		points_to_draw[0][i] = points[i];
	}

	int * npts = new int[1];
	npts[0] = points.size();

	// draw
	polylines(im, const_cast<const Point**>(points_to_draw), npts, 1, isClosed, color, thickness, lineType, shift);

	// clean up the memory mess :)
	delete points_to_draw[0];
	delete points_to_draw;
	delete [] npts;
}
