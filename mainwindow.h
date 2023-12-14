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
    void showParseScreen();
    void showMapOptionsScreen();
    void showDynamicMap(QVector<QCheckBox*> checkedDist);
    void showVectorMap(QVector<QCheckBox*> checkedDist);
};
#endif // MAINWINDOW_H
