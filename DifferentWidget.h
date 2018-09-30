#ifndef DIFFERENTWIDGET_H
#define DIFFERENTWIDGET_H

#include <QWidget>

namespace Ui {
class DifferentWidget;
}

class DifferentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DifferentWidget(QWidget *parent = 0);
    ~DifferentWidget();

private:
    Ui::DifferentWidget *ui;
};

#endif // DIFFERENTWIDGET_H
