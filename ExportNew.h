#ifndef EXPORTNEW_H
#define EXPORTNEW_H

#include <QDialog>
#include <ExportUntranslated.h>

namespace Ui {
class ExportNew;
}

class ExportNew : public QDialog
{
    Q_OBJECT

public:
    explicit ExportNew(QWidget *parent = 0);
    ~ExportNew();

private slots:
    void on_pushButton_openSourceFile_clicked();
    void on_pushButton_openOldFile_clicked();
    void on_pushButton_openExportDir_clicked();
    void on_pushButton_export_clicked();

private:
    Ui::ExportNew *ui;

    QList<QString> m_baseFileList;
    QString m_exportDir;

    QString m_strBaseFileName;  //校对基准文件，默认为英文文件。
    QString m_strOldFileName;
};

#endif // EXPORTNEW_H
