/*
 * This file is part of HarbuzHIST18.
 * imageworkspaceview.h
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

#ifndef IMAGEWORKSPACEVIEW_H
#define IMAGEWORKSPACEVIEW_H

#include <QFrame>
#include "scalableimageview.h"
#include "toolsarea.h"
#include "itoolwidget.h"
#include "compare.h"
#include "histogram2d.h"
#include "histogramstratching.h"

#define TWO_IMAGE_REQUIRED_ERROR_STR "You must have two images for\
    this operation (image and preview of it). \
    Do some action on your image and try again."

namespace Ui {
class ImageWorkspaceView;
}

class ImageWorkspaceView : public IToolWidget
{
    Q_OBJECT

public:
    explicit ImageWorkspaceView(QWidget *parent = 0);
    ImageWorkspaceView(QImage&& image, QWidget* parent = nullptr);
    ~ImageWorkspaceView();

    void deleteActiveTool();
    QImage* getPreviewImage(){return _preview.getImage();}

private:
    Ui::ImageWorkspaceView *ui;

    ToolsArea _tools;
    ScalableImageView _image;
    ScalableImageView _preview;

    QSplitter* _splitter;
    QSplitter* _imageSplitter;

public slots:
    void modifyPreview(QImage* img);

signals:
    void signalPreviewChange(QImage*);

public:
    template <typename T>
    void addToolsAreaItem(){
        T* item;
        if(_preview.getImage()) {
            item = new T(_preview.getImage(), this, "Processed image");
            doSpecifiedStuff<T>(item, &_preview);
        } else {
            item = new T(_image.getImage(), this, "Original image");
            doSpecifiedStuff<T>(item, &_image);
        }
        _tools.addInfoTool(item);
    }

    template <typename T>
    void setCurrentOperation(){
        T* item = new T(_image.getImage(), this);
        doSpecifiedStuff<T>(item, &_preview);
        _tools.setTool(item);
    }

    template <typename T>
    void deleteToolsAreaItem(){
        _tools.deleteTool<T>();
    }

    template<typename T> void doSpecifiedStuff(T* x, ScalableImageView* sigSrc){
        // Do common for generic and specialization
        if (std::is_base_of<IToolWidget, T>()) {
            doSpecifiedStuff((IToolWidget*)x, sigSrc);
            return;
        }
        // Do generic only
    }
    void doSpecifiedStuff(IToolWidget* obj, ScalableImageView* sigSrc) {
        connect(obj, &IToolWidget::setPreview, this, &ImageWorkspaceView::modifyPreview, Qt::UniqueConnection);
        connect(obj, &IToolWidget::setProgressBar, this, &ImageWorkspaceView::setProgressBar, Qt::UniqueConnection);
        connect(obj, &IToolWidget::hideProgressBar, this, &ImageWorkspaceView::hideProgressBar, Qt::UniqueConnection);
        connect(obj, &IToolWidget::showStatusMsg, this, &ImageWorkspaceView::showStatusMsg, Qt::UniqueConnection);
        connect(sigSrc, &ScalableImageView::imageChanged, obj, &IToolWidget::sourceChanged, Qt::UniqueConnection);
    }
};

template <>
void inline ImageWorkspaceView::addToolsAreaItem<Histogram2D>(){
    if (!_preview.getImage()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error", TWO_IMAGE_REQUIRED_ERROR_STR);
        messageBox.setFixedSize(500,200);
        return;
    }
    auto* item = new Histogram2D(_image.getImage(), _preview.getImage(), this);
    doSpecifiedStuff(item, &_image);
    doSpecifiedStuff(item, &_preview);
    _tools.addInfoTool(item);
}

template <>
void inline ImageWorkspaceView::addToolsAreaItem<HistogramStratching>(){
    HistogramStratching* item;
    if(_image.getImage()) {
        item = new HistogramStratching(_image.getImage(), this, "Original image");
        doSpecifiedStuff<HistogramStratching>(item, &_image);
    } else item = nullptr;
    _tools.addInfoTool(item);
}

template<>
void inline ImageWorkspaceView::addToolsAreaItem<Compare>() {
    if (!_preview.getImage()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error", TWO_IMAGE_REQUIRED_ERROR_STR);
        messageBox.setFixedSize(500,200);
        return;
    }
    Compare* item = new Compare(_image.getImage(), _preview.getImage(), this);
    doSpecifiedStuff<Compare>(item, &_image);
    doSpecifiedStuff<Compare>(item, &_preview);
    _tools.addInfoTool(item);
}

#endif // IMAGEWORKSPACEVIEW_H
