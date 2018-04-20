#ifndef COMPARE_H
#define COMPARE_H

#include <QFrame>

namespace Ui {
class Compare;
}

class Compare : public QFrame
{
    Q_OBJECT

public:
    explicit Compare(QWidget *parent = 0);
    ~Compare();

private:
    Ui::Compare *ui;
};

#endif // COMPARE_H
