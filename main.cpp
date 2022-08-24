#include "ApplicationItem.h"
#include "icon.h"
#include <QApplication>
#include <QGridLayout>
#include <QFile>
#include <QVBoxLayout>
#include <QDebug>
#include <QHotkey>
#include "entrywindow.h"
#include <QDir>
#include <QFileInfo>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
    qDebug() << QDir::currentPath();



    EntryWindow w(new CustomTitleBar());
    w.setWindowTitle("Rolan");
    w.show();
    w.activateWindow();


    // 移到主屏幕中央
    QScreen *d = QGuiApplication::primaryScreen();
    QPoint point(d->size().width() / 2 - w.width() / 2, d->size().height() / 2 - w.height() / 2);
    w.move(point);

    QHotkey hotkey(QKeySequence("Alt+C"), true, &a);
    if(!hotkey.isRegistered()){
        QMessageBox::information(&w,"快捷键","快捷键注册失败，可能是快捷键冲突","确认");
    }


    QObject::connect(&hotkey, &QHotkey::activated, qApp, [&]() {
        if (w.isHidden()) {
            w.show();
            w.activateWindow();//linux 平台 show 之后不会认为窗口被激活，会导致下一次响应进入下一个分支，而不是响应hide
            qDebug()<<"hot key show";
        } else if(!w.isActiveWindow()){
            w.activateWindow();
            qDebug()<<"hot key active";
        } else {
            w.hide();
            qDebug()<<"hot key hidden";
        }
    });

    return a.exec();
}
