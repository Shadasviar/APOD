#ifndef IMAGEWORKSPACE_H
#define IMAGEWORKSPACE_H

#include <QWidget>
#include <QGraphicsView>
#include <QSplitter>
#include <QVBoxLayout>
#include <memory>
#include <QMap>
#include "toolsarea.h"
//#include "histstretchwidget.h"
#include "histogramequalisation.h"

class Preview {

};

class ImageWorkspace : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWorkspace(QWidget *parent = nullptr);
    ImageWorkspace(QImage&& image, QWidget* parent = nullptr);
    virtual ~ImageWorkspace();   

protected slots:
    void modifyPreview(QImage* img);

protected:
    QVBoxLayout _layout;
    QFrame _imagesViews;
    QVBoxLayout _imagesLayout;
    QSplitter* _splitter;
    std::unique_ptr<QGraphicsView>_imageView;
    std::unique_ptr<QGraphicsView> _preview;

    ToolsArea _tools;
    QImage _image;
    QImage* _previewImage = nullptr;
    QGraphicsScene _scene;
    QGraphicsScene _previewScene;

public:
    template <typename T>
    void addToolsAreaItem(){
        T* item = new T(&_image, this);
        doSpecifiedStaff<T>(item);
        _tools.addTool(item);
    }

    template <typename T>
    void deleteToolsAreaItem(){
        _tools.deleteTool<T>();
    }

    template<typename T> void doSpecifiedStaff(T*){}
};

template<>
inline void ImageWorkspace::doSpecifiedStaff<HistogramEqualisation>(HistogramEqualisation* hist){
    connect(hist, &HistogramEqualisation::setPreview, this, &ImageWorkspace::modifyPreview);
}

template <>
inline void ImageWorkspace::addToolsAreaItem<Preview>(){
    _preview = std::make_unique<QGraphicsView>(_splitter);
    _imagesLayout.addWidget(_preview.get());
}

template <>
inline void ImageWorkspace::deleteToolsAreaItem<Preview>(){
    _preview.reset();
    delete _previewImage;
    _previewImage = nullptr;
    _imagesLayout.removeWidget(_preview.get());
}

#endif // IMAGEWORKSPACE_H
