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

ItemWidget::ItemWidget(Item item, QWidget *parent): QWidget(parent)
{

    this->item = item;
    layoutWidget  =new QWidget(this);
//    layoutWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout = new QVBoxLayout(layoutWidget);

    icon = new QLabel(this);
//    icon->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    label = new QLabel(this);
    label->setWordWrap(true);
//    label->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);

//    icon->setGeometry(0,0,50,50);
//    layout->setAlignment(Qt::AlignHCenter);

    layout->addWidget(icon,0,Qt::AlignHCenter);
    layout->addWidget(label,0,Qt::AlignHCenter);

    this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
//    menubar->setObjectName(QString::fromUtf8("menubar"));

//    layoutWidget->setObjectName(QString::fromUtf8("ItemWidget"));

    this->setLayout(layout);
    QPixmap ico = getFileIcon(item.path.toStdString());
//    QFileInfo info(item.path);

//    ico.save("D:/"+info.baseName()+".png");

    setIcon(ico);
    setText(item.label);
    setContextMenuPolicy(Qt::CustomContextMenu);

    menu = new QMenu(this);
    QAction* dAction = new QAction();
    dAction->setText("删除");


    menu->addAction(dAction);
qDebug()<<"item,"<<item.label;

//    connect(menu, SIGNAL(triggered(QAction *)),this, SLOT(deleteActionSlot(QAction *)));

    connect(menu, &QMenu::triggered,this,[&](){
      //  qDebug()<<"删除menu被触发"<<this->item.label;
       emit this->deleteAction(this->item);

    });

}

void ItemWidget::setIcon(QPixmap icon){
    qDebug()<<"icon-w:"<<icon.width();
    qDebug()<<"icon-h"<<icon.height();
    qDebug()<<"icon size ="<<this->icon->size().width();
    qDebug()<<"icon size ="<<this->icon->size().height();
//    icon = icon.scaled(this->icon->size().width(), this->icon->size().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    qDebug()<<"缩放后icon-w:"<<icon.width();
    qDebug()<<"缩放后icon-h"<<icon.height();
    qDebug()<<"缩放后icon size ="<<this->icon->size().width();
    qDebug()<<"缩放后icon size ="<<this->icon->size().height();
//    this->icon->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    this->icon->setAlignment(Qt::AlignCenter);
    this->icon->setPixmap(icon);

}

void ItemWidget::setText(QString text){

    this->label->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    this->label->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setPointSize(16);

    this->setToolTip(text);

    QFontMetrics fontWidth(font);//得到每个字符的宽度
    text = fontWidth.elidedText(text, Qt::ElideRight, 70);//最大宽度170px
    this->label->setText(text);//显示省略号的字符串

    this->label->setFont(font);

}

// 实现方法以使用qss样式
void ItemWidget::paintEvent(QPaintEvent *event){

QStyleOption opt;
opt.init(this);
    QPainter painter(this);

    style()->drawPrimitive(QStyle::PE_Widget,&opt,&painter,this);

//    qDebug()<<"启动绘制："<<this->geometry();


//    painter.setPen(Qt::red);
//QRect r = this->geometry();
//r.setX(0);
//r.setY(0);
//QBrush brush(Qt::red);


//    painter.fillRect(r,brush);

}

// 鼠标点击事件

void ItemWidget::mouseReleaseEvent(QMouseEvent *event){
    qDebug()<<"鼠标点击了:"<<item.path;

    if(event->button() ==Qt::RightButton){//右键弹出popMenu
        menu->exec(QCursor::pos());
        return;
    }

    QFileInfo info(item.path);


    bool needConsole=false;



    QString command;


    if(item.path.endsWith(".bat")){
        if(item.needConsole==0||item.needConsole==1){// 输出控制台
            needConsole = true;
            command = "start cmd /k \"%1 && exit\"";
        }else if(item.needConsole==2){//bat不输出控制台
            command = "%1";
        }

    }else{
        if(item.needConsole==0 ||item.needConsole==2){//非bat默认不输出控制台
            command = "%1";
        }else if(item.needConsole==1){//输出控制台
            command = "start %1";
            needConsole = true;
        }
    }


    command = command.arg(item.path);

    qDebug()<<"执行的命令:"<<command;

//    if(needConsole){//需要控制台
//        system((command).toUtf8().data());

//    }else{
//                QProcess*po = new QProcess;
//                po->start(command);
//    }


    event->ignore();



//    if(info.isExecutable()){// 其余可执行文件不需要控制台
//        qDebug()<<"可执行";
//        system((command).toUtf8().data());

////         system(item.path.toUtf8().data());

////        QProcess*po = new QProcess;
////        po->start(item.path);
//        emit clicked(item);
////        QString s = "start "+item.path+" \n";
////        po->write(s.toUtf8());
//    }else{
//        qDebug()<<"不可执行:"<<item.path;
//    }




}
// 空实现，只为屏蔽事件传递
void ItemWidget::mouseDoubleClickEvent(QMouseEvent *event){

}

ItemWidget::~ItemWidget(){
    delete layout;
    delete layoutWidget;
    delete icon;
    delete label;
    delete menu;
}



