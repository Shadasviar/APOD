#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsScene>
#include <functional>
#include "imageworkspace.h"

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
    void on_histButton_toggled(bool checked);
    void on_mainTabWidget_currentChanged(int index);
    void on_histStretch_toggled(bool checked);
    void on_previewButton_toggled(bool checked);

    void on_UPOButton_toggled(bool checked);

    void on_equiliseButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QFileInfo lastOpenedDir;

    /* Image view */
    QImage currentImage;
    QGraphicsScene scene;

    struct toolState {
        QToolButton* tool;
        bool state;
    };

    QList<QToolButton*> _toolsList;
    QMap<QWidget*, QList<toolState>> _toolsStateList;
    QWidget* _preTabIndex = nullptr;
    void dumpToolsState(QWidget *index);
    void restoreToolsState(QWidget* index);


};

#endif // MAINWINDOW_H
