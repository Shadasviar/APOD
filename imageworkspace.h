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

signals:

public slots:

protected:
    QVBoxLayout _layout;
    std::shared_ptr<QSplitter> _splitter;
    std::shared_ptr<QGraphicsView>_imageView;
    ToolsArea _tools;
    QImage _image;
    QGraphicsScene _scene;
};

#endif // IMAGEWORKSPACE_H
