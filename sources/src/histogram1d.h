#ifndef HISTOGRAM1D_H
#define HISTOGRAM1D_H

#include <opencv2/opencv.hpp>

using namespace cv;

class Histogram1D
{
private:
    int histSize[2];
    float hranges[2];
    const float* ranges[1];
    int channels[1];
    int mNbPixel;

    MatND mHist;
    MatND mHistNorm;
    MatND mHistCumul;
    MatND mHistCumulDeriv;

    std::vector<unsigned char> mVecSeuil;

    const static int DERIV_FACTOR = 17;

public:
    Histogram1D();
    Histogram1D(const Mat &image);

    MatND getHistogram();
    MatND getHistogramCumul();
    MatND getHistogramDerivCumul();
    Mat getHistogramImage(MatND hist);

    std::vector<unsigned char> getSeuilByDerivCumul();
    void cumulHist();
    void derivCumul();
    void computeHistogram(const Mat &image);
};

#endif
