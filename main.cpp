#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <params.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    //w.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    //w.setFixedSize(QSize(1024,768)); // BREAKS COMBOBOX!!!!

    //w.setMaximumWidth(1024);
    //w.setFixedHeight(768);

    //w.setFixedWidth(900);
    //w.setFixedHeight(768);
    //w.setMaximumHeight(901);
    //w.setMinimumHeight(900);
    w.show();
    w.setWindowTitle(" ");
    return a.exec();
}
