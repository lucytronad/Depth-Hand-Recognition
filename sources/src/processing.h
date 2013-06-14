#ifndef PROCESSING_H
#define PROCESSING_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;

class Processing
{
private:

    static void searchConnexityPath(Mat img,Mat& dst, int i, int j, unsigned char Sb, unsigned char Sh, int pathCode);
    static bool searchConnexity(Mat img, int i, int j, unsigned char Sh, int connexityCode);

public:

    const static int CONNEXITY_4 = 0;
    const static int CONNEXITY_8 = 1;

    enum ELEM_STATE { N,NE,E,SE,S,SO,O,NO};

    int nbPixel;

    Processing();
    static Mat threshold(Mat src, unsigned char seuil);
    static Mat multyThreshold(Mat src, std::vector<unsigned char> seuils);
    static Mat getErosion(Mat img,InputArray kernel, int iteration = 1);
    static Mat getDilatation(Mat img,InputArray kernel, int iteration = 1);
    static Mat getKernel(int shape,Size size);
    static Mat getInverse(Mat img);
    static Mat getDistanceTransform(Mat img);
    static Mat getMaxLocHyst(Mat img,unsigned char Sh, unsigned char Sb ,int connexity );
    static Mat getCannyDetector(Mat img,double Sh, double Sb);
    static Mat getThinning(Mat1b img, int code, int iteration);
    static ELEM_STATE getNextState(ELEM_STATE cur);
    static Point2i getDistanceMax(Mat img);
    static std::vector<Point2i> getSkelExtremity(Mat img);
    static std::vector<Point2i> getMultiPoints(Mat img);
    static Mat getSkeletonByDistanceMap(Mat dst);
    static Mat getExtractMat(Mat img, std::vector<Point2i> coords);
    static std::vector<Point2i> getExtractCoord(Mat img);
    static std::vector<double> getDensity(Mat rect, int w, int h);
    static std::vector<Point2i> getConvexHull(std::vector<Point2i> contours);
    static std::vector<int> getConvexHullI(std::vector<Point2i> contours);
    static std::vector<Point2i> convexityDefects(std::vector<Point> contour, std::vector<int> hull);
    static std::vector<Point2i> convexityDefectsPoints(std::vector<Point> contour, std::vector<int> hull);
};

#endif // PROCESSING_H
