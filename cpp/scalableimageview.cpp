#include "scalableimageview.h"
#include "ui_scalableimageview.h"
#include "settings.h"

ScalableImageView::ScalableImageView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ScalableImageView)
{
    ui->setupUi(this);
}

ScalableImageView::ScalableImageView(QImage* image, QWidget *parent) :
    ScalableImageView(parent)
{
    _image = image;
    _scene.clear();
    _scene.addPixmap(QPixmap::fromImage(*_image));
    _scene.setSceneRect(_image->rect());
    ui->graphicsView->setScene(&_scene);
}

ScalableImageView &ScalableImageView::operator=(ScalableImageView &&rhs)
{
    qSwap(_image, rhs._image);
    _scene.clear();
    _scene.addPixmap(QPixmap::fromImage(*_image));
    _scene.setSceneRect(_image->rect());
    ui->graphicsView->setScene(&_scene);
    emit on_horizontalSlider_valueChanged(ui->horizontalSlider->value());

    return *this;
}

ScalableImageView::~ScalableImageView()
{
    delete _image;
    delete ui;
}

void ScalableImageView::on_horizontalSlider_valueChanged(int value)
{
    double scale = value / 100.;
    auto img = _image->scaled(_image->width()*scale, _image->height()*scale);
    _scene.clear();
    _scene.addPixmap(QPixmap::fromImage(img));
    _scene.setSceneRect(img.rect());
    ui->graphicsView->setScene(&_scene);
    ui->labelMax->setText(QString("+ (%1%)").arg(value));
}
