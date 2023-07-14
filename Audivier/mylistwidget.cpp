#include "mylistwidget.h"
#include "mainwindow.h"

MyListWidget::MyListWidget(QWidget *parent)
    : QListWidget{parent}
{
    this->setAcceptDrops(true);
    this->setDragEnabled(true);

//    ContextMenu = new QMenu;
    //NextPlay = new QAction("下一首播放",this);
//    Information = new QAction("查看音视频信息",this);
    //ContextMenu->addAction(NextPlay);
//    ContextMenu->addAction(Information);
}

MyListWidget::~MyListWidget()
{

}

void MyListWidget::dropEvent(QDropEvent *event)
{
    //qDebug()<<"dropemit";
    QListWidget::dropEvent(event);
    emit ItemDragSignal();
}

//void MyListWidget::mousePressEvent(QMouseEvent *event)
//{
//    //确保右键点击，然后跳出菜单.
//    if (event->button() == Qt::RightButton)
//    {
//        ContextMenu->exec(event->globalPos());
//    }
//    //要继续保留QListWidget原有的点击事件.
//    QListWidget::mousePressEvent(event);
//}
