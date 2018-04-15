/*
 * This file is part of HarbuzHIST18. 
 * imageworkspace.h
 * Copyright (C) Uladzislau Harbuz 2018 
 * 
 * HarbuzHIST18 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * HarbuzHIST18 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
#include "histogram2d.h"

class ImageWorkspace : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWorkspace(QWidget *parent = nullptr);
    ImageWorkspace(QImage&& image, QWidget* parent = nullptr);
    void deleteActiveTool();
    virtual ~ImageWorkspace();   
    QImage* getPreviewImage(){return _previewImage;}

protected slots:
    void modifyPreview(QImage* img);

signals:
    void emitProgressBar(int progress);
    void hideProgressBar();
    void showStatusMsg(QString);

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
    QWidget* _parent;

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
        connect(obj, &IToolWidget::setProgressBar, this, &ImageWorkspace::emitProgressBar);
        connect(obj, &IToolWidget::hideProgressBar, this, &ImageWorkspace::hideProgressBar);
        connect(obj, &IToolWidget::showStatusMsg, this, &ImageWorkspace::showStatusMsg);
    }
};

template <>
void inline ImageWorkspace::addToolsAreaItem<Histogram2D>(){
    if (!_previewImage) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","You must have two images for this operation"
                                      " (image and preview of it)."
                                      " Do some action on your image and try again.");
        messageBox.setFixedSize(500,200);
        return;
    }
    auto* item = new Histogram2D(&_image, _previewImage, this);
    doSpecifiedStuff(item);
    _tools.addInfoTool(item);
}

#endif // IMAGEWORKSPACE_H
