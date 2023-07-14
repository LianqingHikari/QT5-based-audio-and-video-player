#include "playerslider.h"

PlayerSlider::PlayerSlider(QWidget *parent) : QSlider(parent)
{

}

void PlayerSlider::mousePressEvent(QMouseEvent *ev)
{
    //获取当前点击位置,得到的这个鼠标坐标是相对于当前QSlider的坐标
    int currentX = ev->pos().x();
    //获取当前点击的位置占整个Slider的百分比
    double per = currentX *1.0 / this->width();
    //利用算得的百分比得到具体数字
    double value = per*(this->maximum() - this->minimum()) + this->minimum();
    //设定滑动条位置
    this->setValue(value);
    //滑动条移动事件等事件也用到了mousePressEvent,加这句话是为了不对其产生影响，使得Slider能正常相应其他鼠标事件
    //qDebug()<<value;
    emit mousepress(value);
    QSlider::mousePressEvent(ev);
}

void PlayerSlider::mouseReleaseEvent(QMouseEvent *ev)
{

    //获取当前点击位置,得到的这个鼠标坐标是相对于当前QSlider的坐标
    int currentX = ev->pos().x();
    //获取当前点击的位置占整个Slider的百分比
    double per = currentX *1.0 /this->width();
    //利用算得的百分比得到具体数字
    double value = per*(this->maximum() - this->minimum()) + this->minimum();
    //设定滑动条位置
    this->setValue(value);
    //滑动条移动事件等事件也用到了mousePressEvent,加这句话是为了不对其产生影响，使得Slider能正常相应其他鼠标事件
    //qDebug()<<value;
    emit mouserelease(value);
    QSlider::mouseReleaseEvent(ev);
}

void PlayerSlider::mouseMoveEvent(QMouseEvent *ev)
{
    double value=float(ev->pos().x())/float(this->size().width());
    emit SliderMouseMove(value);
}

void PlayerSlider::leaveEvent(QEvent * e)
{
    emit SliderMouseLeave();
}
