#ifndef EXPORTUNTRANSLATED_H
#define EXPORTUNTRANSLATED_H

#include <QDialog>
#include <QMap>
#include <QDir>

namespace Ui {
class ExportUntranslated;
}

struct LanguageInfo
{
    QString fileName;
    QMap<QString, QString> keyValueMap;
};

class ExportUntranslated : public QDialog
{
    Q_OBJECT

public:
    explicit ExportUntranslated(QWidget *parent = 0);
    ~ExportUntranslated();

private slots:
    void on_pushButton_openSourceFile_clicked();

    void on_pushButton_openExportDir_clicked();

    void on_pushButton_export_clicked();

private:
    Ui::ExportUntranslated *ui;

    QList<QString> m_baseFileList;
    QMap<QString, LanguageInfo> m_fileMap;  //key: FileName
    QFileInfoList m_fileList;
    QString m_exportDir;

    QString m_strBaseFileName;  //校对基准文件，默认为英文文件。
};

#endif // EXPORTUNTRANSLATED_H
