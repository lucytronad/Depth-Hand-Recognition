#include "processing.h"

Processing::Processing()
{
}

//applique un multi-seuillage
Mat Processing::multyThreshold(Mat src, std::vector<unsigned char> seuils)
{
    Mat dst = Mat(src.rows,src.cols,CV_8UC1,Scalar(255));

    std::vector<unsigned char>::iterator it = seuils.begin();

    int color = 255/seuils.size();

    int k = 0;
    for(it;it != seuils.end(); it++)
    {
        for (int i  = 0 ; i < src.rows ; i++)
            for (int j = 0 ; j < src.cols ; j ++)
            {
                if(dst.at<unsigned char>(i,j) == 255 && src.at<unsigned char>(i,j) <= *it)
                {
                    dst.at<unsigned char>(i,j) = color*k;
                }
            }
        k++;
    }
    return dst;
}

//applique un seuillage
Mat Processing::threshold(Mat src, unsigned char seuil)
{
    Mat dst = Mat(src.rows,src.cols,CV_8UC1,Scalar(255));

    for (int i  = 0 ; i < src.rows ; i++)
        for (int j = 0 ; j < src.cols ; j ++)
        {
            if(src.at<unsigned char>(i,j) <= seuil)
                dst.at<unsigned char>(i,j) = 0;
            else 255;
        }
    return dst;
}

//applique une érosion
Mat Processing::getErosion(Mat img,InputArray kernel,int iteration)
{
    Mat dst = Mat(img.rows,img.cols,CV_8UC1);

    erode(img,dst,kernel,Point(-1,-1),iteration);

    return dst;

}

//applique une dilatation
Mat Processing::getDilatation(Mat img,InputArray kernel,int iteration)
{
    Mat dst = Mat(img.rows,img.cols,CV_8UC1);

    dilate(img,dst,kernel,Point(-1,-1),iteration);

    return dst;

}

//retourne les coordonnées d'extraction de la main
std::vector<Point2i> Processing::getExtractCoord(Mat img)
{
    int xL = 0;
    int xR = img.rows;
    int yU = 0;
    int yD = img.cols;
    std::vector<Point2i> coords;

    bool isXLassigned=false,isXRassigned=false,isYUassigned=false,isYDassigned=false;

    for(int i=0; i<img.rows;i++)
    {
        if(isYUassigned) break;

        for(int j=0; j<img.cols;j++)
        {
            if(img.at<unsigned char>(i,j)==0)
            {
                yU=i-1;
                isYUassigned=true;
                break;
            }
        }

    }
    for(int i=yU+1; i<img.rows;i++)
    {
        bool containsBlackPx=false;
        if(isYDassigned) break;

        for(int j=0; j<img.cols;j++)
        {
            if(img.at<unsigned char>(i,j)==0)
            {
                containsBlackPx=true;
                break;
            }
        }
        if(!containsBlackPx)
        {
            yD=i+1;
            isYDassigned=true;
        }
    }

    for(int j=0; j<img.cols;j++)
    {
        if(isXLassigned) break;

        for(int i=yU; i<yD;i++)
        {
            if(img.at<unsigned char>(i,j)==0)
            {
                xL=j-1;
                isXLassigned=true;
                break;
            }
        }

    }
    for(int j=xL+1; j<img.cols;j++)
    {
        bool containsBlackPx=false;
        if(isXRassigned) break;

        for(int i=yU; i<yD;i++)
        {
            if(img.at<unsigned char>(i,j)==0)
            {
                containsBlackPx=true;
                break;
            }
        }
        if(!containsBlackPx)
        {
            xR=j+1;
            isXRassigned=true;
        }
    }

    coords.push_back(Point2i(xL,yU));
    coords.push_back(Point2i(xR,yU));
    coords.push_back(Point2i(xR,yD));
    coords.push_back(Point2i(xL,yD));

    return coords;
}

//retourne la cv::Mat contenant la main extraite
Mat Processing::getExtractMat(Mat img, std::vector<Point2i> coords)
{

    int width=coords[1].x -coords[0].x;
    int height=coords[2].y-coords[1].y;

    Rect rect = Rect(coords[0].x,coords[0].y,width,height);
    Mat extracted = img(rect);

    return extracted;
}

