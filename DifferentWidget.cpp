#include "DifferentWidget.h"
#include "ui_DifferentWidget.h"

DifferentWidget::DifferentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DifferentWidget)
{
    ui->setupUi(this);
}

DifferentWidget::~DifferentWidget()
{
    delete ui;
}
