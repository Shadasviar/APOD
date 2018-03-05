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
    virtual ~ToolsArea();

signals:

public slots:

private:
    QGridLayout _layout;

    /*strings are typenames from TypeIdentifier*/
    QMap<std::type_index, QWidget*> _enabledTools;

public:
    template <typename T>
    void addTool(T* tool){
        if (!_enabledTools.contains(std::type_index(typeid(T)))) {
            _enabledTools[std::type_index(typeid(T))] = tool;
            _layout.addWidget(tool);
        }
        else {
            delete tool;
        }
    }

    template <typename T>
    void deleteTool(T* = nullptr){
        delete _enabledTools[std::type_index(typeid(T))];
        _enabledTools.remove(std::type_index(typeid(T)));
    }
};

#endif // TOOLSAREA_H
