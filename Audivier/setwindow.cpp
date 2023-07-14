#include "setwindow.h"
#include "ui_setwindow.h"

SetWindow::SetWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetWindow)
{
    ui->setupUi(this);
}

SetWindow::~SetWindow()
{
    delete ui;
}

void SetWindow::on_toolButton_clicked()
{
    if(this->PlaySpeed==1)
    {
        this->PlaySpeed=2;
        ui->toolButton->setIcon(QIcon(":/images/2x.png"));
    }
    else if(this->PlaySpeed==2)
    {
        this->PlaySpeed=1;
        ui->toolButton->setIcon(QIcon(":/images/1x.png"));
    }
    emit PlaySpeedSignal(this->PlaySpeed);
}


void SetWindow::on_toolButton_3_clicked()
{
    this->close();
}


void SetWindow::on_toolButton_2_clicked()
{
    if(this->frame==1)
    {
        this->frame=5;
        ui->toolButton_2->setIcon(QIcon(":/images/FiveFrame.png"));
    }
    else if(this->frame==5)
    {
        this->frame=1;
        ui->toolButton_2->setIcon(QIcon(":/images/OneFrame.png"));
    }
    emit FrameSignal(this->frame);
}
