#include "settingsscreen.h"
#include "params.h"
#include "qdebug.h"
#include "qheaderview.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QFile>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QListView>
#include <QRadioButton>
#include <QScrollArea>
#include <QStandardItemModel>
#include <QTabWidget>
#include <QTableView>
#include <QToolBox>
#include <QVBoxLayout>

QMap<int, QMap<QString, bool>> SettingsScreen::h_filter;
int SettingsScreen::checkedThemeInd = 0;
QVector<QString> SettingsScreen::themeVec({VEC_STYLE, DARK_VEC_STYLE, MIN_COL_VEC_STYLE});
QVector<QString> SettingsScreen::themeStat({STATIC_STYLE, DARK_STATIC_STYLE, MIN_COL_STATIC_STYLE});


SettingsScreen::SettingsScreen(QWidget *parent)
    : QWidget(parent){
    w = new QScrollArea(this);
    w->setLayout(new QVBoxLayout);
    w->setMaximumWidth(W_WIDTH);
    w->resize(QSize(W_WIDTH,W_HEIGHT-22));
    //move(1,1);
    //setBaseSize(QSize(W_WIDTH, W_HEIGHT));

    QGroupBox *filterGB = new QGroupBox("Filter");
    w->layout()->addWidget(filterGB);
    w->layout()->setAlignment(Qt::AlignTop);
    filterGB->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    filterGB->setMaximumHeight(0);

    QGroupBox *themeGB = new QGroupBox("Theme");
    w->layout()->addWidget(themeGB);
    themeGB->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    themeGB->setLayout(new QVBoxLayout);
    theme.append(new QRadioButton("default"));
    theme.append(new QRadioButton("dark"));
    theme.append(new QRadioButton("grey"));
    theme[0]->setChecked(true);
    addTheme(themeGB);


    filterGB->setLayout(new QVBoxLayout);
    filter = new QTabWidget();
    filterGB->layout()->addWidget(filter);
    // Fixme::json

    QJsonObject arr = fl.jobject;
    QHashIterator<QString, QVariant> ind = arr.toVariantHash();
    while (ind.hasNext()) {
        ind.next();
        QJsonObject tabFeatures = ind.value().toJsonObject();
        QHashIterator<QString, QVariant> e = tabFeatures.toVariantHash();
        while (e.hasNext()) {
            e.next();
            h_filter[ind.key().toInt()].insert(e.key(), e.value().toBool());
        }
    }

    QMapIterator<int, QMap<QString, bool>> it = h_filter;
    while (it.hasNext()) {
        it.next();
        QTableView *tab = new QTableView();
        tab->setSelectionMode(QAbstractItemView::NoSelection);
        tab->setFocusPolicy(Qt::NoFocus);
        tab->setShowGrid(false);
        tab->horizontalHeader()->hide();
        tab->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        tab->verticalHeader()->hide();

        QStandardItemModel *model = new QStandardItemModel(tab);
        int i = 0;
        QList<QStandardItem *> l;
        QMapIterator<QString, bool> it2 = it.value();
        while (it2.hasNext()) {
            it2.next();
            if (i % 10 == 0 && i > 0) {
                model->appendColumn(l);
                l.clear();
            }
            QStandardItem *poListItem = new QStandardItem(it2.key());

            // Checkable item
            poListItem->setCheckable(true);

            // Save checke state
            if (it2.value())
                poListItem->setData(Qt::Checked, Qt::CheckStateRole);
            else
                poListItem->setData(Qt::Unchecked, Qt::CheckStateRole);
            l.append(poListItem);
            i++;
        }
        if (l.size() > 0)
            model->appendColumn(l);

        filterGB->setMaximumHeight(std::max(tab->height(), filterGB->maximumHeight()));

        tab->setModel(model);

        filter->addTab(tab, QString::number(it.key()) + " km");

        connect(tab, &QTableView::clicked, this, &SettingsScreen::filterChanged);
    }
}

