#ifndef ENTRYWINDOW_H
#define ENTRYWINDOW_H

#include "ApplicationItem.h"
#include <QMainWindow>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QList>
#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDir>
#include <QFileInfo>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QTextStream>
#include <QJsonArray>
#include "CustomTitleBar.h"

class EntryWindow : public QMainWindow {
Q_OBJECT


    QWidget *center;


    QGridLayout *layout;

    QVBoxLayout *vlayout;

    QList<Item> list;

    QString filePath;

    QJsonArray data;
    /**
     * 实现窗口移动
     */
    QPoint last;
public:
    EntryWindow(CustomTitleBar *titleBar = nullptr, QWidget *parent = nullptr);

    void addItem(Item item);

    /**
     * @brief saveItem 保存item到文件
     * @param item
     */
    void saveItem(Item item);

    ~EntryWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void dropEvent(QDropEvent *event);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    /**
     * 将数据持久化到文件
     */
    void save();

};

#endif // ENTRYWINDOW_H
