#include "ApplicationItem.h"
#include <QWidget>
#include<QDebug>
#include <QProcess>
#include <QPainter>
#include <QStyleOption>
#include <QFileInfo>
#include "icon.h"
#include <stdlib.h>
#include <QMouseEvent>
// itemWidget

ItemWidget::ItemWidget(Item item, QWidget *parent) : QWidget(parent) {

    this->item = item;
    layoutWidget = new QWidget(this);
//    layoutWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout = new QVBoxLayout(layoutWidget);

    icon = new QLabel(this);
//    icon->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    label = new QLabel(this);
    label->setWordWrap(true);
//    label->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);

//    icon->setGeometry(0,0,50,50);
//    layout->setAlignment(Qt::AlignHCenter);

    layout->addWidget(icon, 0, Qt::AlignHCenter);
    layout->addWidget(label, 0, Qt::AlignHCenter);

    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
//    menubar->setObjectName(QString::fromUtf8("menubar"));

//    layoutWidget->setObjectName(QString::fromUtf8("ItemWidget"));

    this->setLayout(layout);
    if (item.icon.isNull() || item.icon.isEmpty() || !QFileInfo::exists(item.icon)){
        QPixmap ico = getFileIcon(item.path.toStdString());
//    QFileInfo info(item.path);

//    ico.save("D:/"+info.baseName()+".png");

        setIcon(ico);
    } else{
        setIcon(QPixmap(item.icon).scaled(64, 64));
    }

    setText(item.label);
    setContextMenuPolicy(Qt::CustomContextMenu);

    menu = new QMenu(this);
    QAction *dAction = new QAction();
    dAction->setText("删除");


    menu->addAction(dAction);
    qDebug() << "item," << item.label;

//    connect(menu, SIGNAL(triggered(QAction *)),this, SLOT(deleteActionSlot(QAction *)));

    connect(menu, &QMenu::triggered, this, [&]() {
        //  qDebug()<<"删除menu被触发"<<this->item.label;
        emit this->deleteAction(this->item);

    });

}

void ItemWidget::setIcon(QPixmap icon) {
    qDebug() << "icon-w:" << icon.width();
    qDebug() << "icon-h" << icon.height();
    qDebug() << "icon size =" << this->icon->size().width();
    qDebug() << "icon size =" << this->icon->size().height();
//    icon = icon.scaled(this->icon->size().width(), this->icon->size().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    qDebug() << "缩放后icon-w:" << icon.width();
    qDebug() << "缩放后icon-h" << icon.height();
    qDebug() << "缩放后icon size =" << this->icon->size().width();
    qDebug() << "缩放后icon size =" << this->icon->size().height();
//    this->icon->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    this->icon->setAlignment(Qt::AlignCenter);
    this->icon->setPixmap(icon);

}

QString elidedLineText(QWidget *pWidget, int nLine, QString strText) {
    if (nLine == 0)
        return "";

    QFontMetrics fontMetrics(pWidget->font());

    if (nLine == 1) {
        return fontMetrics.elidedText(strText, Qt::ElideRight, pWidget->width());
    }

    QStringList strListLine;

    for (int i = 0; i < strText.size(); i++) {
        if (fontMetrics.width(strText.left(i)) >= pWidget->width()) {
            strListLine.append(strText.left(i));
            if (strListLine.size() == nLine) {
                break;
            }
            strText = strText.right(strText.size() - i);
            i = 0;
        }
    }

    if (strListLine.size() < nLine) {
        if (!strText.isEmpty()) {
            strListLine.append(strText);
        }
    }

    bool bHasElided = true;
    if (fontMetrics.width(strText) < pWidget->width()) {
        bHasElided = false;
    }

    if (bHasElided && !strListLine.isEmpty()) {
        QString strLast = strListLine.last();
        QString strElided = "...";
        strLast.insert(strLast.length(), strElided);
        while (fontMetrics.width(strLast) >= pWidget->width()) {
            strLast = strLast.replace(0, 1, "");
        }

        strListLine.replace(strListLine.count() - 1, strLast);
    }
    QString strResult = strListLine.join("\n");

    return strResult;
}


void ItemWidget::setText(QString text) {

    this->label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    this->label->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setPointSize(16);

    this->setToolTip(text);

    QFontMetrics fontWidth(font);//得到每个字符的宽度
//    text = fontWidth.elidedText(text, Qt::ElideRight, 70);//最大宽度170px
//text.replace(QChar(' '),QChar('\n'));
//this->label->adjustSize();
    text = elidedLineText(label, 2, text);
    this->label->setText(text);//显示省略号的字符串

    this->label->setFont(font);

}

// 实现方法以使用qss样式
void ItemWidget::paintEvent(QPaintEvent *event) {

    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);

    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

//    qDebug()<<"启动绘制："<<this->geometry();


//    painter.setPen(Qt::red);
//QRect r = this->geometry();
//r.setX(0);
//r.setY(0);
//QBrush brush(Qt::red);


//    painter.fillRect(r,brush);

}


// 空实现，只为屏蔽事件传递
void ItemWidget::mouseDoubleClickEvent(QMouseEvent *event) {

}

ItemWidget::~ItemWidget() {
    delete layout;
    delete layoutWidget;
    delete icon;
    delete label;
    delete menu;
}



