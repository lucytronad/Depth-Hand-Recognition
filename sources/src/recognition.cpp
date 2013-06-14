#include "recognition.h"

Recognition::Recognition()
{
}

//retourne l'état correspondant au nombre de doigts
Recognition::STATE_FINGERS Recognition::getFingerRecognition(std::vector<Point2i> defects)
{
    int nb = defects.size()-1;
    if(nb < 0)
        nb = 0;
    switch(nb)
    {
    case 0:
        return HAND_0;
        break;
    case 1:
        return HAND_1;
        break;
    case 2:
        return HAND_2;
        break;
    case 3:
        return HAND_3;
        break;
    case 4:
        return HAND_4;
        break;
    case 5:
        return HAND_5;
        break;
    default:
        break;
    }
}

//calcule le nombre de doigts reconnus
Recognition::STATE_HAND Recognition::getHandRecognition(std::vector<Point2i> defects,std::vector<Point2i> hull)
{
    STATE_HAND state;
    if(!defects.empty() && !hull.empty())
    {
        state = HAND_OPEN;
        double dist = 800;
        double delta =contourArea(hull)-contourArea(defects);
        if(delta < dist)
            state = HAND_CLOSE;
    }
    return state;
}


