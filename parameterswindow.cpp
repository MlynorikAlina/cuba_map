#include "parameterswindow.h"
#include "ui_parameterswindow.h"

#include <QRegExpValidator>
#include "params.h"

ParametersWindow::ParametersWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametersWindow)
{
    ui->setupUi(this);
    ui->latEdit->setText("6.5");
    ui->lonEdit->setText("80.5");

    QRegExpValidator *validatorLat =
        new QRegExpValidator(QRegExp("^-?(90.0)|((([0-8]\\d)|\\d)\\.\\d{6})$"));
    ui->latEdit->setValidator(validatorLat);
    QRegExpValidator *validatorLon =
        new QRegExpValidator(QRegExp("^-?(180.0)|(((1[0-7]\\d)|(\\d?\\d))\\.\\d{6})$"));
    ui->lonEdit->setValidator(validatorLon);
}

ParametersWindow::~ParametersWindow()
{
    delete ui;
}
