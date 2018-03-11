#ifndef ITOOLWIDGET_H
#define ITOOLWIDGET_H

#include <QWidget>
#include <QImage>
#include <QFrame>

class IToolWidget : public QFrame
{
    Q_OBJECT
public:
    IToolWidget(QWidget* parent = 0):QFrame(parent){}

signals:
    void setPreview(QImage*);
};

#endif // ITOOLWIDGET_H
