#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSslSocket>
#include <params.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    //w.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    //w.setFixedSize(QSize(1024,768)); // BREAKS COMBOBOX!!!!
    w.resize(W_WIDTH+5,W_HEIGHT+50);
    //w.setMaximumWidth(450);
    //w.setFixedHeight(768);

    //w.setFixedWidth(900);
    //w.setFixedHeight(768);
   // w.setMaximumHeight(450);
    //w.setMinimumHeight(900);
    qInfo() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
    w.show();
    w.setWindowTitle(" ");
    return a.exec();
}