//retourne un élément structurant
Mat Processing::getKernel(int shape,Size size)
{
    return getStructuringElement(shape,size);
}

//retourne l'inverse de l'image
Mat Processing::getInverse(Mat img)
{
    Mat dst = Mat(img.rows,img.cols,CV_8UC1,Scalar(255));

    for (int i  = 0 ; i < img.rows ; i++)
        for (int j = 0 ; j < img.cols ; j ++)
        {
            dst.at<unsigned char>(i,j) = 255 - img.at<unsigned char>(i,j);
        }
    return dst;
}

//retourne la carte des distances de la main
Mat Processing::getDistanceTransform(Mat img)
{
    Mat dst = Mat(img.rows,img.cols,CV_32FC1);
    distanceTransform(img,dst,CV_DIST_L2,CV_DIST_MASK_3);
    normalize(dst,dst,0.0,1.0,NORM_MINMAX);
    return dst;
}

//retourne un squelette calculé avec la carte des distances
Mat Processing::getSkeletonByDistanceMap(Mat dst)
{
    Mat laplace,tempSkeleton;
    Laplacian(dst, laplace, CV_16S, 3, 1, 0, BORDER_DEFAULT);
    compare(laplace,-3,tempSkeleton,CV_CMP_LE);

    Mat skeleton = tempSkeleton;

    return skeleton;
}

