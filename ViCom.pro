#-------------------------------------------------
#
# Project created by QtCreator 2015-06-27T09:57:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ViCom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    extractors.cpp \
    preproc.cpp \
    classification.cpp \
    io.cpp \
    progressdialog.cpp \
    utils.cpp \
    dialogresults.cpp \
    confusionmatrix.cpp \
    avgaccperclass.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    extractors.h \
    preproc.h \
    classification.h \
    io.h \
    header.h \
    progressdialog.h \
    utils.h \
    dialogresults.h \
    confusionmatrix.h \
    avgaccperclass.h \
    about.h

FORMS    += mainwindow.ui \
    progressdialog.ui \
    dialogresults.ui \
    confusionmatrix.ui \
    avgaccperclass.ui \
    about.ui

# pkg-config opencv --libs (em caso de erro)
# OpenCV libs
QT += gui declarative
INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib
#LIBS += -lopencv_shape
LIBS += -lopencv_stitching
LIBS += -lopencv_objdetect
LIBS += -lopencv_superres
LIBS += -lopencv_videostab
LIBS += -lopencv_calib3d
LIBS += -lopencv_features2d
LIBS += -lopencv_highgui
#LIBS += -lopencv_videoio
#LIBS += -lopencv_imgcodecs
LIBS += -lopencv_video
LIBS += -lopencv_photo
LIBS += -lopencv_ml
LIBS += -lopencv_imgproc
LIBS += -lopencv_flann
LIBS += -lopencv_core
#LIBS += -lopencv_hal

# Enable threading
QMAKE_CXXFLAGS += -std=c++11

# Disable unsigned comparison warning
QMAKE_CXXFLAGS += -Wno-sign-compare

