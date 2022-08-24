#include <QApplication>
#include <QGridLayout>
#include <QHotkey>
#include "entrywindow.h"
#include <QDir>
#include <QLoggingCategory>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QSettings>
#include <QSystemSemaphore>
#include <QSharedMemory>

void single(){
    // 使用共享内存，当第二个进程启动时，判断内存区数据是否建立，如有。则退出；
    // 这种方式有弊端，在程序发生崩溃时，未及时清除共享区数据，导致程序不能正常启动。

    // 信号量的意义，把操作共享内存的代码锁住。因为有可能同时点击2次APP, 防止并发
    QSystemSemaphore sema("SingleApp Key", 1, QSystemSemaphore::Open);
    sema.acquire();

#ifdef Q_OS_LINUX
    /*  Windows平台上不存在应用程序崩溃后，共享内存段还存在的情况
     *  LINUX应用程序崩溃后,共享内存段不会自动销毁,则该程序再次运行会出问题
     *  所以程序启动时先去检查是否有程序崩溃后还存留的共享内存段，如果有，先销毁,再创建
     */
    QSharedMemory mem("SingleApp");
    // 尝试将进程附加到共享内存段
    if (mem.attach())
    {
        // 将共享内存与主进程分离, 如果此进程是附加到共享存储器段的最后一个进程，则系统释放共享存储器段，即销毁内容
        mem.detach();
    }
#endif

    /*
     * 每个App打开的时候，获取一次共享内存。
     * 如果获取失败，说明是第一个启动的APP，直接创建共享内存就好了。
     * 如果获取成功，说明不是第一个，直接退出就好了。
     * 保证App在系统里只能打开一个。
     */
    QSharedMemory unimem("SingleApp");
    bool isRunning = false;
    if (unimem.attach())
        isRunning = true;
    else
    {
        unimem.create(1);
        isRunning = false;
    }
    sema.release();

    if (isRunning)
    {
        QMessageBox::warning(nullptr, "warning", "程序已运行");
        exit(0);
    }
}

int main(int argc, char *argv[]) {

    single();

    QApplication a(argc, argv);

    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
    qDebug() << QDir::currentPath();

    QSettings settings(QDir::currentPath()+"/"+"setting.ini",QSettings::IniFormat);


    QVariant h = settings.value("hotkey/show");

    if (h.isNull()){
        settings.beginGroup("hotkey");
        settings.setValue("show","Alt+C");
        settings.endGroup();
    }


    EntryWindow w(new CustomTitleBar());
    w.setWindowTitle("Rolan");
    w.show();
    w.activateWindow();


    // 移到主屏幕中央
    QScreen *d = QGuiApplication::primaryScreen();
    QPoint point(d->size().width() / 2 - w.width() / 2, d->size().height() / 2 - w.height() / 2);
    w.move(point);

    QHotkey hotkey(QKeySequence(settings.value("hotkey/show").toString()), true, &a);
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