//retourne un squelette de la main par amincissement
Mat Processing::getThinning(Mat1b img,int code,int iteration)
{
    bool isDone = false;
    ELEM_STATE currentState = NO;
    Mat1b dst = img.clone();
    Mat1b diff = Mat(img.rows,img.cols,CV_8UC1);
    Mat1b tmp;
    tmp = dst.clone();
    int it =0;
    while(it < iteration*8)
    {
        it++;
        currentState = getNextState(currentState);

        for(int i = 1; i < img.rows-1; i++)
            for(int j=1; j < img.cols-1; j++)
            {
                switch(currentState)
                {
                case N:
                    if(dst.at<unsigned char>(i,j) == 255 &&
                            dst.at<unsigned char>(i-1,j-1) == 255 &&
                            dst.at<unsigned char>(i-1,j) == 255 &&
                            dst.at<unsigned char>(i-1,j+1) == 255 &&
                            dst.at<unsigned char>(i+1,j-1) == 0 &&
                            dst.at<unsigned char>(i+1,j) == 0 &&
                            dst.at<unsigned char>(i+1,j+1) == 0 ) tmp.at<unsigned char>(i,j) = 255;
                    else tmp.at<unsigned char>(i,j) = 0;
                    break;
                case NE:
                    if(dst.at<unsigned char>(i,j) == 255 &&
                            dst.at<unsigned char>(i-1,j) == 255 &&
                            dst.at<unsigned char>(i-1,j+1) == 255 &&
                            dst.at<unsigned char>(i,j+1) == 255 &&
                            dst.at<unsigned char>(i,j-1) == 0 &&
                            dst.at<unsigned char>(i+1,j-1) == 0 &&
                            dst.at<unsigned char>(i+1,j) == 0 )  tmp.at<unsigned char>(i,j) = 255;
                    else tmp.at<unsigned char>(i,j) = 0;
                    break;
                case E:
                    if(dst.at<unsigned char>(i,j) == 255 &&
                            dst.at<unsigned char>(i+1,j+1) == 255 &&
                            dst.at<unsigned char>(i,j+1) == 255 &&
                            dst.at<unsigned char>(i+1,j+1) == 255 &&
                            dst.at<unsigned char>(i-1,j-1) == 0 &&
                            dst.at<unsigned char>(i,j-1) == 0 &&
                            dst.at<unsigned char>(i+1,j-1) == 0 )  tmp.at<unsigned char>(i,j) = 255;
                    else tmp.at<unsigned char>(i,j) = 0;
                    break;
                case SE:
                    if(dst.at<unsigned char>(i,j) == 255 &&
                            dst.at<unsigned char>(i,j+1) == 255 &&
                            dst.at<unsigned char>(i+1,j) == 255 &&
                            dst.at<unsigned char>(i+1,j+1) == 255 &&
                            dst.at<unsigned char>(i-1,j-1) == 0 &&
                            dst.at<unsigned char>(i-1,j) == 0 &&
                            dst.at<unsigned char>(i,j-1) == 0 )  tmp.at<unsigned char>(i,j) = 255;
                    else tmp.at<unsigned char>(i,j) = 0;
                    break;
                case S:
                    if(dst.at<unsigned char>(i,j) == 255 &&
                            dst.at<unsigned char>(i+1,j-1) == 255 &&
                            dst.at<unsigned char>(i+1,j) == 255 &&
                            dst.at<unsigned char>(i+1,j+1) == 255 &&
                            dst.at<unsigned char>(i-1,j-1) == 0 &&
                            dst.at<unsigned char>(i-1,j) == 0 &&
                            dst.at<unsigned char>(i-1,j+1) == 0 )  tmp.at<unsigned char>(i,j) = 255;
                    else tmp.at<unsigned char>(i,j) = 0;
                    break;
                case SO:
                    if(dst.at<unsigned char>(i,j) == 255 &&
                            dst.at<unsigned char>(i,j-1) == 255 &&
                            dst.at<unsigned char>(i+1,j-1) == 255 &&
                            dst.at<unsigned char>(i+1,j) == 255 &&
                            dst.at<unsigned char>(i-1,j) == 0 &&
                            dst.at<unsigned char>(i-1,j+1) == 0 &&
                            dst.at<unsigned char>(i,j+1) == 0 )  tmp.at<unsigned char>(i,j) = 255;
                    else tmp.at<unsigned char>(i,j) = 0;
                    break;
                case O:
                    if(dst.at<unsigned char>(i,j) == 255 &&
                            dst.at<unsigned char>(i-1,j-1) == 255 &&
                            dst.at<unsigned char>(i,j-1) == 255 &&
                            dst.at<unsigned char>(i+1,j-1) == 255 &&
                            dst.at<unsigned char>(i-1,j+1) == 0 &&
                            dst.at<unsigned char>(i,j+1) == 0 &&
                            dst.at<unsigned char>(i+1,j+1) == 0 )  tmp.at<unsigned char>(i,j) = 255;
                    else tmp.at<unsigned char>(i,j) = 0;
                    break;
                case NO:
                    if(dst.at<unsigned char>(i,j) == 255 &&
                            dst.at<unsigned char>(i-1,j-1) == 255 &&
                            dst.at<unsigned char>(i-1,j) == 255 &&
                            dst.at<unsigned char>(i,j-1) == 255 &&
                            dst.at<unsigned char>(i,j+1) == 0 &&
                            dst.at<unsigned char>(i+1,j) == 0 &&
                            dst.at<unsigned char>(i+1,j+1) == 0 )  tmp.at<unsigned char>(i,j) = 255;
                    else tmp.at<unsigned char>(i,j) = 0;
                    break;
                default:
                    break;
                }
            }

        for(int i = 1; i < dst.rows-1; i++)
            for(int j=1; j < dst.cols-1; j++)
            {
                if(tmp.at<unsigned char>(i,j) == 255)
                    dst.at<unsigned char>(i,j) = 0;
            }
    }
    return dst;
}


Processing::ELEM_STATE Processing::getNextState(Processing::ELEM_STATE cur)
{
    ELEM_STATE next;
    switch(cur)
    {
    case N:
        next = NE;
        break;
    case NE:
        next = E;
        break;
    case E:
        next = SE;
        break;
    case SE:
        next = S;
        break;
    case S:
        next = SO;
        break;
    case SO:
        next = O;
        break;
    case O:
        next = NO;
        break;
    case NO:
        next = N;
        break;
    }
    return next;
}

//retourne le point central du squelette
Point2i Processing::getDistanceMax(Mat img)
{
    Point_<int> pt;
    int indX = 0;
    int indY = 0;
    unsigned char max = 0;
    for(int i = 0; i < img.rows; i++)
        for(int j=0; j < img.cols; j++)
        {
            if(img.at<unsigned char>(i,j) >= max)
            {
                max = img.at<unsigned char>(i,j);
                indX = i;
                indY = j;
            }
        }
    pt = Point_<int>(indY,indX);
    return pt;
}