SettingsScreen::~SettingsScreen() {
    QJsonObject jobject;
    QFile f(MAP_FEATURES_FILE);
    f.open(QIODevice::WriteOnly);

    QMapIterator<int, QMap<QString, bool>> it = h_filter;
    while (it.hasNext()) {
        it.next();
        QJsonObject dist;
        QMapIterator<QString, bool> it2 = it.value();
        while (it2.hasNext()) {
            it2.next();
            dist.insert(it2.key(), it2.value());
        }
        jobject.insert(QString::number(it.key()), dist);
    }
    QJsonDocument doc(jobject);
    f.write(doc.toJson());
    f.close();
    delete w;
}

OverpassFilter *SettingsScreen::getFilter(int dist) {
    OverpassFilter *f = new OverpassFilter;
    std::set<map_features::MapFeature> include;
    QMapIterator<QString, bool> it = h_filter[dist];
    while (it.hasNext()) {
        it.next();
        if (!it.value())
            include.insert(map_features::from_string(it.key().toStdString()));
    }
    f->excludeFeatures(map_features::getAllExcept(include));
    return f;
}

QString SettingsScreen::getVecStyle()
{
    return themeVec[checkedThemeInd];
}

QString SettingsScreen::getStatStyle()
{
    return themeStat[checkedThemeInd];
}

void SettingsScreen::updCheckedThemeInd()
{
    int i=0;
    for(i; i< theme.size() && !theme[i]->isChecked(); i++);
    checkedThemeInd = i;
}

void SettingsScreen::addTheme(QGroupBox *group) {
    for (auto rb : theme){
        group->layout()->addWidget(rb);
        connect(rb, &QRadioButton::clicked, this, &SettingsScreen::updCheckedThemeInd);
    }
    group->setMaximumHeight(2 * theme.size() * theme[0]->sizeHint().height());
}

void SettingsScreen::filterChanged(const QModelIndex &ind) {
    bool checked = ind.data(Qt::CheckStateRole).toBool();
    QString option = ind.data().toString();
    QString dist = filter->tabText(filter->currentIndex()).split(' ')[0];

    h_filter[dist.toInt()][option] = checked;
}

FeatureList::FeatureList() {
    if (!QFile::exists(MAP_FEATURES_FILE)) {
        defaultInit();
        QJsonDocument doc(jobject);
        QFile f(MAP_FEATURES_FILE);
        f.open(QIODevice::WriteOnly);
        f.write(doc.toJson());
        f.close();
    } else {
        QFile f(MAP_FEATURES_FILE);
        f.open(QIODevice::ReadOnly);
        QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
        jobject = doc.object();
        f.close();
    }
}

void FeatureList::defaultInit() {
    QJsonObject _100km, _70km, _50km, _35km, _25km, _20km, _5km;
    for (auto e : f_100km)
        _100km.insert(QString::fromStdString(map_features::to_string(e)), true);
    for (auto e : f_70km)
        _70km.insert(QString::fromStdString(map_features::to_string(e)), true);
    for (auto e : f_50km)
        _50km.insert(QString::fromStdString(map_features::to_string(e)), true);
    for (auto e : f_35km)
        _35km.insert(QString::fromStdString(map_features::to_string(e)), true);
    for (auto e : f_25km)
        _25km.insert(QString::fromStdString(map_features::to_string(e)), true);
    for (auto e : f_20km)
        _20km.insert(QString::fromStdString(map_features::to_string(e)), true);
    for (auto e : f_5km)
        _5km.insert(QString::fromStdString(map_features::to_string(e)), true);

    jobject.insert("100", _100km);
    jobject.insert("70", _70km);
    jobject.insert("50", _50km);
    jobject.insert("35", _35km);
    jobject.insert("25", _25km);
    jobject.insert("20", _20km);
    jobject.insert("5", _5km);
}
