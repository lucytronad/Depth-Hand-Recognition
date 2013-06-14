#include "dialogdata.h"
#include "ui_dialogdata.h"

DialogData::DialogData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogData)
{
    ui->setupUi(this);
}

DialogData::~DialogData()
{
    delete ui;
}
