#ifndef TOOLSAREA_H
#define TOOLSAREA_H

#include <QWidget>
#include <QMap>
#include <QGridLayout>
#include <memory>
#include "histogram.h"

class ToolsArea : public QWidget
{
    Q_OBJECT
public:

    enum AllowedTools {
        Histogram,
    };

    explicit ToolsArea(QWidget *parent = nullptr);
    void addTool(QWidget* tool, ToolsArea::AllowedTools type);
    void deleteTool(ToolsArea::AllowedTools type);

signals:

public slots:

private:
    QGridLayout _layout;
    QMap<AllowedTools, QWidget*> _enabledTools;
};

#endif // TOOLSAREA_H
