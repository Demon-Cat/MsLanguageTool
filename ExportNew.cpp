#include "ExportNew.h"
#include "ui_ExportNew.h"
#include <QtDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

ExportNew::ExportNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportNew)
{
    ui->setupUi(this);
}

ExportNew::~ExportNew()
{
    delete ui;
}

void ExportNew::on_pushButton_openSourceFile_clicked()
{
    QString strFile = QFileDialog::getOpenFileName(this, "选择旧文件", "./", "翻译文件 (*.txt *.lng)");
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
}

void ExportNew::on_pushButton_openOldFile_clicked()
{
    QString strFile = QFileDialog::getOpenFileName(this, "选择新文件", "./", "翻译文件 (*.txt *.lng)");
    ui->lineEdit_old->setText(strFile);
    if (strFile.isEmpty())
    {
        return;
    }

    QFileInfo fileInfo(strFile);
    m_strOldFileName = fileInfo.fileName();
}

void ExportNew::on_pushButton_openExportDir_clicked()
{
    m_exportDir = QFileDialog::getExistingDirectory(this, "导出位置", "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_exportDir->setText(m_exportDir);
}

void ExportNew::on_pushButton_export_clicked()
{
    if (m_exportDir.isEmpty())
    {
        QMessageBox::information(this, "警告", "未选择导出目录。");
        return;
    }

    ui->pushButton_export->setEnabled(false);

    QFile file(ui->lineEdit_old->text());
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "警告", QString("文件打开失败：%1").arg(file.errorString()));
        return;
    }

    LanguageInfo lanInfo;
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
    file.close();

    //
    QString strFilePath = QString("%1/new-%2").arg(m_exportDir).arg(m_strBaseFileName);
    QFile newFile(strFilePath);
    if (!newFile.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "警告", QString("文件打开失败：%1").arg(newFile.errorString()));
        return;
        return;
    }
    QTextStream out(&newFile);

    bool addGroup = false;
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
                    if (lanInfo.keyValueMap.value(strCompleteKey) != strValue)
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
    newFile.close();
    //
    ui->pushButton_export->setEnabled(true);
    QMessageBox::information(this, "提示", "导出完毕。");
}
