#ifndef LISTITEMMOVE_H
#define LISTITEMMOVE_H

#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QListWidget>

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListWidget(QWidget *parent = nullptr);
    ~MyListWidget();
    void dropEvent(QDropEvent *event);
    //void performDrag();

protected:
    //void mousePressEvent(QMouseEvent *event);
private:
    QMenu *ContextMenu;
    QAction *NextPlay;
    QAction *Information;
signals:
    void ItemDragSignal();

};

#endif // LISTITEMMOVE_H
