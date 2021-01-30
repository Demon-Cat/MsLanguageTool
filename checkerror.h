#ifndef CHECKERROR_H
#define CHECKERROR_H

#include <QDialog>

namespace Ui {
class CheckError;
}

class CheckError : public QDialog
{
    Q_OBJECT

public:
    explicit CheckError(QWidget *parent = 0);
    ~CheckError();

private slots:
    void on_pushButtonOpen_clicked();

    void on_pushButtonCheck_clicked();

private:
    Ui::CheckError *ui;
};

#endif // CHECKERROR_H
