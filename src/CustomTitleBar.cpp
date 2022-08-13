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
    delete layout;
}
