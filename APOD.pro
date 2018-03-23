#-------------------------------------------------
#
# Project created by QtCreator 2018-02-21T19:30:41
#
#-------------------------------------------------

QT       += core gui
QT += charts
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
    cpp/imageworkspace.cpp \
    cpp/toolsarea.cpp \
    cpp/histogramequalisation.cpp \
    cpp/histogram.cpp \
    cpp/histogramstratching.cpp \
    cpp/universalpointoperation.cpp \
    cpp/binaryimageoperation.cpp \
    cpp/convolutionmatrix.cpp

HEADERS += \
    include/mainwindow.h \
    include/imageworkspace.h \
    include/toolsarea.h \
    include/histogramequalisation.h \
    include/histogram.h \
    include/histogramstratching.h \
    include/itoolwidget.h \
    include/universalpointoperation.h \
    include/binaryimageoperation.h \
    include/convolutionmatrix.h

FORMS += \
    UI/mainwindow.ui \
    UI/histogramequalisation.ui \
    UI/histogram.ui \
    UI/histogramstratching.ui \
    UI/universalpointoperation.ui \
    UI/binaryimageoperation.ui \
    UI/convolutionmatrix.ui

RESOURCES += \
    images.qrc
