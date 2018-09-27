#include "ExportUntranslated.h"
#include "ui_ExportUntranslated.h"
#include <QtDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

ExportUntranslated::ExportUntranslated(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportUntranslated)
{
    ui->setupUi(this);
}

ExportUntranslated::~ExportUntranslated()
{
    delete ui;
}

void ExportUntranslated::on_pushButton_openSourceFile_clicked()
{
    QString strFile = QFileDialog::getOpenFileName(this, "选择", "./", "翻译文件 (*.txt *.lng)");
    ui->lineEdit_source->setText(strFile);
    m_baseFileList.clear();
    if (strFile.isEmpty())
    {
        return;
    }
    QFile file(strFile);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this, "提示", QString("文件打开失败：%1").arg(file.errorString()));
        return;
    }
    QFileInfo fileInfo(file);
    m_strBaseFileName = fileInfo.fileName();

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString strLine = in.readLine().trimmed();
        if (!strLine.isEmpty())
        {
            m_baseFileList.append(strLine);
        }
    }

    QDir fileDir(strFile);
    fileDir.cdUp();
    m_fileList = fileDir.entryInfoList(QDir::Files);
}

void ExportUntranslated::on_pushButton_openExportDir_clicked()
{
    m_exportDir = QFileDialog::getExistingDirectory(this, "导出位置", "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_exportDir->setText(m_exportDir);
}

void ExportUntranslated::on_pushButton_export_clicked()
{
    if (m_exportDir.isEmpty())
    {
        QMessageBox::information(this, "警告", "未选择导出目录。");
        return;
    }

    ui->progressBar->setMaximum(m_fileList.size());

    ui->pushButton_export->setEnabled(false);
    for (int i = 0; i < m_fileList.size(); ++i)
    {
        const QFileInfo &fileInfo = m_fileList.at(i);

        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            continue;
        }

        QString strFileName = fileInfo.fileName();

        if (strFileName == m_strBaseFileName)
        {
            continue;
        }

        LanguageInfo &lanInfo = m_fileMap[strFileName];
        QString strGroup;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString strLine = in.readLine().trimmed();
            if (strLine.startsWith("[") && strLine.endsWith("]"))
            {
                strGroup = strLine;
            }
            else
            {
                QRegularExpression rx("(^\\d+)=(.*)");
                QRegularExpressionMatch match = rx.match(strLine);
                if (match.hasMatch())
                {
                    QString strKey = match.captured(1);
                    QString strValue = match.captured(2);
                    QString strCompleteKey = QString("%1/%2").arg(strGroup).arg(strKey);
                    lanInfo.keyValueMap.insert(strCompleteKey, strValue);
                }
            }
        }
    }

    //
    for (int i = 0; i < m_fileList.size(); ++i)
    {
        const QFileInfo &fileInfo = m_fileList.at(i);

        QString strFileName = fileInfo.fileName();
        if (strFileName == m_strBaseFileName)
        {
            continue;
        }

        const LanguageInfo &lanInfo = m_fileMap.value(strFileName);

        QString strFilePath = QString("%1/%2").arg(m_exportDir).arg(strFileName);
        QFile file(strFilePath);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            continue;
        }
        QTextStream out(&file);

        bool addGroup = false;
        QString strGroup;
        for (int i = 0; i < m_baseFileList.size(); ++i)
        {
            QString strLine = m_baseFileList.at(i);
            if (strLine.startsWith("[") && strLine.endsWith("]"))
            {
                addGroup = true;
                strGroup = strLine;
            }
            else
            {
                QRegularExpression rx("(^\\d+)=(.*)");
                QRegularExpressionMatch match = rx.match(strLine);
                if (match.hasMatch())
                {
                    QString strKey = match.captured(1);
                    QString strValue = match.captured(2);
                    QString strCompleteKey = QString("%1/%2").arg(strGroup).arg(strKey);

                    bool addLine = false;
                    if (lanInfo.keyValueMap.contains(strCompleteKey))
                    {
                        if (lanInfo.keyValueMap.value(strCompleteKey) == strValue)
                        {
                            addLine = true;
                        }
                    }
                    else
                    {
                        addLine = true;
                    }
                    if (addLine)
                    {
                        if (addGroup)
                        {
                            addGroup = false;
                            out << "\r\n" << strGroup << "\r\n";
                        }
                        out << strLine << "\r\n";
                    }
                }
            }
        }
        file.close();
        //
        ui->progressBar->setValue(i);
        qApp->processEvents();
    }
    ui->pushButton_export->setEnabled(true);
    ui->progressBar->setValue(ui->progressBar->maximum());
    QMessageBox::information(this, "提示", "导出完毕。");
    ui->progressBar->setValue(0);
}
