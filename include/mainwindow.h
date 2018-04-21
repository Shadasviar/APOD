/*
 * This file is part of HarbuzHIST18. 
 * mainwindow.h
 * Copyright (C) Uladzislau Harbuz 2018 
 * 
 * HarbuzHIST18 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * HarbuzHIST18 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QGraphicsScene>
#include <functional>
#include <QFileInfo>
#include <QWidget>
#include <QAction>
#include <QProgressBar>
#include <QLabel>

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
    void on_actionConvolution_matrix_triggered();
    void on_actionAbout_program_triggered();
    void on_actionHistogram_2D_triggered(bool checked);
    void on_actionActive_widget_of_operation_on_image_triggered(bool checked);
    void setProgressBar(int progress);
    void hideProgressBar();
    void showStatusMsg(QString text);
    void on_actionSave_triggered();
    void on_actionSettings_triggered();
    void on_actionCompare_triggered(bool checked);

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

    QProgressBar* _progressBar = new QProgressBar;
    QLabel* _statusText = new QLabel;

};

#endif // MAINWINDOW_H
