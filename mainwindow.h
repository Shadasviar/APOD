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
    void on_mainTabWidget_currentChanged(int index);

    void on_actionHistogram_toggled(bool checked);
    void on_actionHistogram_stratching_triggered();
    void on_actionHistogram_equalization_triggered();
    void on_actionSingle_Argument_Operation_triggered();
    void on_actionMultiple_arguments_operation_triggered();

    void on_actionText_only_triggered();

    void on_actionIcons_only_triggered();

    void on_actionText_under_icons_triggered();

    void on_actionText_beside_icons_triggered();

private:
    Ui::MainWindow *ui;
    QFileInfo lastOpenedDir;

    /* Image view */
    QImage currentImage;
    QGraphicsScene scene;

    struct toolState {
        QAction* tool;
        bool state;
    };

    QList<QAction*> _toolsList;
    QMap<QWidget*, QList<toolState>> _toolsStateList;
    QWidget* _preTabIndex = nullptr;
    void dumpToolsState(QWidget *index);
    void restoreToolsState(QWidget* index);


};

#endif // MAINWINDOW_H
