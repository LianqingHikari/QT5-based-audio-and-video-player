#ifndef CHANGEBUTTONCOLOR_H
#define CHANGEBUTTONCOLOR_H

#include <QToolButton>
#include<qDebug>

class ChangeButtonColor : public QToolButton
{
    Q_OBJECT
public:
    explicit ChangeButtonColor(QWidget *parent = nullptr);
    ~ChangeButtonColor();
protected:
    void enterEvent(QEnterEvent * e);
    void leaveEvent(QEvent *);
//    void mouseMoveEvent(QMouseEvent * event);
signals:

};

#endif // CHANGEBUTTONCOLOR_H
