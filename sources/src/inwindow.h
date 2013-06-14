#ifndef INWINDOW_H
#define INWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QSlider>
#include <QFileDialog>
#include <opencv2/opencv.hpp>

#include "histogram1d.h"
#include "preprocessing.h"
#include "processing.h"
#include "recognition.h"
#include "image.h"

#include "ui_dialogdata.h"

#include "ui_mainwindow.h"

using namespace cv;

class INWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit INWindow(QWidget *parent = 0);
    struct datImage {
        std::vector<double> dist;
        Recognition::STATE_HAND state_hand;
        Recognition::STATE_FINGERS state_fingers;
        Mat img;
    };
    
signals:
    
public slots:
    void updateSeuil(int);
    void openDialog();
    void createDataDialog();
    void setData();

    void setHandStateO(){m_state_hand = Recognition::HAND_OPEN;}
    void setHandStateC(){m_state_hand = Recognition::HAND_CLOSE;}

    void setFingerState0(){m_state_fing = Recognition::HAND_0;}
    void setFingerState1(){m_state_fing = Recognition::HAND_1;}
    void setFingerState2(){m_state_fing = Recognition::HAND_2;}
    void setFingerState3(){m_state_fing = Recognition::HAND_3;}
    void setFingerState4(){m_state_fing = Recognition::HAND_4;}
    void setFingerState5(){m_state_fing = Recognition::HAND_5;}

private:
    QToolBar * m_toolBar;
    QLabel * m_seuil_text;
    QSlider * m_seuil_slide;
    Ui::MainWindow * ui;
    Ui::DialogData * dialogData;
    QDialog * m_dialog;
    Image * m_img_init;
    Image * m_img_thres;
    Image * m_img_hist;
    Image * m_img_final;
    Image * m_img_skel;
    Image * m_img_hull;
    Image * m_img_dialog;
    QStringList m_list_name;
    bool isStored;

    Mat m_mat_init;
    Mat m_mat_thres;
    Mat m_mat_inv;
    Mat m_mat_hist;
    Mat m_mat_skel;
    Mat m_current_image;

    std::vector<datImage> m_vec_data;
    std::vector<Point2i> m_vec_extrem;
    std::vector<Point2i> m_vec_multi;
    std::vector<Point2i> m_extract_coords;
    std::vector<Point2i> m_extract_data_coords;
    std::vector<std::vector<Point2i> > m_contours;
    std::vector<Point2i> m_hull;
    std::vector<int> m_hullI;
    std::vector<Point2i> m_defects;
    std::vector<Point2i> m_defectsPoint;
    Point2i m_center;

    Histogram1D *  h;
    int m_seuil;
    Recognition::STATE_FINGERS m_state_fing;
    Recognition::STATE_HAND m_state_hand;

    Recognition::STATE_FINGERS m_rec_fing;
    Recognition::STATE_HAND m_rec_hand;


    void compute();
    void drawPoints();
    void drawConvexhull();
    void drawAllRecognition();
    void addDataImage();
};

#endif // INWINDOW_H
