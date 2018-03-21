#ifndef IMAGEWORKSPACE_H
#define IMAGEWORKSPACE_H

#include <QWidget>
#include <QGraphicsView>
#include <QSplitter>
#include <QVBoxLayout>
#include <memory>
#include <QMap>
#include "toolsarea.h"
#include "histogramstratching.h"
#include "histogramequalisation.h"
#include "itoolwidget.h"

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
    QSplitter* _imageSplitter;
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
        T* item = new T(_previewImage ? _previewImage : &_image, this);
        doSpecifiedStuff<T>(item);
        _tools.addInfoTool(item);
    }

    template <typename T>
    void setCurrentOperation(){
        T* item = new T(&_image, this);
        doSpecifiedStuff<T>(item);
        _tools.setTool(item);
    }

    template <typename T>
    void deleteToolsAreaItem(){
        _tools.deleteTool<T>();
    }

    template<typename T> void doSpecifiedStuff(T* x){
        // Do common for generic and specialization
        if (std::is_base_of<IToolWidget, T>()) {
            doSpecifiedStuff((IToolWidget*)x);
            return;
        }
        // Do generic only
    }
    void doSpecifiedStuff(IToolWidget* obj) {
        connect(obj, &IToolWidget::setPreview, this, &ImageWorkspace::modifyPreview);
    }
};

#endif // IMAGEWORKSPACE_H
