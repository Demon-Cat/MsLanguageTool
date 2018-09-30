#ifndef DEVELOPERMODE_H
#define DEVELOPERMODE_H

#include <QWidget>

namespace Ui {
class DeveloperMode;
}

class DeveloperMode : public QWidget
{
    Q_OBJECT

public:
    explicit DeveloperMode(QWidget *parent = 0);
    ~DeveloperMode();

private:
    Ui::DeveloperMode *ui;
};

#endif // DEVELOPERMODE_H
