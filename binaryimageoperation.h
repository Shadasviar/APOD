#ifndef BINARYIMAGEOPERATION_H
#define BINARYIMAGEOPERATION_H

#include <QFrame>
#include "itoolwidget.h"
#include <QFileInfo>
#include <QToolButton>
#include <QList>
#include <QGraphicsView>
#include <memory>
#include <list>
#include <QMap>
#include <functional>

namespace Ui {
class BinaryImageOperation;
}

class BinaryImageOperation : public IToolWidget
{
    Q_OBJECT

public:
    explicit BinaryImageOperation(QImage *img, QWidget *parent = 0);
    ~BinaryImageOperation();
    static QImage* reduceImages(const std::list<std::unique_ptr<QImage>>& images,
                                std::function<uint8_t(const uint8_t, const uint8_t)> op,
                                QImage *startImg);

private slots:

    void on_addImageButton_clicked();

    void on_applyButton_clicked();

private:
    Ui::BinaryImageOperation *ui;
    QList<QGraphicsView*> _views;
    std::list<std::unique_ptr<QImage>> _images;
    QFileInfo _lastOpenedDir;
    QImage* _image;

    static QMap<QString, std::function<uint8_t(const uint8_t, const uint8_t)>> _operations;

    void addImage();
};

#endif // BINARYIMAGEOPERATION_H
