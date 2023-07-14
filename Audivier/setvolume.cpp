#include "setvolume.h"
#include "ui_setvolume.h"
#include <QFile>

SetVolume::SetVolume(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetVolume)
{
    ui->setupUi(this);
}

SetVolume::~SetVolume()
{
    delete ui;
}

void SetVolume::on_toolButton_clicked()
{
    this->close();
}


void SetVolume::on_verticalSlider_valueChanged(int value)
{
    emit VolumeSignal(ui->verticalSlider->value());
    if(value==0)
    {
        ui->toolButton->setIcon(QIcon(":/images/Mute.png"));
    }
    else
    {
        ui->toolButton->setIcon(QIcon(":/images/Voice.png"));
    }
    //()<<ui->verticalSlider->value();
}

