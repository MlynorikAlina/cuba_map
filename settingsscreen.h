#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include "tiles/MapFeatures.h"
#include <QJsonObject>
#include <QStandardItem>
#include <QTabWidget>
#include <QWidget>
#include <tiles/OverpassFilter.h>
#include <QGroupBox>
#include <QRadioButton>
#include <QScrollArea>

struct FeatureList{
    QJsonObject jobject;
    FeatureList();
private:
    void defaultInit();
};



class SettingsScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsScreen(QWidget *parent = nullptr);
    ~SettingsScreen();

    static OverpassFilter* getFilter(int dist);
    static QString getVecStyle();
    static QString getStatStyle();
private:
    FeatureList fl;
    QTabWidget* filter;
    QVector<QRadioButton *> theme;
    QScrollArea* w;
    static QVector<QString> themeVec;
    static QVector<QString> themeStat;
    static QMap<int, QMap<QString, bool>> h_filter;
    static int checkedThemeInd;
    void addTheme(QGroupBox* group);
    static void initTheme();
private slots:
    void updCheckedThemeInd();
    void filterChanged(const QModelIndex & ind);
};

struct Tab_X
{
    QHash<QString, bool> featuresIncluaded;
};



#endif // SETTINGSSCREEN_H
