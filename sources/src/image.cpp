#include "image.h"

Image::Image(QWidget *parent) :
    QLabel(parent)
{
    this->setMinimumSize(200,200);
    this->setPixmap(QPixmap(200,200));
}

// conversion d'une cv::Mat à une QImage afin de l'afficher dans une fenêtre Qt
QImage Image::convertCvMatToQImage(cv::Mat mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

// set l'image envoyé au paramètre dans notre QLabel hérité
void Image::setImage(cv::Mat mat)
{
    m_img=convertCvMatToQImage(mat);
    this->setPixmap(QPixmap::fromImage(m_img));
}
