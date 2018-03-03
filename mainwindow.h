#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsScene>

#include "histogram.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_mainTabWidget_tabCloseRequested(int index);

private:
    Ui::MainWindow *ui;
    QFileInfo lastOpenedDir;

    /* Image view */
    QImage currentImage;
    QGraphicsScene scene;

    void updateHist();
};

#endif // MAINWINDOW_H
