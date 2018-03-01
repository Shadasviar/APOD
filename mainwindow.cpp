#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <memory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    currentImage.load(filename);
    scene.clear();
    scene.addPixmap(QPixmap::fromImage(currentImage));
    scene.setSceneRect(currentImage.rect());
    ui->graphicsView->setScene(&scene);

    updateHist();
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::updateHist()
{
    Histogram* hist = new Histogram(currentImage);
    delete ui->imgWorkspaceSplitter->replaceWidget(1, hist->getQChartView());
    ui->left_widget = hist->getQChartView();
    repaint();
}
