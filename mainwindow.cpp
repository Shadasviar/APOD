#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <memory>
#include "imageworkspace.h"
#include "histogramequalisation.h"
#include "histogramstratching.h"
#include "universalpointoperation.h"
#include "binaryimageoperation.h"

/*Use macro because forward declaration of ui brokes template using*/
#define addInfoTool addToolsAreaItem
#define setOperationTool setCurrentOperation

#define toolButtonToggled(T, action, checked) \
    auto currentTab = qobject_cast<ImageWorkspace*>(ui->mainTabWidget->currentWidget());\
    if (checked) {\
        if (ui->mainTabWidget->currentWidget()){\
            currentTab->action<T>();\
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
            /*ui->equiliseButton,
            ui->histStretch,
            ui->UPOButton,
            ui->binaryOpButton,*/
    };
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
