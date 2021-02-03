#include "checkerror.h"
#include "ui_checkerror.h"
#include <QFileDialog>
#include <QSettings>
#include <QDir>
#include <QTextStream>
#include <QRegularExpression>
#include <QtDebug>

CheckError::CheckError(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckError)
{
    ui->setupUi(this);

    QSettings setting(QString("%1/setting.ini").arg(qApp->applicationDirPath()), QSettings::IniFormat);
    QString strPath1 = setting.value("FilePath1").toString();
    ui->lineEditDir->setText(QFileInfo(strPath1).absoluteDir().absolutePath());
}

CheckError::~CheckError()
{
    delete ui;
}

void CheckError::on_pushButtonOpen_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "选择目录", ui->lineEditDir->text());
    if (!dir.isEmpty())
    {
        ui->lineEditDir->setText(dir);
    }
}

void CheckError::on_pushButtonCheck_clicked()
{
    ui->textEdit->clear();

    QMap<QString, QString> mapIdName;

    QDir dir(ui->lineEditDir->text());
    QFileInfoList infoList = dir.entryInfoList();
    for (int i = 0; i < infoList.size(); ++i)
    {
        const QFileInfo &info = infoList.at(i);
        if (info.fileName().endsWith(".lng"))
        {
            QMap<QString, QString> mapKeyValue;
            QString strGroup;
            QFile file(info.absoluteFilePath());
            if (!file.open(QFile::ReadOnly))
            {
                ui->textEdit->append(QString("打开文件失败, %1, %2").arg(info.absoluteFilePath()).arg(file.errorString()));
                continue;
            }
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
                        if (strKey == QString("LangID"))
                        {
                            if (mapIdName.contains(strValue))
                            {
                                //重复id
                                ui->textEdit->append(QString("重复id： %1， %2").arg(info.fileName(), mapIdName.value(strValue)));
                            }
                            else
                            {
                                mapIdName.insert(strValue, info.fileName());
                            }
                        }
                        if (mapKeyValue.contains(strKey))
                        {
                            //重复key
                            ui->textEdit->append(QString("%1, 重复key：%2").arg(info.fileName()).arg(strKey));
                        }
                        else
                        {
                            mapKeyValue.insert(strKey, strValue);
                        }
                    }
                }
            }
        }
    }
    ui->textEdit->append("检查结束");
}
