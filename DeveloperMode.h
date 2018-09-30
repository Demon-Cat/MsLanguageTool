#ifndef DEVELOPERMODE_H
#define DEVELOPERMODE_H

#include <QWidget>
#include <QPlainTextEdit>

namespace Ui {
class DeveloperMode;
}

enum BlockType
{
    BlockUnknow,    //未知类型
    BlockGroup,     //[xxx]
    BlockLanguage,  //xxx=xxx
    BlockHeader,    //Language and LangID
    BlockEmpty      //空行
};

struct BlockInfo
{
    int blockNumber = 0;
    BlockType type = BlockUnknow;
    QString strBlock;
    QString strGroup;
    QString strKey;
    QString strValue;

    bool operator<(const BlockInfo &other) const
    {
        return strBlock < other.strBlock;
    }

    bool operator==(const BlockInfo &other) const
    {
        if (type == other.type)
        {
            switch (type)
            {
            case BlockLanguage:
                return strKey == other.strKey;
            default:
                return strBlock == other.strBlock && strGroup == other.strGroup && strKey == other.strKey && strValue == other.strValue;
            }
        }
        else
        {
            return false;
        }
    }
};

class DeveloperMode : public QWidget
{
    Q_OBJECT

public:
    explicit DeveloperMode(QWidget *parent = 0);
    ~DeveloperMode();

private:
    //插入块
    void insertBlock(QPlainTextEdit *edit, int blockNumber, const QString &text, const QTextBlockFormat &blockFormat);
    //设置块样式
    void setBlockFormat(QPlainTextEdit *edit, int blockNumber, const QTextBlockFormat &blockFormat);
    BlockInfo makeBlockInfo(const QTextBlock &block);
    void compareEdit(QPlainTextEdit *edit1, QPlainTextEdit *edit2);
    QTextBlock findBlock(QPlainTextEdit *edit, const QTextBlock &block);

private slots:
    void on_pushButton_copy_clicked();
    void on_pushButton_open1_clicked();

private:
    Ui::DeveloperMode *ui;
};

#endif // DEVELOPERMODE_H
