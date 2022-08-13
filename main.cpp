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

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);
    qDebug() << QDir::currentPath();

    QFile data(QDir::currentPath() + "/data.json");

    QFileInfo info(QDir::currentPath() + "/data.json");


    if (info.exists() && data.open(QIODevice::ReadWrite)) {
        QTextStream text(&data);
        text.setCodec("UTF-8");
        QString d = text.readAll();


        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(d.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError) {

        }
    }


    EntryWindow w(new CustomTitleBar());
    w.show();

    // 移到主屏幕中央
    QDesktopWidget *desktop = qApp->desktop();
    QWidget *d = desktop->screen(desktop->primaryScreen());
    QPoint point(d->width() / 2 - w.width() / 2, d->height() / 2 - w.height() / 2);
    w.move(point);


    QHotkey hotkey(QKeySequence("Alt+C"), true, &a);
    QObject::connect(&hotkey, &QHotkey::activated, qApp, [&]() {
        if (w.isHidden()) {
            w.show();
        } else {
            w.hide();
        }
    });

    return a.exec();
}
