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
#include <QLabel>

#define CUSTOM_TITLE_HEIGHT 30
#define CUSTOM_TITLE_BUTTON_HEIGHT CUSTOM_TITLE_HEIGHT
#define CUSTOM_TITLE_BUTTON_WIDTH CUSTOM_TITLE_HEIGHT

class CustomTitleBar : public QWidget {

Q_OBJECT

private:
    QHBoxLayout *layout;

    QLabel *title;

    QPushButton *quitButton;

public:
    CustomTitleBar(QWidget *parent = nullptr);

    ~CustomTitleBar();


    void paintEvent(QPaintEvent *event);

protected:
    //设置界面标题与图标
    virtual bool eventFilter(QObject *obj, QEvent *event);

signals:
    void quit();
};


#endif //ROLAN_CUSTOMTITLEBAR_H
