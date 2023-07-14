#include "getframe.h"
#include "ui_getframe.h"

GetFrame::GetFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GetFrame)
{
    ui->setupUi(this);
    ui->label->setScaledContents(1);
    setFixedSize(this->width(),this->height());
}

GetFrame::~GetFrame()
{
    delete ui;
}
