#include "LanguageWidget.h"
#include "ui_LanguageWidget.h"
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>
#include <QTextBlock>
#include <QTextStream>
#include <QtDebug>

LanguageWidget::LanguageWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LanguageWidget)
{
    ui->setupUi(this);

    m_fileWatcher = new QFileSystemWatcher(this);
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &LanguageWidget::onFileChanged);
}

LanguageWidget::~LanguageWidget()
{
    delete ui;
}

void LanguageWidget::setIndex(int index)
{
    m_currentIndex = index;
}

void LanguageWidget::loadFile(const QString &fileName)
{
    m_detectTextChanged = false;
    int currentBlock = ui->plainTextEdit->textCursor().blockNumber();
    ui->plainTextEdit->clear();

    //
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "警告", QString("打开文件失败:%1").arg(file.errorString()));
        return;
    }
    ui->lineEdit_filePath->setText(fileName);

    //
    QSettings setting(QString("%1/setting.ini").arg(qApp->applicationDirPath()), QSettings::IniFormat);
    setting.setValue("BasePath", QDir(fileName).absolutePath());
    setting.setValue(QString("FilePath%1").arg(m_currentIndex + 1), fileName);

    //File Watcher
    QStringList watchList = m_fileWatcher->files();
    if (!watchList.isEmpty()) {
        m_fileWatcher->removePaths(m_fileWatcher->files());
    }
    m_fileWatcher->addPath(fileName);

    //
    ui->comboBox_filter->clear();
    QMap<QString, int> m_groupMap;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString str = in.readLine();
        ui->plainTextEdit->appendPlainText(str);

        if (str.startsWith("[")) {
            QString strGroup = str.remove(QRegularExpression("[\\[\\]]"));
            m_groupMap.insert(strGroup, 0);
        }
    }
    for (auto iter = m_groupMap.constBegin(); iter != m_groupMap.constEnd(); ++iter) {
        QString text = iter.key();
        ui->comboBox_filter->addItem(text);
    }
    //
    QTextBlock block = ui->plainTextEdit->document()->findBlockByNumber(currentBlock);
    ui->plainTextEdit->setTextCursor(QTextCursor(block));
    m_detectTextChanged = true;
}

void LanguageWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S) {
        on_pushButton_save_clicked();
    }
    QWidget::keyPressEvent(event);
}

void LanguageWidget::emitCursorPositionChanged(int blockNumber, const QString &str)
{
    if (m_isEmitCursorPositionChange) {
        emit sig_cursorPositionChanged(blockNumber, str);
    }
}

void LanguageWidget::onSetCursorPosition(int blockNumber, const QString &str)
{
    m_isEmitCursorPositionChange = false;
    if (str.isEmpty()) {
        QTextBlock tempBlock = ui->plainTextEdit->document()->findBlockByNumber(blockNumber);
        QTextCursor textCursor(tempBlock);
        ui->plainTextEdit->setTextCursor(textCursor);
        m_isEmitCursorPositionChange = true;
        return;
    }
    //
    QTextBlock block = ui->plainTextEdit->document()->begin();
    bool found = false;
    bool isGroup = str.startsWith("[");
    QString strKey;
    if (!isGroup) {
        QStringList strList = str.split("=");
        strKey = strList.at(0);
    }
    while (block.isValid()) {
        if (isGroup) {
            if (block.text() == str) {
                found = true;
            }
        } else {
            if (block.text().left(strKey.size()) == strKey) {
                found = true;
            }
        }

        if (found) {
            QTextCursor textCursor(block);
            ui->plainTextEdit->setTextCursor(textCursor);
            break;
        }

        block = block.next();
    }

    if (!found) {
        QTextBlock tempBlock = ui->plainTextEdit->document()->findBlockByNumber(blockNumber);
        QTextCursor textCursor(tempBlock);
        ui->plainTextEdit->setTextCursor(textCursor);
    }
    m_isEmitCursorPositionChange = true;
}

