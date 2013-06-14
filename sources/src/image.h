#ifndef IMAGE_H
#define IMAGE_H

#include <QLabel>
#include <QImage>
#include <QDebug>
#include <opencv2/opencv.hpp>

class Image : public QLabel
{

public:
    explicit Image(QWidget *parent = 0);
    QImage convertCvMatToQImage(cv::Mat mat);
    void setImage(cv::Mat mat);
signals:
    
public slots:

private:
    QImage m_img;

};

#endif // IMAGE_H
