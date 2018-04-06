/*
 * This file is part of HarbuzHIST18. 
 * itoolwidget.h
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
    void setProgressBar(int progress);
    void hideProgressBar();
    void showStatusMsg(QString text);
};

#endif // ITOOLWIDGET_H
