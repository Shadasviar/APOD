#include "imageworkspace.h"

ImageWorkspace::ImageWorkspace(QWidget *parent) : QWidget(parent),
    _imageView(std::make_shared<QGraphicsView>(this)),
    _splitter(std::make_shared<QSplitter>(this)),
    _leftWidget(std::make_shared<QWidget>(this))
{
    _splitter->addWidget(_imageView.get());
    _splitter->addWidget(_leftWidget.get());
}

ImageWorkspace::ImageWorkspace(QImage &&image, QWidget *parent): ImageWorkspace()
{
    _image = QImage(image);
    _scene.clear();
    _scene.addPixmap(QPixmap::fromImage(_image));
    _scene.setSceneRect(_image.rect());
    _imageView->setScene(&_scene);
}
