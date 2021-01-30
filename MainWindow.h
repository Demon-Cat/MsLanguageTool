#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

private slots:
    void onLanguageResult(const QString &str);
    void on_pushButton_copy_clicked();

    void on_action_exportUntranslated_triggered();
    void on_action_exportNew_triggered();

    void on_action_about_triggered();

    void on_actionCheckError_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
