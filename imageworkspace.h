#ifndef IMAGEWORKSPACE_H
#define IMAGEWORKSPACE_H

#include <QWidget>
#include <QGraphicsView>
#include <QSplitter>
#include <QVBoxLayout>
#include <memory>
#include <QMap>
#include "toolsarea.h"

class ImageWorkspace : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWorkspace(QWidget *parent = nullptr);
    ImageWorkspace(QImage&& image, QWidget* parent = nullptr);
    virtual ~ImageWorkspace();

    void addHist();
    void deleteHist();

    void addStretchHist();
    void deleteStretchHist();

    void addPreview();
    void deletePreview();

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
};

#endif // IMAGEWORKSPACE_H
