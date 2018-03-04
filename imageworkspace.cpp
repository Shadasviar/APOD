#include "imageworkspace.h"
#include "histstretchwidget.h"

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

void ImageWorkspace::addHist()
{
    Histogram *hist = new Histogram(_image, this);
    _tools.addTool(hist, ToolsArea::Histogram);
    repaint();
}

void ImageWorkspace::deleteHist()
{
    _tools.deleteTool(ToolsArea::Histogram);
}

void ImageWorkspace::addStretchHist()
{
    HistStretchWidget *histStr = new HistStretchWidget(_image, this);
    connect(histStr, &HistStretchWidget::setPreview, this, &ImageWorkspace::modifyPreview);
    _tools.addTool(histStr, ToolsArea::StretchHist);
}

void ImageWorkspace::deleteStretchHist()
{
    _tools.deleteTool(ToolsArea::StretchHist);
}

void ImageWorkspace::addPreview()
{
    _preview = std::make_unique<QGraphicsView>(_splitter);
    _imagesLayout.addWidget(_preview.get());
}

void ImageWorkspace::deletePreview()
{
    _preview.reset();
    delete _previewImage;
    _previewImage = nullptr;
    _imagesLayout.removeWidget(_preview.get());
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
