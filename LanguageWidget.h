#ifndef LANGUAGEWIDGET_H
#define LANGUAGEWIDGET_H

#include <QWidget>
#include <QMap>
#include <QFileSystemWatcher>

namespace Ui {
class LanguageWidget;
}

class LanguageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LanguageWidget(QWidget *parent = 0);
    ~LanguageWidget();

    void setIndex(int index);
    void loadFile(const QString &fileName);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void emitCursorPositionChanged(int blockNumber, const QString &str);

signals:
    void sig_result(const QString &str);
    void sig_cursorPositionChanged(int blockNumber, const QString &str);

public slots:
    void onSetCursorPosition(int blockNumber, const QString &str);

private slots:
    void onFileChanged(const QString &strFile);

    void on_pushButton_open_clicked();
    void on_pushButton_reload_clicked();
    void on_pushButton_save_clicked();
    void on_lineEdit_filter_textChanged(const QString &str);
    void on_plainTextEdit_cursorPositionChanged();
    void on_plainTextEdit_textChanged();
    void on_comboBox_filter_activated(const QString &str);

private:
    Ui::LanguageWidget *ui;

    QFileSystemWatcher *m_fileWatcher = nullptr;

    int m_currentIndex = 0;
    QString m_currentGroup;

    bool m_detectTextChanged = true;
    bool m_isEmitCursorPositionChange = true;
};

#endif // LANGUAGEWIDGET_H
