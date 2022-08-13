//
// Created by inkbox on 2022/8/13.
//
#include "CustomTitleBar.h"
#include <QMessageBox>
#include <qapplication.h>

CustomTitleBar::CustomTitleBar(QWidget *parent) : QWidget(parent) {

    quitButton = new QPushButton("exit");
    layout = new QHBoxLayout(this);

    layout->addStretch(1);

    layout->addWidget(quitButton);

    connect(quitButton, &QPushButton::clicked, this, [&]() {
        QMessageBox box;
        box.setText("退出程序");
        box.setInformativeText("确认退出？");
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setDefaultButton(QMessageBox::Ok);
        int r = box.exec();
        if (r == QMessageBox::Ok) {
            qApp->quit();
        }
    });


}

CustomTitleBar::~CustomTitleBar() noexcept {
    delete quitButton;
    delete layout;
}
