#include "parameterswindow.h"
#include "ui_parameterswindow.h"

#include <QRegExpValidator>
#include "params.h"


ParametersWindow::ParametersWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParametersWindow)
{
    ui->setupUi(this);
    ui->latEdit->setText("23.01373");
    ui->lonEdit->setText("-82.2425");


    QRegExpValidator *validatorLat =
        new QRegExpValidator(QRegExp("^-?(90.0)|((([0-8]\\d)|\\d)\\.\\d{6})$"));
    ui->latEdit->setValidator(validatorLat);
    QRegExpValidator *validatorLon =
        new QRegExpValidator(QRegExp("^-?((180.0)|(((1[0-7]\\d)|(\\d?\\d))\\.\\d{6}))$"));
    ui->lonEdit->setValidator(validatorLon);

    ui->progressBar->setRange(0,0);
    ui->progressBar->setValue(-1);
    ui->progressBar->hide();

    connect(ui->cancelButton, &QPushButton::clicked, this, &QWidget::hide);
    connect(ui->okButton, &QPushButton::clicked, this, &ParametersWindow::getParams);
}



ParametersWindow::~ParametersWindow()
{
    delete ui;
}

void ParametersWindow::setDynamicParams()
{
    mode = DYNAMIC;

    for (auto e : ui->distance->findChildren<QCheckBox*>())
        e->setCheckState(Qt::Unchecked);

    ui->cb_001->hide();
    ui->cb_002->hide();
    ui->cb_005->hide();
    ui->cb_020->hide();
    ui->cb_025->hide();
    ui->cb_035->hide();
    ui->cb_070->hide();
    ui->cb_050->show();
    ui->cb_100->show();
    ui->cb_100->setCheckState(Qt::Checked);
}

void ParametersWindow::setVectorParams()
{
    mode = VECTOR;

    for (auto e : ui->distance->findChildren<QCheckBox*>())
        e->setCheckState(Qt::Unchecked);

    ui->cb_001->show();
    ui->cb_002->show();
    ui->cb_005->show();
    ui->cb_005->setCheckState(Qt::Checked);
    ui->cb_020->hide();
    ui->cb_025->hide();
    ui->cb_035->hide();
    ui->cb_050->hide();
    ui->cb_070->hide();
    ui->cb_100->hide();
}

void ParametersWindow::setStaticParams()
{
    mode = STATIC;

    for (auto e : ui->distance->findChildren<QCheckBox*>())
        e->setCheckState(Qt::Unchecked);

    ui->cb_001->hide();
    ui->cb_002->hide();
    ui->cb_005->hide();
    ui->cb_020->show();
    ui->cb_025->show();
    ui->cb_035->show();
    ui->cb_050->show();
    ui->cb_070->show();
    ui->cb_100->show();
    ui->cb_100->setCheckState(Qt::Checked);
}

void ParametersWindow::setProgressRange(int min, int max)
{
    ui->progressBar->setRange(min,max);
    ui->progressBar->setValue(min);
    ui->progressBar->show();
    ui->cancelButton->hide();
    ui->okButton->hide();
}

void ParametersWindow::updProgress()
{
    updProgress(ui->progressBar->value()+1);
}

void ParametersWindow::finishProgress()
{
    ui->progressBar->hide();
    ui->cancelButton->show();
    ui->okButton->show();
    this->hide();
    switch(mode){
    case DYNAMIC:
        emit showDynamic(p->checkedDist);
        break;
    case VECTOR:
        emit showVector(p->checkedDist);
        break;
    case STATIC:
        emit showStatic(p->checkedDist);
        break;
    }
}

void ParametersWindow::updProgress(int val)
{
    if(ui->progressBar->maximum() == val){
        finishProgress();
    }else ui->progressBar->setValue(val);
}

void ParametersWindow::getParams()
{
    p = new Params;

    p->checkedDist.clear();
    for (auto e : ui->distance->findChildren<QCheckBox *>()) {
        if (e->isChecked()) {
            p->checkedDist.push_back(e->text());
        }
    }
    p->mode = mode;
    p->c_lat = ui->latEdit->text();
    p->c_lon = ui->lonEdit->text();
    emit parametersLoaded(p);
}
