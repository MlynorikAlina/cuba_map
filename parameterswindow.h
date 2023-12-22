#ifndef PARAMETERSWINDOW_H
#define PARAMETERSWINDOW_H

#include <QCheckBox>
#include <QWidget>

namespace Ui {
class ParametersWindow;
}

enum Mode{
    STATIC, DYNAMIC, VECTOR
};
struct Params{
    QVector<QString> checkedDist;
    QString c_lat;
    QString c_lon;
    Mode mode;
};


class ParametersWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ParametersWindow(QWidget *parent = nullptr);
    ~ParametersWindow();
    void setDynamicParams();
    void setVectorParams();
    void setStaticParams();
    void setProgressRange(int min, int max);
    void updProgress(int val);
public slots:
    void updProgress();
    void finishProgress();
signals:
    void parametersLoaded(Params* p);
    void showDynamic(QVector<QString> checkedDist);
    void showVector(QVector<QString> checkedDist);
    void showStatic(QVector<QString> checkedDist);
private: 
    Ui::ParametersWindow *ui;
    Mode mode;
    Params* p;
private slots:
    void getParams();
};

#endif // PARAMETERSWINDOW_H
