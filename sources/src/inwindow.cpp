#include "inwindow.h"

INWindow::INWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui = new Ui::MainWindow();
    this->ui->setupUi(this);

    dialogData = new Ui::DialogData;
    m_dialog = new QDialog();
    dialogData->setupUi(m_dialog);

    //slide pour ajuster la valeur de seuil
    m_seuil_slide = new QSlider(Qt::Horizontal);
    m_seuil_slide->setRange(0,255);
    m_seuil_slide->setEnabled(false);

    /****************************** Tool Bar *****************************************************/
    m_toolBar = new QToolBar("Tool bar");
    m_seuil_text = new QLabel();
    //m_seuil_text->minimumWidth(50);
    m_toolBar->addWidget(m_seuil_slide);
    m_toolBar->addWidget(m_seuil_text);
    m_toolBar->setFloatable(false);
    m_toolBar->setMovable(false);
    this->addToolBar(m_toolBar);

    //connection du déplacement du slider de seuil au slot updateSeuil()
    QObject::connect(m_seuil_slide,SIGNAL(valueChanged(int)),this,SLOT(updateSeuil(int)));
    QObject::connect(ui->actionOpen_Image,SIGNAL(triggered()),this,SLOT(openDialog()));
    QObject::connect(ui->actionCreate_DataBase,SIGNAL(triggered()),this,SLOT(createDataDialog()));
    QObject::connect(dialogData->valid_button,SIGNAL(clicked()),this,SLOT(setData()));

    QObject::connect(dialogData->finger_0,SIGNAL(clicked()),this,SLOT(setFingerState0()));
    QObject::connect(dialogData->finger_1,SIGNAL(clicked()),this,SLOT(setFingerState1()));
    QObject::connect(dialogData->finger_2,SIGNAL(clicked()),this,SLOT(setFingerState2()));
    QObject::connect(dialogData->finger_3,SIGNAL(clicked()),this,SLOT(setFingerState3()));
    QObject::connect(dialogData->finger_4,SIGNAL(clicked()),this,SLOT(setFingerState4()));

    QObject::connect(dialogData->hand_open,SIGNAL(clicked()),this,SLOT(setHandStateO()));
    QObject::connect(dialogData->hand_close,SIGNAL(clicked()),this,SLOT(setHandStateC()));

    //création des objets de types Image (QLabel hérité)
    m_img_init = new Image();
    m_img_thres = new Image();
    m_img_hist = new Image();
    m_img_final = new Image();
    m_img_skel = new Image();
    m_img_hull = new Image();
    m_img_dialog = new Image();

    m_img_init->setFixedSize(200,200);
    m_img_thres->setFixedSize(200,200);
    m_img_final->setFixedSize(200,200);
    m_img_hist->setFixedSize(200,200);
    m_img_skel->setFixedSize(200,200);
    m_img_hull->setFixedSize(200,200);

    m_img_init->show();
    m_img_thres->show();
    m_img_hist->show();
    m_img_final->show();
    m_img_skel->show();
    m_img_hull->show();

    dialogData->image_layout->addWidget(m_img_dialog);
    ui->centralLayout->addWidget(m_img_thres,0,2);
    ui->centralLayout->addWidget(m_img_init,0,0);
    ui->centralLayout->addWidget(m_img_hist,0,1);
    ui->centralLayout->addWidget(m_img_skel,1,0);
    ui->centralLayout->addWidget(m_img_final,1,2);
    ui->centralLayout->addWidget(m_img_hull,1,1);
}

