#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "maploader.h"
#include <QMainWindow>
#include <QMenu>
#include <parameterswindow.h>
#include <qcheckbox.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMenu* mapOptions;
    MapLoader* mapLoader;
private slots:
    void showDynamicParameters();
    void showSettingsScreen();
    void showDynamicScreen();
    void showVectorScreen();
    void showStaticScreen();
    void showDynamicMap(QVector<QString> checkedDist);
    void showVectorMap(QVector<QString> checkedDist);
    void showStaticMap(QVector<QString> checkedDist);
};
#endif // MAINWINDOW_H
