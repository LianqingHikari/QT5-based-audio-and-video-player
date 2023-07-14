#ifndef GETFRAME_H
#define GETFRAME_H

#include <QWidget>

namespace Ui {
class GetFrame;
}

class GetFrame : public QWidget
{
    Q_OBJECT

public:
    explicit GetFrame(QWidget *parent = nullptr);
    ~GetFrame();
    Ui::GetFrame *ui;

private:

};

#endif // GETFRAME_H
