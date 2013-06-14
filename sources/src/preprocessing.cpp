#include "preprocessing.h"

PreProcessing::PreProcessing()
{}

//retourne une cv::Mat d'unsigned char après ajustement des valeurs
Mat PreProcessing::getUCHARImage(Mat& img,float value)
{
    Mat dst(img.rows,img.cols,CV_8UC1);

    for(int i=0;i<img.rows;i++)
    {
        for(int j=0;j<img.cols;j++)
        {
            dst.at<unsigned char>(i,j)=(unsigned char)(img.at<float>(i,j)*255/value);
        }
    }
    return dst;
}

//applique un filtre médian de taille 3
void PreProcessing::getMedianFilter3(Mat &img)
{
    for(int i = 1 ; i < img.rows-1 ; i++)
        for(int j = 1 ; j < img.cols-1 ; j++)
        {
            int tmp = 0;
            tmp += img.at<unsigned char>(i-1,j-1);
            tmp += img.at<unsigned char>(i-1,j-1);
            tmp += img.at<unsigned char>(i-1,j-1);

            tmp += img.at<unsigned char>(i,j-1);
            tmp += img.at<unsigned char>(i,j);
            tmp += img.at<unsigned char>(i,j+1);

            tmp += img.at<unsigned char>(i+1,j-1);
            tmp += img.at<unsigned char>(i+1,j);
            tmp += img.at<unsigned char>(i+1,j+1);

            img.at<unsigned char>(i,j) = tmp/9;
        }
}

//applique un filtre médian de taille 5
void PreProcessing::getMedianFilter5(Mat &img)
{
    for(int i = 1 ; i < img.rows-1 ; i++)
        for(int j = 1 ; j < img.cols-1 ; j++)
        {
            int tmp = 0;
            tmp += img.at<unsigned char>(i-2,j-2);
            tmp += img.at<unsigned char>(i-2,j-1);
            tmp += img.at<unsigned char>(i-2,j);
            tmp += img.at<unsigned char>(i-2,j+1);
            tmp += img.at<unsigned char>(i-2,j+2);

            tmp += img.at<unsigned char>(i-1,j-2);
            tmp += img.at<unsigned char>(i-1,j-1);
            tmp += img.at<unsigned char>(i-1,j);
            tmp += img.at<unsigned char>(i-1,j+1);
            tmp += img.at<unsigned char>(i-1,j+2);

            tmp += img.at<unsigned char>(i,j-2);
            tmp += img.at<unsigned char>(i,j-1);
            tmp += img.at<unsigned char>(i,j);
            tmp += img.at<unsigned char>(i,j+1);
            tmp += img.at<unsigned char>(i,j+2);

            tmp += img.at<unsigned char>(i+1,j-2);
            tmp += img.at<unsigned char>(i+1,j-1);
            tmp += img.at<unsigned char>(i+1,j);
            tmp += img.at<unsigned char>(i+1,j+1);
            tmp += img.at<unsigned char>(i+1,j+2);

            tmp += img.at<unsigned char>(i+2,j-2);
            tmp += img.at<unsigned char>(i+2,j-1);
            tmp += img.at<unsigned char>(i+2,j);
            tmp += img.at<unsigned char>(i+2,j+1);
            tmp += img.at<unsigned char>(i+2,j+2);

            img.at<unsigned char>(i,j) = tmp/25;
        }
}

//applique un filtre pour étendre les valeurs de l'image sur 0-255
void PreProcessing::getExpansion(Mat &img)
{
    unsigned char max = 0;
    unsigned char min = 255 ;

    for(int i = 0 ; i < img.rows ; i++)
        for(int j = 0 ; j < img.cols ; j++)
        {
            if(img.at<unsigned char>(i,j) > max)
                max = img.at<unsigned char>(i,j);
            if(img.at<unsigned char>(i,j) < min)
                min = img.at<unsigned char>(i,j);
        }

    uchar tmp = max - min;
    for(int i = 0 ; i < img.rows ; i++)
        for(int j = 0 ; j < img.cols ; j++)
        {
            if(tmp != 0)
                img.at<unsigned char>(i,j) = (255*(img.at<unsigned char>(i,j)-min))/tmp;
        }
}

//applique un flou moyen à l'image de taille blurSize
Mat1b PreProcessing::getMedianBlur(Mat img, int blurSize)
{

    Mat1b blur = Mat(img.rows,img.cols,CV_8UC1);

    for ( int i = 1; i < blurSize; i = i + 2 )
    {
        medianBlur(img,blur,i);
    }
    return blur;
}
