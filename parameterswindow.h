#ifndef PARAMETERSWINDOW_H
#define PARAMETERSWINDOW_H

#include <QCheckBox>
#include <QWidget>

namespace Ui {
class ParametersWindow;
}

struct Params{
    QVector<QString> checkedDist;
    QString c_lat;
    QString c_lon;
    bool isDynamicParams;
};

class ParametersWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ParametersWindow(QWidget *parent = nullptr);
    ~ParametersWindow();
    void setDynamicParams();
    void setVectorParams();
    void setProgressRange(int min, int max);
    void updProgress(int val);
    void updProgress();
signals:
    void parametersLoaded(Params* p);
    void showDynamic(QVector<QString> checkedDist);
    void showVector(QVector<QString> checkedDist);
public slots:
    void finishProgress();
private: 
    Ui::ParametersWindow *ui;
    bool isDynamicParams;
    Params* p;
private slots:
    void getParams();
};

#endif // PARAMETERSWINDOW_H
