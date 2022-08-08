#ifndef APPLICATIONITEM_H
#define APPLICATIONITEM_H

#include <QString>
#include <QPixmap>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

typedef struct Item{

    QString path;
    QString label;
    /**
     * @brief needConsole 是否需要控制台；0为未设置，即按照默认情况决定，1代表需要，2不需要
     */
    unsigned short needConsole;
}  Item;

class ItemWidget : public QWidget
{
    Q_OBJECT

    Item item;
    QVBoxLayout * layout;
    QWidget * layoutWidget;
    QLabel * icon;
    QLabel * label;

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
    void clicked(Item item);

};

#endif // APPLICATIONITEM_H
