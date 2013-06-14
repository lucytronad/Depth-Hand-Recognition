#include "histogram1d.h"

Histogram1D::Histogram1D() {
    histSize[0] = 256;
    histSize[1] = 256;
    hranges[0] = 0;
    hranges[1] = 255;
    ranges[0] = hranges;
    channels[0] = 0; // par défaut on regarde le canal 0

}

Histogram1D::Histogram1D(const Mat &image) {
    // initialisation des variables de l'histogramme
    histSize[0] = 256;
    histSize[1]= 256;
    hranges[0] = 0;
    hranges[1] = 255;
    ranges[0] = hranges;
    channels[0] = 0; // par défaut on regarde le canal 0

    computeHistogram(image);

}

//calcule l'histogramme
void Histogram1D::computeHistogram(const Mat &image) {

    mNbPixel = image.rows*image.cols;
    // calcul de l'histogramme
    calcHist(&image,
                1, // histogramme d'une seule image
                channels, // canal utilisé
                Mat(), // aucun masque utilisé
                mHist, // histogramme résultant
                1, // histogramme 1D
                histSize, // number of bins
                ranges // intervalle de l'histogramme (de 0 à 255)
             );
}

//calcule l'histogramme cumulé
void Histogram1D::cumulHist()
{
    mHistCumul = Mat(mHist.rows,mHist.cols,CV_32FC1,Scalar(0.0));
    for(int i = 0 ; i < mHist.rows ; i++)
    {
        mHistCumul.at<float>(i)= mHistCumul.at<float>(i-1) + mHist.at<float>(i);
    }
}

//calcule la dérivée de l'histogramme cumulé
void Histogram1D::derivCumul()
{
    if(!mHistCumul.empty())
    {
        mHistCumulDeriv= Mat(mHist.rows,mHist.cols,CV_32FC1,Scalar(0.0));

        for (int i = 0 ; i < mHistCumulDeriv.rows ; i++)
        {
            float tmp = 0;
            for(int k = i-DERIV_FACTOR ; k < i+DERIV_FACTOR ; k++ )
                tmp += (mHistCumul.at<float>(k)/(2*DERIV_FACTOR+1));
            mHistCumulDeriv.at<float>(i) = mHistCumul.at<float>(i)-tmp;
        }
    }
}

//calcule les passages de positif à négatif de la dérivée de l'histogramme cumulé
std::vector<unsigned char> Histogram1D::getSeuilByDerivCumul()
{
    if(!mHistCumulDeriv.empty())
    {
        for(int i = 1 ; i < mHistCumulDeriv.rows-1; i++)
            if( mHistCumulDeriv.at<float>(i-1) > 0 && mHistCumulDeriv.at<float>(i) < 0 )
            {
                mVecSeuil.push_back(i);
            }
    }
    return mVecSeuil;
}

//retourne une image de l'histogramme passé en paramètre
Mat Histogram1D::getHistogramImage(MatND hist) {

    double maxVal = 0;
    double minVal = 0;

    minMaxLoc(hist, &minVal, &maxVal, 0, 0);

    // cv::Mat qui contiendra l'image de l'histogramme
    Mat histImg(histSize[0], histSize[1], CV_8U, Scalar(255));

    int hpt = static_cast<int>(0.9*histSize[1]);

    // pour chaque niveau de gris on dessine une ligne verticale
    for (int h = 0; h < histSize[0]; h++ ) {
        float binVal = hist.at<float>(h);
        int intensity = static_cast<int>(binVal*hpt/maxVal);

        // dessine une ligne représentant le nombre de pixels du niveau de gris h
        line(histImg, Point(h, histSize[1]),
             Point(h, histSize[1] - intensity),
             Scalar::all(0));
    }
    return histImg;
}

//retourne l'histogramme
MatND Histogram1D::getHistogram()
{
    if(!mHist.empty())
    {
        return mHist; // retourne la Mat 1D de taille 256 contenant le nombre de pixel pour chaque niveau de gris
    }
}

//retourne l'histogramme cumulé
MatND Histogram1D::getHistogramCumul()
{
    if(!mHistCumul.empty())
    {
        return mHistCumul;
    }
}

//retourne la dérivée de l'histogramme cumulé
MatND Histogram1D::getHistogramDerivCumul()
{
    if(!mHistCumulDeriv.empty())
    {
        return mHistCumulDeriv;
    }
}
