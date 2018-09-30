#include "DeveloperMode.h"
#include "ui_DeveloperMode.h"
#include <QtDebug>
#include <QMessageBox>
#include <QClipboard>
#include <QSettings>
#include <QFileDialog>
#include <QTextStream>
#include <QTextBlock>
#include <QRegularExpression>

//[xxx]
const QString rxGroup("\\[.+\\]");
//123=xxx
const QString rxLanguage("(\\d+)=(.+)");

DeveloperMode::DeveloperMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeveloperMode)
{
    ui->setupUi(this);

    QSettings setting("./setting.ini", QSettings::IniFormat);
    QString strPath1 = setting.value("FilePath1").toString();
    QString strPath2 = setting.value("FilePath2").toString();
    ui->lineEdit_filePath1->setText(strPath1);
    ui->lineEdit_filePath2->setText(strPath2);
}

DeveloperMode::~DeveloperMode()
{
    delete ui;
}

void DeveloperMode::insertBlock(QPlainTextEdit *edit, int blockNumber, const QString &text, const QTextBlockFormat &blockFormat)
{
    QTextBlock block = edit->document()->findBlockByNumber(blockNumber);
    QTextCursor cursor(block);
    cursor.insertText(QString("%1\n").arg(text));
    cursor.movePosition(QTextCursor::PreviousBlock);
    cursor.beginEditBlock();
    cursor.setBlockFormat(blockFormat);
    cursor.endEditBlock();
}

void DeveloperMode::setBlockFormat(QPlainTextEdit *edit, int blockNumber, const QTextBlockFormat &blockFormat)
{
    QTextBlock block = edit->document()->findBlockByNumber(blockNumber);
    QTextCursor cursor(block);
    cursor.beginEditBlock();
    cursor.setBlockFormat(blockFormat);
    cursor.endEditBlock();
}

BlockInfo DeveloperMode::makeBlockInfo(const QTextBlock &block)
{
    BlockInfo blockInfo;
    blockInfo.strBlock = block.text();

    QString strText = block.text().trimmed();
    if (strText.isEmpty())
    {
        blockInfo.type = BlockEmpty;
    }
    if (blockInfo.type == BlockUnknow)
    {
        QRegularExpression rx(rxGroup);
        QRegularExpressionMatch match = rx.match(strText);
        if (match.hasMatch())
        {
            blockInfo.type = BlockGroup;
            blockInfo.strGroup = match.captured(0);
        }
    }
    if (blockInfo.type == BlockUnknow)
    {
        QRegularExpression rx(rxLanguage);
        QRegularExpressionMatch match = rx.match(strText);
        if (match.hasMatch())
        {
            blockInfo.type = BlockLanguage;
            blockInfo.strKey = match.captured(1);
            blockInfo.strValue = match.captured(2);
        }
    }
    if (blockInfo.type == BlockUnknow)
    {
        if (strText.contains("Language=") || strText.contains("LangID="))
        {
            QRegularExpression rx("(.+)=(.+)");
            QRegularExpressionMatch match = rx.match(strText);
            if (match.hasMatch())
            {
                blockInfo.type = BlockHeader;
                blockInfo.strKey = match.captured(1);
                blockInfo.strValue = match.captured(2);
            }
        }
    }

    return blockInfo;
}

void DeveloperMode::compareEdit(QPlainTextEdit *edit1, QPlainTextEdit *edit2)
{
    QTextBlockFormat blockFormat;
    blockFormat.setBackground(QColor(Qt::red).lighter());

    setBlockFormat(edit1, 1, blockFormat);

//    QTextBlock block1 = edit1->document()->firstBlock();
//    while (block1.isValid())
//    {
//        QTextBlock block2 = findBlock(ui->plainTextEdit2, block1);
//        if (block2.isValid())
//        {

//        }
//        else
//        {
//            QTextBlockFormat blockFormat;
//            blockFormat.setBackground(Qt::BDiagPattern);
//            block2 = edit2->document()->findBlockByNumber(block1.blockNumber() - 1);
//            QTextCursor cursor(block2);
//            cursor.insertBlock(blockFormat);
//            cursor.insertText("heheh");
//            //edit2->setTextCursor(cursor);
//        }
//        block1 = block1.next();
//    }


//    QTextCursor cursor(block);
//    QTextBlockFormat blockFormat = cursor.blockFormat();
//    blockFormat.setBackground(QColor(Qt::red).lighter());
//    cursor.setBlockFormat(blockFormat);
}

QTextBlock DeveloperMode::findBlock(QPlainTextEdit *edit, const QTextBlock &block)
{
    BlockInfo blockInfo = makeBlockInfo(block);

    QTextBlock tempBlock = edit->document()->findBlockByNumber(block.blockNumber());
    while (tempBlock.isValid())
    {
        BlockInfo tempBlockInfo = makeBlockInfo(tempBlock);
        if (tempBlockInfo == blockInfo)
        {
            return tempBlock;
        }

        tempBlock = tempBlock.next();
    }
    return QTextBlock();
}

void DeveloperMode::on_pushButton_copy_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->lineEdit_result->text());
}

void DeveloperMode::on_pushButton_open1_clicked()
{
    QString strFilePath = QFileDialog::getOpenFileName(this, "打开文件", "./");
    QFileInfo fileInfo(strFilePath);
    if (!fileInfo.exists())
    {
        return;
    }
    ui->lineEdit_filePath1->setText(strFilePath);
    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "警告", QString("打开文件失败：%1").arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
    ui->plainTextEdit1->setPlainText(in.readAll());
    file.close();
    //
    compareEdit(ui->plainTextEdit1, ui->plainTextEdit2);
}