//retour les points extrêmes du squelette
std::vector<Point2i> Processing::getSkelExtremity(Mat img)
{
    ELEM_STATE currentState = NO;
    Point2i pt;
    std::vector<Point2i> vect_pt;
    int it =0;
    while(it < 8)
    {
        it++;
        currentState = getNextState(currentState);

        for(int i = 1; i < img.rows-1; i++)
            for(int j=1; j < img.cols-1; j++)
            {
                switch(currentState)
                {
                case N:
                    if(img.at<unsigned char>(i,j) == 255 &&
                            img.at<unsigned char>(i-1,j) == 255 &&
                            img.at<unsigned char>(i,j-1) == 0 &&
                            img.at<unsigned char>(i+1,j-1) == 0 &&
                            img.at<unsigned char>(i+1,j) == 0 &&
                            img.at<unsigned char>(i+1,j+1) == 0 &&
                            img.at<unsigned char>(i,j+1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case NE:
                    if(img.at<unsigned char>(i,j) == 255 &&
                            img.at<unsigned char>(i-1,j+1) == 255 &&
                            img.at<unsigned char>(i-1,j-1) == 0 &&
                            img.at<unsigned char>(i,j-1) == 0 &&
                            img.at<unsigned char>(i+1,j-1) == 0 &&
                            img.at<unsigned char>(i+1,j) == 0 &&
                            img.at<unsigned char>(i+1,j+1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case E:
                    if(img.at<unsigned char>(i,j) == 255 &&
                            img.at<unsigned char>(i,j+1) == 255 &&
                            img.at<unsigned char>(i-1,j-1) == 0 &&
                            img.at<unsigned char>(i-1,j) == 0 &&
                            img.at<unsigned char>(i,j-1) == 0 &&
                            img.at<unsigned char>(i+1,j-1) == 0 &&
                            img.at<unsigned char>(i+1,j) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case SE:
                    if(img.at<unsigned char>(i,j) == 255 &&
                            img.at<unsigned char>(i+1,j+1) == 255 &&
                            img.at<unsigned char>(i-1,j+1) == 0 &&
                            img.at<unsigned char>(i-1,j) == 0 &&
                            img.at<unsigned char>(i-1,j-1) == 0 &&
                            img.at<unsigned char>(i,j-1) == 0 &&
                            img.at<unsigned char>(i+1,j-1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case S:
                    if(img.at<unsigned char>(i,j) == 255 &&
                            img.at<unsigned char>(i+1,j) == 255 &&
                            img.at<unsigned char>(i,j+1) == 0 &&
                            img.at<unsigned char>(i-1,j+1) == 0 &&
                            img.at<unsigned char>(i-1,j) == 0 &&
                            img.at<unsigned char>(i-1,j-1) == 0 &&
                            img.at<unsigned char>(i,j-1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case SO:
                    if(img.at<unsigned char>(i,j) == 255 &&
                            img.at<unsigned char>(i+1,j-1) == 255 &&
                            img.at<unsigned char>(i+1,j+1) == 0 &&
                            img.at<unsigned char>(i,j+1) == 0 &&
                            img.at<unsigned char>(i-1,j+1) == 0 &&
                            img.at<unsigned char>(i-1,j) == 0 &&
                            img.at<unsigned char>(i-1,j-1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case O:
                    if(img.at<unsigned char>(i,j) == 255 &&
                            img.at<unsigned char>(i,j-1) == 255 &&
                            img.at<unsigned char>(i-1,j) == 0 &&
                            img.at<unsigned char>(i-1,j+1) == 0 &&
                            img.at<unsigned char>(i,j+1) == 0 &&
                            img.at<unsigned char>(i+1,j+1) == 0 &&
                            img.at<unsigned char>(i+1,j) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case NO:
                    if(img.at<unsigned char>(i,j) == 255 &&
                            img.at<unsigned char>(i-1,j-1) == 255 &&
                            img.at<unsigned char>(i-1,j+1) == 0 &&
                            img.at<unsigned char>(i,j+1) == 0 &&
                            img.at<unsigned char>(i+1,j+1) == 0 &&
                            img.at<unsigned char>(i+1,j) == 0 &&
                            img.at<unsigned char>(i+1,j-1) == 0 ){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                default:
                    break;
                }
            }
    }
    return vect_pt;
}

//retourne les points multiples du squelette
std::vector<Point2i> Processing::getMultiPoints(Mat img)
{
    ELEM_STATE currentState = NO;
    Point2i pt;
    std::vector<Point2i> vect_pt;
    int it =0;
    while(it < 8)
    {
        it++;
        currentState = getNextState(currentState);

        for(int i = 1; i < img.rows-1; i++)
            for(int j=1; j < img.cols-1; j++)
            {
                switch(currentState)
                {
                case N:
                    if((img.at<unsigned char>(i,j-1) == 255 &&
                        img.at<unsigned char>(i,j) == 255 &&
                        img.at<unsigned char>(i,j+1) == 255 &&
                        img.at<unsigned char>(i+1,j) == 255 &&
                        img.at<unsigned char>(i-1,j-1) == 0 &&
                        img.at<unsigned char>(i-1,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j-1) == 0) ||
                            (img.at<unsigned char>(i,j-1) == 255 &&
                             img.at<unsigned char>(i,j) == 255 &&
                             img.at<unsigned char>(i,j+1) == 255 &&
                             img.at<unsigned char>(i+1,j) == 255 &&
                             img.at<unsigned char>(i-1,j) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case NE:
                    if((img.at<unsigned char>(i-1,j-1) == 255 &&
                        img.at<unsigned char>(i,j) == 255 &&
                        img.at<unsigned char>(i+1,j-1) == 255 &&
                        img.at<unsigned char>(i+1,j+1) == 255 &&
                        img.at<unsigned char>(i-1,j) == 0 &&
                        img.at<unsigned char>(i,j-1) == 0 &&
                        img.at<unsigned char>(i,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j) == 0)||
                            (img.at<unsigned char>(i-1,j-1) == 255 &&
                             img.at<unsigned char>(i,j) == 255 &&
                             img.at<unsigned char>(i+1,j-1) == 255 &&
                             img.at<unsigned char>(i+1,j+1) == 255 &&
                             img.at<unsigned char>(i-1,j+1) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case E:
                    if((img.at<unsigned char>(i-1,j) == 255 &&
                        img.at<unsigned char>(i,j) == 255 &&
                        img.at<unsigned char>(i+1,j) == 255 &&
                        img.at<unsigned char>(i,j-1) == 255 &&
                        img.at<unsigned char>(i-1,j-1) == 0 &&
                        img.at<unsigned char>(i-1,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j-1) == 0)||
                            (img.at<unsigned char>(i-1,j) == 255 &&
                             img.at<unsigned char>(i,j) == 255 &&
                             img.at<unsigned char>(i+1,j) == 255 &&
                             img.at<unsigned char>(i,j-1) == 255 &&
                             img.at<unsigned char>(i,j+1) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case SE:
                    if((img.at<unsigned char>(i,j) == 255 &&
                        img.at<unsigned char>(i-1,j-1) == 255 &&
                        img.at<unsigned char>(i-1,j+1) == 255 &&
                        img.at<unsigned char>(i+1,j-1) == 255 &&
                        img.at<unsigned char>(i-1,j) == 0 &&
                        img.at<unsigned char>(i,j-1) == 0 &&
                        img.at<unsigned char>(i,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j) == 0)||
                            (img.at<unsigned char>(i,j) == 255 &&
                             img.at<unsigned char>(i-1,j-1) == 255 &&
                             img.at<unsigned char>(i-1,j+1) == 255 &&
                             img.at<unsigned char>(i+1,j-1) == 255 &&
                             img.at<unsigned char>(i-1,j) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case S:
                    if((img.at<unsigned char>(i,j) == 255 &&
                        img.at<unsigned char>(i-1,j) == 255 &&
                        img.at<unsigned char>(i,j+1) == 255 &&
                        img.at<unsigned char>(i,j-1) == 255 &&
                        img.at<unsigned char>(i-1,j-1) == 0 &&
                        img.at<unsigned char>(i-1,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j-1) == 0)||
                            (img.at<unsigned char>(i,j) == 255 &&
                             img.at<unsigned char>(i-1,j) == 255 &&
                             img.at<unsigned char>(i,j+1) == 255 &&
                             img.at<unsigned char>(i,j-1) == 255 &&
                             img.at<unsigned char>(i+1,j) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case SO:
                    if((img.at<unsigned char>(i,j) == 255 &&
                        img.at<unsigned char>(i-1,j-1) == 255 &&
                        img.at<unsigned char>(i-1,j+1) == 255 &&
                        img.at<unsigned char>(i+1,j+1) == 255 &&
                        img.at<unsigned char>(i-1,j) == 0 &&
                        img.at<unsigned char>(i,j-1) == 0 &&
                        img.at<unsigned char>(i,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j) == 0)||
                            (img.at<unsigned char>(i,j) == 255 &&
                             img.at<unsigned char>(i-1,j-1) == 255 &&
                             img.at<unsigned char>(i-1,j+1) == 255 &&
                             img.at<unsigned char>(i+1,j+1) == 255 &&
                             img.at<unsigned char>(i+1,j-1) == 0 )){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case O:
                    if((img.at<unsigned char>(i,j) == 255 &&
                        img.at<unsigned char>(i-1,j) == 255 &&
                        img.at<unsigned char>(i,j+1) == 255 &&
                        img.at<unsigned char>(i+1,j) == 255 &&
                        img.at<unsigned char>(i-1,j-1) == 0 &&
                        img.at<unsigned char>(i-1,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j-1) == 0)||
                            (img.at<unsigned char>(i,j) == 255 &&
                             img.at<unsigned char>(i-1,j) == 255 &&
                             img.at<unsigned char>(i,j+1) == 255 &&
                             img.at<unsigned char>(i+1,j) == 255 &&
                             img.at<unsigned char>(i,j-1) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                case NO:
                    if((img.at<unsigned char>(i,j) == 255 &&
                        img.at<unsigned char>(i-1,j+1) == 255 &&
                        img.at<unsigned char>(i+1,j+1) == 255 &&
                        img.at<unsigned char>(i+1,j-1) == 255 &&
                        img.at<unsigned char>(i-1,j) == 0 &&
                        img.at<unsigned char>(i,j-1) == 0 &&
                        img.at<unsigned char>(i,j+1) == 0 &&
                        img.at<unsigned char>(i+1,j) == 0)||
                            (img.at<unsigned char>(i,j) == 255 &&
                             img.at<unsigned char>(i-1,j+1) == 255 &&
                             img.at<unsigned char>(i+1,j+1) == 255 &&
                             img.at<unsigned char>(i+1,j-1) == 255 &&
                             img.at<unsigned char>(i-1,j-1) == 0)){
                        pt = Point2i(j,i);
                        vect_pt.push_back(pt);
                    }
                    break;
                default:
                    break;
                }
            }
    }
    return vect_pt;
}

// retourne les defect points avec contrôle de la taille de l'aire
std::vector<Point2i> Processing::convexityDefectsPoints(std::vector<Point> contour, std::vector<int> hull){
    std::vector<Point> convexDefects;
    if(hull.size() > 0 && contour.size() > 0){
        CvSeq* contourPoints;
        CvSeq* defects;
        CvMemStorage* storage;
        CvMemStorage* strDefects;
        CvMemStorage* contourStr;
        CvConvexityDefect *defectArray = 0;
        double maxArea = 17;

        strDefects = cvCreateMemStorage();
        defects = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvSeq),sizeof(CvPoint), strDefects );

        contourStr = cvCreateMemStorage();
        contourPoints = cvCreateSeq(CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvSeq), sizeof(CvPoint), contourStr);
        for(int i=0; i<(int)contour.size(); i++) {
            CvPoint cp = {contour[i].x,  contour[i].y};
            cvSeqPush(contourPoints, &cp);
        }

        int count = (int)hull.size();
        //int hullK[count];
        int* hullK = (int*)malloc(count*sizeof(int));
        for(int i=0; i<count; i++){hullK[i] = hull.at(i);}
        CvMat hullMat = cvMat(1, count, CV_32SC1, hullK);

        storage = cvCreateMemStorage(0);
        defects = cvConvexityDefects(contourPoints, &hullMat, storage);

        defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*defects->total);
        cvCvtSeqToArray(defects, defectArray, CV_WHOLE_SEQ);

        for(int i = 0; i<defects->total; i++){
            CvPoint ptf;
            ptf.x = defectArray[i].depth_point->x;
            ptf.y = defectArray[i].depth_point->y;

            std::vector<Point2i> cont;

            Point2i st (defectArray[i].start->x,defectArray[i].start->y);
            Point2i end (defectArray[i].end->x,defectArray[i].end->y);
            Point2i depth (defectArray[i].depth_point->x,defectArray[i].depth_point->y);
            cont.push_back(st);
            cont.push_back(end);
            cont.push_back(depth);

            double area = contourArea(cont);
            if(area > maxArea)
                convexDefects.push_back(ptf);
        }

        cvReleaseMemStorage(&contourStr);
        cvReleaseMemStorage(&strDefects);
        cvReleaseMemStorage(&storage);
    }
    return convexDefects;
}

// retourne la totalité des defects points
std::vector<Point2i> Processing::convexityDefects(std::vector<Point> contour, std::vector<int> hull){
     std::vector<Point> convexDefects;
    if(contour.size() > 0){
        CvSeq* contourPoints;
        CvSeq* defects;
        CvMemStorage* storage;
        CvMemStorage* strDefects;
        CvMemStorage* contourStr;
        CvConvexityDefect *defectArray = 0;
        double maxArea = 17;

        strDefects = cvCreateMemStorage();
        defects = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvSeq),sizeof(CvPoint), strDefects );

        contourStr = cvCreateMemStorage();
        contourPoints = cvCreateSeq(CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvSeq), sizeof(CvPoint), contourStr);
        for(int i=0; i<(int)contour.size(); i++) {
            CvPoint cp = {contour[i].x,  contour[i].y};
            cvSeqPush(contourPoints, &cp);
        }

        int count = (int)hull.size();
        //int hullK[count];
        int* hullK = (int*)malloc(count*sizeof(int));
        for(int i=0; i<count; i++){hullK[i] = hull.at(i);}
        CvMat hullMat = cvMat(1, count, CV_32SC1, hullK);

        storage = cvCreateMemStorage(0);
        defects = cvConvexityDefects(contourPoints, &hullMat, storage);

        defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect)*defects->total);
        cvCvtSeqToArray(defects, defectArray, CV_WHOLE_SEQ);

        for(int i = 0; i<defects->total; i++){
            CvPoint ptf;
            ptf.x = defectArray[i].depth_point->x;
            ptf.y = defectArray[i].depth_point->y;

            convexDefects.push_back(ptf);
        }

        cvReleaseMemStorage(&contourStr);
        cvReleaseMemStorage(&strDefects);
        cvReleaseMemStorage(&storage);
    }
    return convexDefects;
}

//retourne les points de l'enveloppe convexe
std::vector<Point2i> Processing::getConvexHull(std::vector<Point2i>contour)
{ 
    std::vector<Point2i> hullP(contour.size());
    convexHull(contour, hullP, false);
    return hullP;
}

//retourne les index de l'enveloppe convexe nécessaire pour le calcul des defect points
std::vector<int> Processing::getConvexHullI(std::vector<Point2i>contour)
{
    std::vector<int> hullI(contour.size());
    convexHull(contour, hullI, false,false);
    return hullI;
}

//retourne un vecteur des densités des zones de la main
std::vector<double> Processing::getDensity(Mat rect, int w, int h)
{
    int stepX = (int)rect.rows/w;
    int stepY = (int)rect.cols/h;

    std::vector<double> dens(w*h);

    for(int i = 0; i < w ; i++ )
        for(int j=0; j < h ; j++)
        {
            Mat tmp(stepX,stepY,CV_8UC1);
            for(int k = 0; k < stepX; k++ )
                for(int l=0; l < stepY ; l++)
                    tmp.at<unsigned char>(k,l) = rect.at<unsigned char>(l-i*stepX,k-j*stepY);
            dens.push_back(countNonZero(tmp) / (stepX*stepY));
        }
    return dens;
}
