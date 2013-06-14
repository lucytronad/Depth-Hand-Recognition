#ifndef DIALOGDATA_H
#define DIALOGDATA_H

#include <QDialog>

namespace Ui {
class DialogData;
}

class DialogData : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogData(QWidget *parent = 0);
    ~DialogData();
    
private:
    Ui::DialogData *ui;
};

#endif // DIALOGDATA_H
