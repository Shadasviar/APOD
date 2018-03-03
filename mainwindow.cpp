#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <memory>
#include "imageworkspace.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainTabWidget->removeTab(0);
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
                lastOpenedDir.absolutePath()
                );
    lastOpenedDir = QFileInfo(filename);
    QImage img(filename);
    auto *tab = new ImageWorkspace(std::move(img), this);
    ui->mainTabWidget->addTab(tab, filename);

    //updateHist();
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::updateHist()
{
    /*Histogram* hist = new Histogram(currentImage);
    delete ui->imgWorkspaceSplitter->replaceWidget(1, hist->getQChartView());
    ui->left_widget = hist->getQChartView();
    repaint();*/
}

void MainWindow::on_mainTabWidget_tabCloseRequested(int index)
{
    delete ui->mainTabWidget->widget(index);
}
