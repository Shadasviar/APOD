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
