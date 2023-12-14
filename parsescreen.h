#ifndef PARSESCREEN_H
#define PARSESCREEN_H

#include <QPropertyAnimation>
#include <QWidget>
#include <qnetworkaccessmanager.h>
#include <tilesselector.h>

namespace Ui {
class ParseScreen;
}

class ParseScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ParseScreen(QWidget *parent = nullptr);
    ~ParseScreen();

private:
    Ui::ParseScreen *ui;
    QAtomicInt filesToLoad;
    QAtomicInt filesToLoadTotal;
    TilesSelector* tileSelector;
private slots:
    void parseFiles();
    void fileLoad();
};

#endif // PARSESCREEN_H
