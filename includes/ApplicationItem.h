#ifndef APPLICATIONITEM_H
#define APPLICATIONITEM_H

#include <QString>
#include <QPixmap>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMenu>

typedef struct Item{

    QString path;
    QString label;
    /**
     * @brief needConsole 是否需要控制台；0为未设置，即按照默认情况决定，1代表需要，2不需要
     */
    unsigned short needConsole;
    /**
     * @brief 索引，用于数据管理
     */
    unsigned short index;
    /**
     * @brief 类型，0代表可执行文件，1网页url
     */
    unsigned short type;
}  Item;

class ItemWidget : public QWidget
{
    Q_OBJECT

    Item item;
    QVBoxLayout * layout;
    QWidget * layoutWidget;
    QLabel * icon;
    QLabel * label;

    QMenu* menu;

public:
    explicit ItemWidget(Item item, QWidget *parent = nullptr);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);// 必须提供一个空实现，避免事件传递给父组件
    void setIcon(QPixmap pixmap);
    void setText(QString text);
    ~ItemWidget();
protected:
    void paintEvent(QPaintEvent *event);
signals:
    void clickAction(Item item);
    void deleteAction(Item item);

};

#endif // APPLICATIONITEM_H
