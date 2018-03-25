/*
 * This file is part of HarbuzHIST18. 
 * toolsarea.h
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
#ifndef TOOLSAREA_H
#define TOOLSAREA_H

#include <QWidget>
#include <QMap>
#include <QGridLayout>
#include <memory>
#include "histogram.h"
#include <typeindex>

class ToolsArea : public QWidget
{
    Q_OBJECT
public:

    explicit ToolsArea(QWidget *parent = nullptr);
    void setTool(QWidget* tool);
    void deleteTool();
    virtual ~ToolsArea();

signals:

public slots:

private:
    QGridLayout _layout;
    QMap<std::type_index, QWidget*> _enabledInfoTools;
    std::unique_ptr<QWidget> _activeTool;

public:
    template <typename T>
    void addInfoTool(T* tool){
        if (!_enabledInfoTools.contains(std::type_index(typeid(T)))) {
            _enabledInfoTools[std::type_index(typeid(T))] = tool;
            _layout.addWidget(tool);
        }
        else {
            delete tool;
        }
    }

    template <typename T>
    void deleteTool(T* = nullptr){
        delete _enabledInfoTools[std::type_index(typeid(T))];
        _enabledInfoTools.remove(std::type_index(typeid(T)));
    }
};

#endif // TOOLSAREA_H
