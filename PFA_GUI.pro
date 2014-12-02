#-------------------------------------------------
#
# Project created by QtCreator 2014-05-16T08:41:09
#
#-------------------------------------------------

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PFA_GUI
TEMPLATE = app



SOURCES += main.cpp\
        mainwindow.cpp \
    Utils.cpp \
    SkinFilter.cpp \
    OpticalFlowManager.cpp \
    NofaceException.cpp \
    HandRecognizer.cpp \
    Engine.cpp \
    Configurator.cpp \
    xmlwriter.cpp

HEADERS  += mainwindow.h \
    Utils.h \
    SkinFilter.h \
    OpticalFlowManager.h \
    NofaceException.h \
    HandRecognizer.h \
    Engine.h \
    Configurator.h \
    xmlwriter.h

FORMS    += mainwindow.ui

INCLUDEPATH +=  /opt/local/include/

LIBS += -L/opt/local/lib -lopencv_core -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_imgproc -lopencv_objdetect
