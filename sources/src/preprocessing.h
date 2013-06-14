#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <opencv2/opencv.hpp>

using namespace cv;

class PreProcessing
{

public:
    PreProcessing();

    static void getMedianFilter3( Mat& img );
    static void getMedianFilter5( Mat& img );
    static void getExpansion(Mat& img);
    static Mat  getUCHARImage(Mat& img,float value);
    static Mat1b getMedianBlur(Mat img, int blurSize);


};

#endif // PREPROCESSING_H
