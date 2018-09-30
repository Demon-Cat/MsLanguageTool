#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QtDebug>

#include "ExportUntranslated.h"
#include "ExportNew.h"
#include "About.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_exportUntranslated_triggered()
{
    ExportUntranslated exportDialog(this);
    exportDialog.exec();
}

void MainWindow::on_action_exportNew_triggered()
{
    ExportNew exportDialog(this);
    exportDialog.exec();
}

void MainWindow::on_action_about_triggered()
{
    About about(this);
    about.exec();
}
