#include "imageworkspace.h"

ImageWorkspace::ImageWorkspace(QWidget *parent) : QWidget(parent),
    _layout(this),
    _imagesViews(this),
    _imagesLayout(&_imagesViews),
    _splitter(new QSplitter(this)),
    _imageView(std::make_unique<QGraphicsView>(_splitter)),
    _tools(this)
{
    _imagesLayout.addWidget(_imageView.get());
    _layout.addWidget(_splitter);
    _splitter->addWidget(&_imagesViews);
    _splitter->addWidget(&_tools);
}

ImageWorkspace::ImageWorkspace(QImage &&image, QWidget *parent): ImageWorkspace(parent)
{
    _image = QImage(image);
    _scene.clear();
    _scene.addPixmap(QPixmap::fromImage(_image));
    _scene.setSceneRect(_image.rect());
    _imageView->setScene(&_scene);
}

ImageWorkspace::~ImageWorkspace()
{
    _scene.clear();
}

void ImageWorkspace::modifyPreview(QImage *img)
{
    if (_preview) {
        delete _previewImage;
        _previewImage = img;
        _previewScene.clear();
        _previewScene.addPixmap(QPixmap::fromImage(*img));
        _previewScene.setSceneRect(img->rect());
        _preview->setScene(&_previewScene);
    }
}
