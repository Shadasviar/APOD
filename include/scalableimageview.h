/*
 * This file is part of HarbuzHIST18.
 * scalableimageview.h
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

#ifndef SCALABLEIMAGEVIEW_H
#define SCALABLEIMAGEVIEW_H

#include <QFrame>
#include <QGraphicsScene>

namespace Ui {
class ScalableImageView;
}

class ScalableImageView : public QFrame
{
    Q_OBJECT

public:
    ScalableImageView(QWidget* parent = nullptr);
    explicit ScalableImageView(QImage* image, QWidget *parent = 0);

    ScalableImageView(ScalableImageView&& rhs);
    ScalableImageView& operator=(ScalableImageView&& rhs);

    QImage* getImage() {return _image;}
    ~ScalableImageView();

private slots:
    void on_horizontalSlider_valueChanged(int value);

signals:
    void imageChanged(QImage*);

private:
    Ui::ScalableImageView *ui;
    QImage* _image = nullptr;
    QGraphicsScene _scene;
    QGraphicsScene _previewScene;
};

#endif // SCALABLEIMAGEVIEW_H
