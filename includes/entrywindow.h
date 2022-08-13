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
class EntryWindow : public QMainWindow
{
    Q_OBJECT


    QWidget * center;


    QGridLayout * layout;

    QVBoxLayout * vlayout;

    QList<Item> list;

    QString filePath;

    QJsonArray data;

public:
    EntryWindow(QWidget *parent=nullptr);

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
private:
    /**
     * 将数据持久化到文件
     */
    void save();

};

#endif // ENTRYWINDOW_H
