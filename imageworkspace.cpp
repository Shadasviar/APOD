#include "imageworkspace.h"

ImageWorkspace::ImageWorkspace(QWidget *parent) : QWidget(parent),
    _layout(this),
    _splitter(std::make_shared<QSplitter>(this)),
    _imageView(std::make_shared<QGraphicsView>(_splitter.get())),
    _tools(this)
{
    _layout.addWidget(_splitter.get());
    _splitter->addWidget(_imageView.get());
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

void ImageWorkspace::addHist()
{
    Histogram *hist = new Histogram(_image);
    _tools.addTool(hist, ToolsArea::Histogram);
    repaint();
}

void ImageWorkspace::deleteHist()
{
    _tools.deleteTool(ToolsArea::Histogram);
    repaint();
}
