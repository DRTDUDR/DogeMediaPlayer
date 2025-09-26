#include "supportwindow.h"
#include "ui_supportwindow.h"

supportwindow::supportwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::supportwindow)
{
    ui->setupUi(this);
}

supportwindow::~supportwindow()
{
    delete ui;
}

void supportwindow::on_closeBu_clicked()
{
    this->close();
}

