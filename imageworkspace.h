#ifndef IMAGEWORKSPACE_H
#define IMAGEWORKSPACE_H

#include <QWidget>
#include <QGraphicsView>
#include <QSplitter>
#include <QVBoxLayout>
#include <memory>

class ImageWorkspace : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWorkspace(QWidget *parent = nullptr);
    ImageWorkspace(QImage&& image, QWidget* parent = nullptr);
    virtual ~ImageWorkspace();

signals:

public slots:

protected:
    QVBoxLayout _layout;
    std::shared_ptr<QSplitter> _splitter;
    std::shared_ptr<QGraphicsView>_imageView;
    std::shared_ptr<QWidget> _leftWidget;
    QImage _image;
    QGraphicsScene _scene;
};

#endif // IMAGEWORKSPACE_H
