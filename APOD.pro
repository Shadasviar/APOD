#-------------------------------------------------
#
# Project created by QtCreator 2018-02-21T19:30:41
#
#-------------------------------------------------

QT       += core gui
QT += charts
QT += datavisualization
CONFIG += c++14 strict_c++

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = APOD
TEMPLATE = app
INCLUDEPATH += include

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
    cpp/main.cpp \
    cpp/mainwindow.cpp \
    cpp/toolsarea.cpp \
    cpp/histogramequalisation.cpp \
    cpp/histogram.cpp \
    cpp/histogramstratching.cpp \
    cpp/universalpointoperation.cpp \
    cpp/binaryimageoperation.cpp \
    cpp/convolutionmatrix.cpp \
    cpp/aboutprogramwindow.cpp \
    cpp/histogram2d.cpp \
    cpp/settings.cpp \
    cpp/compare.cpp \
    cpp/scalableimageview.cpp \
    cpp/imageworkspaceview.cpp

HEADERS += \
    include/mainwindow.h \
    include/toolsarea.h \
    include/histogramequalisation.h \
    include/histogram.h \
    include/histogramstratching.h \
    include/itoolwidget.h \
    include/universalpointoperation.h \
    include/binaryimageoperation.h \
    include/convolutionmatrix.h \
    include/aboutprogramwindow.h \
    include/histogram2d.h \
    include/settings.h \
    include/compare.h \
    include/scalableimageview.h \
    include/imageworkspaceview.h

FORMS += \
    UI/mainwindow.ui \
    UI/histogramequalisation.ui \
    UI/histogram.ui \
    UI/histogramstratching.ui \
    UI/universalpointoperation.ui \
    UI/binaryimageoperation.ui \
    UI/convolutionmatrix.ui \
    UI/aboutprogramwindow.ui \
    UI/histogram2d.ui \
    UI/settings.ui \
    UI/compare.ui \
    UI/scalableimageview.ui \
    UI/imageworkspaceview.ui

RESOURCES += \
    images.qrc
