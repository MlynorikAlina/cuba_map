#ifndef PARAMETERSWINDOW_H
#define PARAMETERSWINDOW_H

#include <QWidget>

namespace Ui {
class ParametersWindow;
}

class ParametersWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ParametersWindow(QWidget *parent = nullptr);
    ~ParametersWindow();

private:
    Ui::ParametersWindow *ui;
};

#endif // PARAMETERSWINDOW_H
