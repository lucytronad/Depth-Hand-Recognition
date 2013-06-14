#ifndef RECOGNITION_H
#define RECOGNITION_H

#include "opencv2/opencv.hpp"

using namespace cv;

class Recognition
{

public:
    enum STATE_HAND {HAND_OPEN,HAND_CLOSE};
    enum STATE_FINGERS {HAND_0,HAND_1,HAND_2,HAND_3,HAND_4,HAND_5};

    Recognition();
    static STATE_HAND getHandRecognition(std::vector<Point2i> defects,std::vector<Point2i> hull);
    static STATE_FINGERS getFingerRecognition(std::vector<Point2i> defects);

};

#endif // RECOGNITION_H
