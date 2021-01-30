#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QFontDatabase>
#include <QtDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    QString appPath = a.applicationDirPath();
    int fontId = QFontDatabase::addApplicationFont(QString("%1/YaHei.Consolas.1.12.ttf").arg(appPath));
    if (fontId >= 0)
    {
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).first();
        a.setFont(QFont(fontFamily));
    }

    MainWindow w;
    w.show();

    return a.exec();
}
