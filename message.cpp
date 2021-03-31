#include "message.h"
#include "ui_message.h"
#include <QPainter>
#include <QStyle>

Message *Message::self = nullptr;

Message::Message(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Message)
{
    self = this;
    ui->setupUi(this);
}

Message::~Message()
{
    self = nullptr;
    delete ui;
}

int Message::question(const QString &text)
{
    return self->showQuestion(text);
}

void Message::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 80));
    painter.drawRect(rect());
}

Message *Message::instance()
{
    return self;
}

int Message::showQuestion(const QString &text)
{
    ui->labelMessage->setText(text);

    QRect rc = parentWidget()->rect();
    setGeometry(rc);

    QPoint containerPos = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, ui->widgetContainer->size(), rc).topLeft();
    ui->widgetContainer->move(containerPos);

    show();

    return m_eventLoop.exec();
}

void Message::on_pushButtonOk_clicked()
{
    m_eventLoop.exit(StandardButton::Yes);
    hide();
}

void Message::on_pushButtonCancel_clicked()
{
    m_eventLoop.exit(StandardButton::Cancel);
    hide();
}
