#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    ParametersWindow* p;

private slots:
    void showSettingsScreen();
    void showDynamicScreen();
    void showVectorScreen();
    void showDynamicMap(QVector<QString> checkedDist);
    void showVectorMap(QVector<QString> checkedDist);
    void loadVector(Params * par);
};
#endif // MAINWINDOW_H
