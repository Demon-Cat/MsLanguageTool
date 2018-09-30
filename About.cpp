#include "About.h"
#include "ui_About.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->label_bulidTime->setText(QString("编译时间：%1 %2").arg(__TIME__).arg(__DATE__));
}

About::~About()
{
    delete ui;
}

void About::on_pushButton_close_clicked()
{
    close();
}
