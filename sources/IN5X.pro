
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IN5X
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/processing.cpp \
    src/preprocessing.cpp \
    src/histogram1d.cpp \
    src/recognition.cpp \
    src/inwindow.cpp \
    src/image.cpp

OTHER_FILES += \
    res/7.yml \
    res/6.yml \
    res/5.yml \
    res/4.yml \
    res/3.yml \
    res/2.yml \
    res/1.yml

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lopencv_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lopencv_core
else:symbian: LIBS += -lopencv_core
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_core

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lopencv_highgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lopencv_highgui
else:symbian: LIBS += -lopencv_highgui
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_highgui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lopencv_imgproc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lopencv_imgproc
else:symbian: LIBS += -lopencv_imgproc
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_imgproc

HEADERS += \
    src/processing.h \
    src/preprocessing.h \
    src/histogram1d.h \
    src/recognition.h \
    src/inwindow.h \
    image.h \
    src/image.h

FORMS += \
    mainwindow.ui \
    dialogdata.ui

