//
// Created by inkbox on 2022/8/13.
//
#include "CustomTitleBar.h"
#include <QMessageBox>
#include <qapplication.h>
#include <QPainter>
#include <QStyleOption>
#include <QFile>
#include <QIODevice>
#include <QDebug>


CustomTitleBar::CustomTitleBar(QWidget *parent) : QWidget(parent) {

    title = new QLabel;
    title->setAlignment(Qt::AlignCenter);
    quitButton = new QPushButton();

    quitButton->setFixedSize(CUSTOM_TITLE_BUTTON_WIDTH, CUSTOM_TITLE_HEIGHT);
    quitButton->setIcon(QIcon(":img/close"));
    quitButton->setFlat(true);
//    quitButton->setStyleSheet("background-color:transparent; ");
    quitButton->setIconSize(QSize(CUSTOM_TITLE_BUTTON_WIDTH, CUSTOM_TITLE_HEIGHT));

    layout = new QHBoxLayout(this);

    QFile qss(":/style/titleBar.qss");
    if (qss.open(QFile::ReadOnly)) {
        setStyleSheet(qss.readAll());
    } else {
        qDebug() << "qss读取失败";
    }

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addStretch(1);

    layout->addWidget(title);

    layout->addWidget(quitButton);

    connect(quitButton, &QPushButton::clicked, this, [&]() {
        QMessageBox box;
        box.setText("退出程序");
        box.setInformativeText("确认退出？");
        box.setWindowTitle("退出");
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setDefaultButton(QMessageBox::Ok);
        int r = box.exec();
        if (r == QMessageBox::Ok) {
            qApp->quit();
        }
    });

    setFixedHeight(CUSTOM_TITLE_HEIGHT);

}

void CustomTitleBar::paintEvent(QPaintEvent *event) {
//    QPainter painter(this);
//    painter.setBrush(Qt::blue);
//    painter.fillRect(this->geometry(),Qt::blue);

    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);

    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

}

CustomTitleBar::~CustomTitleBar() noexcept {
    delete quitButton;
    delete title;
    delete layout;
}

//使用事件过滤器监听标题栏所在的窗体，所以当窗体标题、图标等信息发生改变时，标题栏也应该随之改变
bool CustomTitleBar::eventFilter(QObject *obj, QEvent *event) {
    switch (event->type()) //判断发生事件的类型
    {
        case QEvent::WindowTitleChange: //窗口标题改变事件
        {
            QWidget *pWidget = qobject_cast<QWidget *>(obj); //获得发生事件的窗口对象
            if (pWidget) {
                //窗体标题改变，则标题栏标题也随之改变
                title->setText(pWidget->windowTitle());
                return true;
            }
        }
            break;

        case QEvent::WindowIconChange: //窗口图标改变事件
        {
            QWidget *pWidget = qobject_cast<QWidget *>(obj);
            if (pWidget) {
                //窗体图标改变，则标题栏图标也随之改变
                QIcon icon = pWidget->windowIcon();
//                m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
                return true;
            }
        }
            break;

        case QEvent::Resize:
//            updateMaximize(); //最大化/还原
            return true;

        default:
            return QWidget::eventFilter(obj, event);
    }

    return QWidget::eventFilter(obj, event);
}