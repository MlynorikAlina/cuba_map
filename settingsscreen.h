#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include "tiles/MapFeatures.h"
#include <QJsonObject>
#include <QStandardItem>
#include <QTabWidget>
#include <QWidget>
#include <tiles/OverpassFilter.h>

struct FeatureList{
    QJsonObject jobject;
    FeatureList();
private:
    void defaultInit();
};

namespace Ui {
class SettingsScreen;
}

class SettingsScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsScreen(QWidget *parent = nullptr);
    ~SettingsScreen();

    static OverpassFilter* getFilter(int dist);

private:
    FeatureList fl;
    Ui::SettingsScreen *ui;
    QTabWidget* filter;
    static QMap<int, QMap<QString, bool>> h_filter;
private slots:
    void filterChanged(const QModelIndex & ind);
};

struct Tab_X
{
    QHash<QString, bool> featuresIncluaded;
};



#endif // SETTINGSSCREEN_H
