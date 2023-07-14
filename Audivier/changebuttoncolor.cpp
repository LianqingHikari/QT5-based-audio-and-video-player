#include "changebuttoncolor.h"

ChangeButtonColor::ChangeButtonColor(QWidget *parent)
    : QToolButton{parent}
{

}


ChangeButtonColor::~ChangeButtonColor()
{

}

void ChangeButtonColor::enterEvent(QEnterEvent * e)
{
    //qDebug()<<"鼠标移入";
    QToolButton::enterEvent(e);
    this->setStyleSheet("QToolButton{background:rgba(135, 206, 250,0.3);}");
}
void ChangeButtonColor::leaveEvent(QEvent * e)
{
    this->setStyleSheet("QToolButton{background:rgba(135, 206, 250,0);}");
    //qDebug()<<"鼠标移出";
}

//void ChangeButtonColor::mouseMoveEvent(QMouseEvent * event)
//{
//    qDebug()<<"鼠标移动";
//}
