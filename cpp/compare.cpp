#include "compare.h"
#include "ui_compare.h"

Compare::Compare(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Compare)
{
    ui->setupUi(this);
}

Compare::~Compare()
{
    delete ui;
}