//réagit au clic sur open
void INWindow::openDialog()
{
    //lecture du fichier .yml
    QString filename = QFileDialog::getOpenFileName(this,"Open an image ...","./","*.yml");

    if(filename != "")
    {
        FileStorage fs(filename.toStdString(), FileStorage::READ);
        ui->centralLayout->removeWidget(m_img_init);
        Mat temp;

        fs["M1"] >> temp;

        //assignation de la cv::Mat contenant l'image et affection au QLabel
        m_mat_init = PreProcessing::getUCHARImage(temp,5.0);
        PreProcessing::getExpansion(m_mat_init);
        PreProcessing::getMedianFilter3(m_mat_init);

        m_img_init->setImage(m_mat_init);

        //assignation de la cv::Mat contenant l'histogramme et affection au QLabel
        h = new Histogram1D(m_mat_init);
        m_mat_hist =  h->getHistogramImage(h->getHistogram());
        h->cumulHist();
        h->derivCumul();

        m_img_hist->setImage(m_mat_hist);

        //calcul du seuil initial
        std::vector<unsigned char> dSeuil = h->getSeuilByDerivCumul();
        std::vector<unsigned char>::iterator it = dSeuil.begin();
        for(;it !=dSeuil.end();it++)
            if((*it) > m_seuil_slide->minimum())
            {
                m_seuil = 30;// (*it);
                m_seuil_slide->setEnabled(true);
                m_seuil_slide->setValue((int)m_seuil);
                m_seuil_text->setText(QString::number(m_seuil));
                break;
            }
        compute();
    }
}

//met à jour le seuil et rappelle la fonction compute
void INWindow::updateSeuil(int val)
{
    m_seuil = val;
    m_seuil_text->setText(QString::number(m_seuil));
    compute();
}

