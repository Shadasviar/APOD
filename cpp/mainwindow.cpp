/*
 * This file is part of HarbuzHIST18. 
 * mainwindow.cpp
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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <memory>
#include "imageworkspace.h"
#include "histogramequalisation.h"
#include "histogramstratching.h"
#include "universalpointoperation.h"
#include "binaryimageoperation.h"
#include "convolutionmatrix.h"
#include "aboutprogramwindow.h"
#include "histogram2d.h"
#include "settings.h"
#include "compare.h"

/*Use macro because forward declaration of ui brokes template using*/
#define addInfoTool(T) currentTab->addToolsAreaItem<T>();

#define setOperationTool(T) currentTab->setCurrentOperation<T>();\
    ui->actionActive_widget_of_operation_on_image->setChecked(true);

#define toolButtonToggled(T, action, checked) \
    auto currentTab = qobject_cast<ImageWorkspace*>(ui->mainTabWidget->currentWidget());\
    if (checked) {\
        if (ui->mainTabWidget->currentWidget()){\
            action(T);\
        }\
    }\
    else {\
        if (ui->mainTabWidget->currentWidget()) {\
            currentTab->deleteToolsAreaItem<T>();\
        }\
    }\


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainTabWidget->removeTab(0);

    _toolsList = QList<QAction*>{
            ui->actionHistogram,
            ui->actionHistogram_2D,
            ui->actionActive_widget_of_operation_on_image,
            ui->actionCompare,
    };

    ui->statusBar->addPermanentWidget(_statusText, 0);
    ui->statusBar->addWidget(_progressBar, 1);
    _progressBar->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                "Open file",
                lastOpenedDir.absolutePath(),
                "ALL(*);;JPEG(*.jpg, *.jpeg);;PNG(*.png);;BMP(*.bmp)"
                );
    lastOpenedDir = QFileInfo(filename);
    QImage img(filename);
    auto *tab = new ImageWorkspace(std::move(img), this);
    connect(tab, &ImageWorkspace::emitProgressBar, this, &MainWindow::setProgressBar);
    connect(tab, &ImageWorkspace::hideProgressBar, this, &MainWindow::hideProgressBar);
    connect(tab, &ImageWorkspace::showStatusMsg, this, &MainWindow::showStatusMsg);

    _preTabIndex = ui->mainTabWidget->currentWidget();
    ui->mainTabWidget->addTab(tab, filename);
    ui->mainTabWidget->setCurrentWidget(tab);
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}


void MainWindow::on_mainTabWidget_tabCloseRequested(int index)
{
    _toolsStateList.remove(ui->mainTabWidget->widget(index));
    delete ui->mainTabWidget->widget(index);
}

void MainWindow::on_mainTabWidget_currentChanged(int index)
{
    dumpToolsState(_preTabIndex);
    _preTabIndex = ui->mainTabWidget->widget(index);
    restoreToolsState(_preTabIndex);
}

void MainWindow::dumpToolsState(QWidget* index)
{
    _toolsStateList.insert(index, QList<toolState>());
    for (auto& item : _toolsList) {
        _toolsStateList[index].append({.tool=item, .state=item->isChecked()});
    }
}

void MainWindow::restoreToolsState(QWidget *index)
{
    if (!_toolsStateList.contains(index)) {
        for (auto& item : _toolsList) {
            item->setChecked(false);
        }
        return;
    }
    for (auto& item: _toolsStateList[index]) {
        item.tool->setChecked(item.state);
    }
    _toolsStateList.remove(index);
}

void MainWindow::on_actionHistogram_toggled(bool checked)
{
    toolButtonToggled(Histogram, addInfoTool, checked);
}

void MainWindow::on_actionHistogram_stratching_triggered()
{
    toolButtonToggled(HistogramStratching, setOperationTool, true);
}

void MainWindow::on_actionHistogram_equalization_triggered()
{
    toolButtonToggled(HistogramEqualisation, setOperationTool, true);
}

void MainWindow::on_actionSingle_Argument_Operation_triggered()
{
    toolButtonToggled(UniversalPointOperation, setOperationTool, true);
}

void MainWindow::on_actionMultiple_arguments_operation_triggered()
{
    toolButtonToggled(BinaryImageOperation, setOperationTool, true);
}

void MainWindow::on_actionText_only_triggered()
{
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
}

void MainWindow::on_actionIcons_only_triggered()
{
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
}

void MainWindow::on_actionText_under_icons_triggered()
{
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void MainWindow::on_actionText_beside_icons_triggered()
{
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void MainWindow::on_actionConvolution_matrix_triggered()
{
    toolButtonToggled(ConvolutionMatrix, setOperationTool, true);
}

void MainWindow::on_actionAbout_program_triggered()
{
    static AboutProgramWindow* info = nullptr;
    if (info == nullptr) {
        info = new AboutProgramWindow(this);
    }
    info->show();
}

void MainWindow::on_actionHistogram_2D_triggered(bool checked)
{
    toolButtonToggled(Histogram2D, addInfoTool, checked);
}

void MainWindow::on_actionActive_widget_of_operation_on_image_triggered(bool checked)
{
    if (!checked){
        auto currentTab = qobject_cast<ImageWorkspace*>(ui->mainTabWidget->currentWidget());
        currentTab->deleteActiveTool();
    }
    else {
        ui->actionActive_widget_of_operation_on_image->setChecked(false);
    }
}

void MainWindow::setProgressBar(int progress)
{
    _progressBar->setVisible(true);
    _progressBar->setValue(progress);
}

void MainWindow::hideProgressBar()
{
    _progressBar->setVisible(false);
}

void MainWindow::showStatusMsg(QString text)
{
    _statusText->setText(text);
}

void MainWindow::on_actionSave_triggered()
{
    QImage* image(nullptr);
    auto* currentWidget = qobject_cast<ImageWorkspace*>(ui->mainTabWidget->currentWidget());

    if (currentWidget)
        image = currentWidget->getPreviewImage();
    if (!image) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","There is no edited image to save");
        messageBox.setFixedSize(500,200);
        return;
    }

    QString savePath = QFileDialog::getSaveFileName(
                this,
                "Save file",
                lastOpenedDir.absolutePath(),
                "JPEG(*.jpg);;PNG(*.png);;BMP(*.bmp);;ALL(*)"
                );

    lastOpenedDir = QFileInfo(savePath);
    if (!savePath.contains("."))
        savePath += ".jpg";
    if (savePath != "")
        image->save(savePath);
}

void MainWindow::on_actionSettings_triggered()
{
    toolButtonToggled(Settings, setOperationTool, true);
}

void MainWindow::on_actionCompare_triggered(bool checked)
{
    toolButtonToggled(Compare, addInfoTool, checked);
}