void LanguageWidget::onFileChanged(const QString &strFile)
{
    if (isActiveWindow()) {
        return;
    }
    int result = QMessageBox::question(this, "提示", QString("文件：%1发生改变，是否重新加载？").arg(strFile));
    if (result == QMessageBox::Yes) {
        on_pushButton_reload_clicked();
    }
}

void LanguageWidget::on_pushButton_open_clicked()
{
    QSettings setting(QString("%1/setting.ini").arg(qApp->applicationDirPath()), QSettings::IniFormat);
    QString basePath = setting.value("BasePath").toString();

    QString strPath = QFileDialog::getOpenFileName(this, "打开文件", basePath);
    if (strPath.isEmpty()) {
        return;
    }

    setting.setValue("BasePath", QDir(strPath).absolutePath());
    setting.setValue(QString("FilePath%1").arg(m_currentIndex + 1), strPath);

    loadFile(strPath);

    on_lineEdit_filter_textChanged(ui->lineEdit_filter->text());
}

void LanguageWidget::on_pushButton_reload_clicked()
{
    loadFile(ui->lineEdit_filePath->text());
    on_lineEdit_filter_textChanged(ui->lineEdit_filter->text());
}

void LanguageWidget::on_pushButton_save_clicked()
{
    QFile file(ui->lineEdit_filePath->text());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "警告", QString("打开文件失败:%1").arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    out << ui->plainTextEdit->toPlainText();
    file.close();

    ui->plainTextEdit->setStyleSheet("");
}

void LanguageWidget::on_lineEdit_filter_textChanged(const QString &str)
{
    QTextBlock block = ui->plainTextEdit->document()->begin();
    while (block.isValid()) {
        if (str.isEmpty()) {
            block.setVisible(true);
        } else if (block.text().contains(str)) {
            block.setVisible(true);
        } else {
            block.setVisible(false);
        }
        block = block.next();
    }
    ui->plainTextEdit->document()->adjustSize();
    ui->plainTextEdit->viewport()->update();

    QScrollBar *verScrollBar = ui->plainTextEdit->verticalScrollBar();
    verScrollBar->setSliderPosition(verScrollBar->minimum());

    if (str.isEmpty()) {
        ui->plainTextEdit->setTextCursor(ui->plainTextEdit->textCursor());
    }
}

void LanguageWidget::on_plainTextEdit_cursorPositionChanged()
{
    int currentBlock = ui->plainTextEdit->textCursor().blockNumber();
    QTextBlock block = ui->plainTextEdit->document()->findBlockByNumber(currentBlock);
    QString strValue = block.text();
    QStringList valueList = strValue.split("=");
    if (valueList.size() != 2) {
        emit sig_result("");
        emitCursorPositionChanged(currentBlock, strValue);
        return;
    }
    while (block.isValid()) {
        QString str = block.text().trimmed();
        if (str.startsWith("[") && str.endsWith("]")) {
            m_currentGroup = str.remove(QRegularExpression("[\\[\\]]"));
            QString strResult = QString("GET_TEXT(\"%1/%2\", \"%3\")").arg(m_currentGroup).arg(valueList.at(0)).arg(valueList.at(1));
            emit sig_result(strResult);
            //
            ui->comboBox_filter->setCurrentIndex(ui->comboBox_filter->findText(m_currentGroup));
            break;
        }
        block = block.previous();
    }

    emitCursorPositionChanged(currentBlock, strValue);
}

void LanguageWidget::on_plainTextEdit_textChanged()
{
    if (m_detectTextChanged) {
        ui->plainTextEdit->setStyleSheet("QPlainTextEdit{border: 1px solid #FF0000}");
    }
}

void LanguageWidget::on_comboBox_filter_activated(const QString &str)
{
    QTextBlock block = ui->plainTextEdit->document()->begin();
    QString strGroup = QString("[%1]").arg(str);
    while (block.isValid()) {
        if (block.text() == strGroup) {
            QTextCursor textCursor(block);
            ui->plainTextEdit->setTextCursor(textCursor);
        }
        block = block.next();
    }
}
