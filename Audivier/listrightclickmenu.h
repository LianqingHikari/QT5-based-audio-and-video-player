#ifndef QMYLISTWIDGET_H
#define QMYLISTWIDGET_H
#include <QListWidget>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QMouseEvent>

#include <QListWidget>

class ListRightClickMenu : public QListWidget
{
    Q_OBJECT
public:
    explicit ListRightClickMenu(QWidget *parent = nullptr);
    ~ListRightClickMenu();
protected:
    void mousePressEvent(QMouseEvent *event);
private:
    QMenu *m_contextMenu;
    QAction *m_addAction;
    QAction *m_delAction;
signals:

};

#endif // LISTRIGHTCLICKMENU_H
