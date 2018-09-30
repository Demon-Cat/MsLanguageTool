#include "DeveloperMode.h"
#include "ui_DeveloperMode.h"

DeveloperMode::DeveloperMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeveloperMode)
{
    ui->setupUi(this);
}

DeveloperMode::~DeveloperMode()
{
    delete ui;
}
