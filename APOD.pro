#-------------------------------------------------
#
# Project created by QtCreator 2018-02-21T19:30:41
#
#-------------------------------------------------

QT       += core gui
QT += charts
CONFIG += c++17 strict_c++

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = APOD
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    imageworkspace.cpp \
    toolsarea.cpp \
    histogramequalisation.cpp \
    histogram.cpp \
    histogramstratching.cpp \
    universalpointoperation.cpp \
    binaryimageoperation.cpp \
    convolutionmatrix.cpp

HEADERS += \
        mainwindow.h \
    imageworkspace.h \
    toolsarea.h \
    histogramequalisation.h \
    histogram.h \
    histogramstratching.h \
    itoolwidget.h \
    universalpointoperation.h \
    binaryimageoperation.h \
    convolutionmatrix.h

FORMS += \
        mainwindow.ui \
    histogramequalisation.ui \
    histogram.ui \
    histogramstratching.ui \
    universalpointoperation.ui \
    binaryimageoperation.ui \
    convolutionmatrix.ui

RESOURCES += \
    images.qrc
