/********************************************************************************
** Form generated from reading UI file 'dialogdata.ui'
**
** Created: Mon Jan 7 14:36:37 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGDATA_H
#define UI_DIALOGDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogData
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QGridLayout *image_layout;
    QVBoxLayout *verticalLayout;
    QGroupBox *hand_state;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *hand_open;
    QRadioButton *hand_close;
    QGroupBox *finger_state;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *finger_0;
    QRadioButton *finger_1;
    QRadioButton *finger_2;
    QRadioButton *finger_3;
    QRadioButton *finger_5;
    QRadioButton *finger_4;
    QPushButton *valid_button;

    void setupUi(QDialog *DialogData)
    {
        if (DialogData->objectName().isEmpty())
            DialogData->setObjectName(QString::fromUtf8("DialogData"));
        DialogData->resize(400, 300);
        gridLayout_2 = new QGridLayout(DialogData);
        gridLayout_2->setContentsMargins(5, 5, 5, 5);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        image_layout = new QGridLayout();
        image_layout->setObjectName(QString::fromUtf8("image_layout"));

        gridLayout->addLayout(image_layout, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        hand_state = new QGroupBox(DialogData);
        hand_state->setObjectName(QString::fromUtf8("hand_state"));
        verticalLayout_3 = new QVBoxLayout(hand_state);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        hand_open = new QRadioButton(hand_state);
        hand_open->setObjectName(QString::fromUtf8("hand_open"));

        verticalLayout_3->addWidget(hand_open);

        hand_close = new QRadioButton(hand_state);
        hand_close->setObjectName(QString::fromUtf8("hand_close"));

        verticalLayout_3->addWidget(hand_close);


        verticalLayout->addWidget(hand_state);

        finger_state = new QGroupBox(DialogData);
        finger_state->setObjectName(QString::fromUtf8("finger_state"));
        verticalLayout_2 = new QVBoxLayout(finger_state);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        finger_0 = new QRadioButton(finger_state);
        finger_0->setObjectName(QString::fromUtf8("finger_0"));

        verticalLayout_2->addWidget(finger_0);

        finger_1 = new QRadioButton(finger_state);
        finger_1->setObjectName(QString::fromUtf8("finger_1"));

        verticalLayout_2->addWidget(finger_1);

        finger_2 = new QRadioButton(finger_state);
        finger_2->setObjectName(QString::fromUtf8("finger_2"));

        verticalLayout_2->addWidget(finger_2);

        finger_3 = new QRadioButton(finger_state);
        finger_3->setObjectName(QString::fromUtf8("finger_3"));

        verticalLayout_2->addWidget(finger_3);

        finger_5 = new QRadioButton(finger_state);
        finger_5->setObjectName(QString::fromUtf8("finger_5"));

        verticalLayout_2->addWidget(finger_5);

        finger_4 = new QRadioButton(finger_state);
        finger_4->setObjectName(QString::fromUtf8("finger_4"));

        verticalLayout_2->addWidget(finger_4);


        verticalLayout->addWidget(finger_state);


        gridLayout_2->addLayout(verticalLayout, 0, 1, 1, 1);

        valid_button = new QPushButton(DialogData);
        valid_button->setObjectName(QString::fromUtf8("valid_button"));

        gridLayout_2->addWidget(valid_button, 1, 1, 1, 1);


        retranslateUi(DialogData);

        QMetaObject::connectSlotsByName(DialogData);
    } // setupUi

    void retranslateUi(QDialog *DialogData)
    {
        DialogData->setWindowTitle(QApplication::translate("DialogData", "Dialog", 0, QApplication::UnicodeUTF8));
        hand_state->setTitle(QApplication::translate("DialogData", "Hand State", 0, QApplication::UnicodeUTF8));
        hand_open->setText(QApplication::translate("DialogData", "Opened", 0, QApplication::UnicodeUTF8));
        hand_close->setText(QApplication::translate("DialogData", "Closed", 0, QApplication::UnicodeUTF8));
        finger_state->setTitle(QApplication::translate("DialogData", "Number of Fingers", 0, QApplication::UnicodeUTF8));
        finger_0->setText(QApplication::translate("DialogData", "0", 0, QApplication::UnicodeUTF8));
        finger_1->setText(QApplication::translate("DialogData", "1", 0, QApplication::UnicodeUTF8));
        finger_2->setText(QApplication::translate("DialogData", "2", 0, QApplication::UnicodeUTF8));
        finger_3->setText(QApplication::translate("DialogData", "3", 0, QApplication::UnicodeUTF8));
        finger_5->setText(QApplication::translate("DialogData", "4", 0, QApplication::UnicodeUTF8));
        finger_4->setText(QApplication::translate("DialogData", "5", 0, QApplication::UnicodeUTF8));
        valid_button->setText(QApplication::translate("DialogData", "Valider", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogData: public Ui_DialogData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGDATA_H