//effectue les différents traitements et reconnaissances d'image à partir de l'image de la main seuillée
void INWindow::compute()
{
    //seuillage de l'image
    m_mat_thres = Processing::threshold(m_mat_init,m_seuil);
    m_mat_thres=PreProcessing::getMedianBlur(m_mat_thres,8);
    m_mat_thres = Processing::getInverse(m_mat_thres);
    m_img_thres->setImage(m_mat_thres);

    //calcul du squelette
    m_mat_skel = Processing::getThinning(m_mat_thres,Processing::CONNEXITY_8,50);

    //calcul des contours afin de recherche l'enveloppe convexe
    findContours(m_mat_thres, m_contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    if(!m_contours.empty())
    {
        //calcul de l'enveloppe convexe
        m_hull = Processing::getConvexHull(m_contours[0]);
        m_hullI = Processing::getConvexHullI(m_contours[0]);

        //calcul des defect points
        m_defects = Processing::convexityDefects(m_contours[0],m_hullI);
        m_defectsPoint = Processing::convexityDefectsPoints(m_contours[0],m_hullI);
    }

    Mat distance = Processing::getDistanceTransform(m_mat_thres);
    Mat squeletteUChar = PreProcessing::getUCHARImage(distance,1.0);
    PreProcessing::getExpansion(squeletteUChar);

    //calcul des points significatifs du squelette
    m_center = Processing::getDistanceMax(squeletteUChar);
    m_vec_extrem = Processing::getSkelExtremity(m_mat_skel);
    m_vec_multi = Processing::getMultiPoints(m_mat_skel);

    //calcul de la reconnaissance
    m_rec_fing = Recognition::getFingerRecognition(m_defectsPoint);
    m_rec_hand = Recognition::getHandRecognition(m_defects,m_hull);

    //dessin des points du squelette
    drawPoints();

    //dessin de l'enveloppe convexe et de l'enveloppe des defect points
    drawConvexhull();

    //dessin de la reconnaissance
    drawAllRecognition();
}

//dessine les différents points et lignes du squelettes
void INWindow::drawPoints()
{
    if(!m_vec_extrem.empty() || !m_vec_multi.empty())
    {
        Mat tmp;
        cvtColor(m_mat_init,tmp,CV_GRAY2RGB);
        for(int i = 0; i < m_mat_skel.rows; i++)
            for(int j = 0 ; j < m_mat_skel.cols ; j++)
            {
                if(m_mat_skel.at<unsigned char>(i,j) == 255)
                    tmp.at<Vec3b>(i,j) = Vec3b(255,255,255);
            }
        Point2i l = m_center;
        //dessin du point central
        circle(tmp,l,1,Scalar_<unsigned char>(0,0,255));
        std::vector<Point2i>::iterator it = m_vec_extrem.begin();
        for(;it != m_vec_extrem.end();it++)
        {
            Point2i pt = (*it);
            //dessin des points extrêmes
            circle(tmp,pt,1,Scalar_<unsigned char>(255,0,0));
            //dessin des lignes de distances
            line(tmp,pt,l,Scalar_<unsigned char>(0,0,255));
        }
        std::vector<Point2i>::iterator it1 = m_vec_multi.begin();
        for(;it1 != m_vec_multi.end();it1++)
        {
            Point2i pt = (*it1);
            //dessin des points multiples
            circle(tmp,pt,1,Scalar_<unsigned char>(0,255,0));
        }
        m_img_skel->setImage(tmp);
    }
}

//dessine l'enveloppe convexe, les defect points retenus et l'enveloppe des defect points
void INWindow::drawConvexhull()
{


    Mat tmp;
    cvtColor(m_mat_init,tmp,CV_GRAY2RGB);
    if(!m_defectsPoint.empty())
    {
        for(int i = 0; i < m_defectsPoint.size() ; i++)
        {
            //dessin des defect points retenus
            circle(tmp,m_defectsPoint[i],3,Scalar_<unsigned char>(255,0,0));
        }
    }
    if(!m_defects.empty())
    {
        for(int i = 0; i < m_defects.size()-1 ; i++)
        {
            //dessin de l'enveloppe des defect points
            line(tmp,m_defects[i],m_defects[i+1],Scalar_<unsigned char>(0,0,255));
            if((i+1) == m_defects.size()-1)
                line(tmp,m_defects[i+1],m_defects[0],Scalar_<unsigned char>(0,0,255));
        }
    }
    if(!m_hull.empty() ){
        for(int i = 0; i < m_hull.size()-1 ; i++)
        {
            //dessin de l'enveloppe convexe
            line(tmp,m_hull[i],m_hull[i+1],Scalar_<unsigned char>(255,255,0));
            if((i+1) == m_hull.size()-1)
                line(tmp,m_hull[i+1],m_hull[0],Scalar_<unsigned char>(255,255,0));
        }
    }
    m_img_hull->setImage(tmp);

}

//dessine les résultats de la reconnaissance
void INWindow::drawAllRecognition()
{

    if(!m_hull.empty() || !m_vec_extrem.empty() || !m_vec_multi.empty())
    {
        Mat tmp;
        int nb = m_defectsPoint.size();

        cvtColor(m_mat_init,tmp,CV_GRAY2RGB);
        Point2i l = m_center;
        circle(tmp,l,1,Scalar_<unsigned char>(0,0,255));
        std::vector<Point2i>::iterator it = m_vec_extrem.begin();
        for(;it != m_vec_extrem.end();it++)
        {
            Point2i pt = (*it);
            circle(tmp,pt,1,Scalar_<unsigned char>(255,0,0));
            line(tmp,pt,l,Scalar_<unsigned char>(0,255,255));
        }
        std::vector<Point2i>::iterator it1 = m_vec_multi.begin();
        for(;it1 != m_vec_multi.end();it1++)
        {
            Point2i pt = (*it1);
            circle(tmp,pt,1,Scalar_<unsigned char>(0,255,0));
        }
        for(int i = 0; i < m_defectsPoint.size() ; i++)
        {
            circle(tmp,m_defectsPoint[i],3,Scalar_<unsigned char>(255,0,0));
        }
        for(int i = 0; i < m_hull.size()-1 ; i++)
        {
            line(tmp,m_hull[i],m_hull[i+1],Scalar_<unsigned char>(255,255,0));
            if((i+1) == m_hull.size()-1)
                line(tmp,m_hull[i+1],m_hull[0],Scalar_<unsigned char>(255,255,0));
        }

        std::string text_fing;
        std::string text_hand;

        switch(m_rec_fing)
        {
        case Recognition::HAND_0:
            text_fing = " 0 finger are detected";
            break;
        case Recognition::HAND_1:
            text_fing = " 1 finger is detected";
            break;
        case Recognition::HAND_2:
            text_fing = " 2 fingers are detected";
            break;
        case Recognition::HAND_3:
            text_fing = " 3 fingers are detected";
            break;
        case Recognition::HAND_4:
            text_fing = " 4 fingers are detected";
            break;
        case Recognition::HAND_5:
            text_fing = " 5 fingers are detected";
            break;
        }
        switch(m_rec_hand)
        {
        case Recognition::HAND_OPEN:
            text_hand = " The hand is open";
            break;
        case Recognition::HAND_CLOSE:
            text_hand = " The hand is closed";
            break;
        }

        putText(tmp, text_fing, Point(0, 20),FONT_HERSHEY_SIMPLEX,0.3, Scalar_<unsigned char>(255, 255, 255));
        putText(tmp, text_hand, Point(0, 40),FONT_HERSHEY_SIMPLEX,0.3, Scalar_<unsigned char>(255, 255, 255));

        m_img_final->setImage(tmp);
    }
}

//réagit au clic sur create data
void INWindow::createDataDialog()
{
    m_list_name = QFileDialog::getOpenFileNames(this,"Get Images ...","./","*.yml");
    addDataImage();
}

//ouvre les images et propose les options pour l'ajout
void INWindow::addDataImage()
{
    if(!m_list_name.empty())
    {
        m_dialog->show();

        FileStorage fs(((QString)m_list_name.first()).toStdString(), FileStorage::READ);
        ui->centralLayout->removeWidget(m_img_init);
        Mat temp;

        fs["M1"] >> temp;

        m_current_image = PreProcessing::getUCHARImage(temp,5.0);
        PreProcessing::getExpansion(m_current_image);
        PreProcessing::getMedianFilter3(m_current_image);

        unsigned char seuil;
        Histogram1D *hist = new Histogram1D(m_current_image);
        hist->cumulHist();
        hist->derivCumul();

        std::vector<unsigned char> dSeuil = hist->getSeuilByDerivCumul();
        std::vector<unsigned char>::iterator it = dSeuil.begin();
        for(;it !=dSeuil.end();it++)
            if((*it) > m_seuil_slide->minimum() && (*it) < m_seuil_slide->maximum())
                seuil = (*it);


        Mat thresholded = Processing::threshold(m_current_image,seuil);
        Mat blur=PreProcessing::getMedianBlur(thresholded,8);
        imshow("Blur",blur);
        Mat inv = Processing::getInverse(blur);

        m_extract_data_coords = Processing::getExtractCoord(blur);
        Mat current;
        cvtColor(m_current_image,current,CV_GRAY2RGB);

        line(current,m_extract_data_coords[0],m_extract_data_coords[1],Scalar(0,255,0));
        line(current,m_extract_data_coords[1],m_extract_data_coords[2],Scalar(0,255,0));
        line(current,m_extract_data_coords[2],m_extract_data_coords[3],Scalar(0,255,0));
        line(current,m_extract_data_coords[3],m_extract_data_coords[0],Scalar(0,255,0));

        m_img_dialog->setImage(current);
        m_list_name.removeFirst();
    }
    else m_dialog->hide();
}

//ajoute les données à une base de données
void INWindow::setData()
{
    datImage temp;
    //temp.dist = Processing::getDensity(blur,5,5);
    temp.img = m_current_image;
    temp.state_fingers = m_state_fing;
    temp.state_hand = m_state_hand;

    m_vec_data.push_back(temp);

    addDataImage();
}
