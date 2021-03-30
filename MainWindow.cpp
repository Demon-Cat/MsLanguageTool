#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "About.h"
#include "ExportNew.h"
#include "ExportUntranslated.h"
#include "checkerror.h"
#include <QClipboard>
#include <QSettings>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->widget_language1, SIGNAL(sig_result(QString)), this, SLOT(onLanguageResult(QString)));
    connect(ui->widget_language1, SIGNAL(sig_cursorPositionChanged(int, QString)), ui->widget_language2, SLOT(onSetCursorPosition(int, QString)));
    connect(ui->widget_language2, SIGNAL(sig_cursorPositionChanged(int, QString)), ui->widget_language1, SLOT(onSetCursorPosition(int, QString)));

    QSettings setting(QString("%1/setting.ini").arg(qApp->applicationDirPath()), QSettings::IniFormat);
    QString strPath1 = setting.value("FilePath1").toString();
    QString strPath2 = setting.value("FilePath2").toString();
    ui->widget_language1->setIndex(0);
    ui->widget_language1->loadFile(strPath1);
    ui->widget_language2->setIndex(1);
    ui->widget_language2->loadFile(strPath2);

    showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLanguageResult(const QString &str)
{
    ui->lineEdit_result->setText(str);
}

void MainWindow::on_pushButton_copy_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->lineEdit_result->text());
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

void MainWindow::on_actionCheckError_triggered()
{
    CheckError check(this);
    check.exec();
}
