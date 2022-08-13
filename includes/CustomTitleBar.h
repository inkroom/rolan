//
// Created by inkbox on 2022/8/13.
//

#ifndef ROLAN_CUSTOMTITLEBAR_H
#define ROLAN_CUSTOMTITLEBAR_H
/**
 * 自定义的标题栏组件
 */

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

class CustomTitleBar : public QWidget{

    Q_OBJECT

private:
    QHBoxLayout* layout;

    QPushButton* quitButton;

public:
    CustomTitleBar(QWidget* parent= nullptr);
    ~CustomTitleBar();


};


#endif //ROLAN_CUSTOMTITLEBAR_H
