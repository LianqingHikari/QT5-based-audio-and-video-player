#ifndef SETWINDOW_H
#define SETWINDOW_H

#include <QWidget>

namespace Ui {
class SetWindow;
}

class SetWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SetWindow(QWidget *parent = nullptr);
    ~SetWindow();

private slots:
    void on_toolButton_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::SetWindow *ui;
    int frame=1;
    int PlaySpeed=1;

signals:
    void FrameSignal(int);
    void PlaySpeedSignal(int);
};

#endif // SETWINDOW_H
