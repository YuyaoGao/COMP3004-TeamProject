#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "Dir===" << QDir::currentPath();
    QFile file(QDir::currentPath() + "/blue.css");
            if (file.open(QFile::ReadOnly))
            {
                QString qss = QLatin1String(file.readAll());
                qApp->setStyleSheet(qss);
                file.close();
            }
    MainWindow w;
    w.show();
    return a.exec();
}
