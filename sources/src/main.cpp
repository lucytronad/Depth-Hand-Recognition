/*
 * main.cpp
 *
 *  Created on: 8 nov. 2012
 *      Author: IN5X-team
 */

#include "inwindow.h"
#include <QApplication>

int main(int argc, char * argv[])
{
    QApplication * app = new QApplication(argc,argv);
    INWindow * w = new INWindow();
    w->show();
    return app->exec();
}
