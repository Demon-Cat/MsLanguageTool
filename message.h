#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>
#include <QEventLoop>

namespace Ui {
class Message;
}

class Message : public QWidget
{
    Q_OBJECT

public:
    enum StandardButton {
        NoButton           = 0x00000000,
        Ok                 = 0x00000400,
        Save               = 0x00000800,
        SaveAll            = 0x00001000,
        Open               = 0x00002000,
        Yes                = 0x00004000,
        YesToAll           = 0x00008000,
        No                 = 0x00010000,
        NoToAll            = 0x00020000,
        Abort              = 0x00040000,
        Retry              = 0x00080000,
        Ignore             = 0x00100000,
        Close              = 0x00200000,
        Cancel             = 0x00400000,
        Discard            = 0x00800000,
        Help               = 0x01000000,
        Apply              = 0x02000000,
        Reset              = 0x04000000,
        RestoreDefaults    = 0x08000000,

        FirstButton        = Ok,                // internal
        LastButton         = RestoreDefaults,   // internal

        YesAll             = YesToAll,          // obsolete
        NoAll              = NoToAll,           // obsolete

        Default            = 0x00000100,        // obsolete
        Escape             = 0x00000200,        // obsolete
        FlagMask           = 0x00000300,        // obsolete
        ButtonMask         = ~FlagMask          // obsolete
    };

    explicit Message(QWidget *parent = nullptr);
    ~Message();

    static int question(const QString &text);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();

private:
    static Message *instance();
    int showQuestion(const QString &text);

private:
    static Message *self;

    Ui::Message *ui;
    QEventLoop m_eventLoop;
};

#endif // MESSAGE_H
