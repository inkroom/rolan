//
// Created by inkbox on 2022/8/11.
//
#include "icon.h"
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QTextStream>
//#include <filesystem>
#include <QFileInfo>
#include <QDir>

bool fileExist(QString path) {
    return QFileInfo::exists(path);
// return std::filesystem::exists(path.toStdString()); // 需要在cmake中 target_link_libraries(rolan -lstdc++fs)
}

/**
 * 按照linux的规则，查找logo的绝对路径
 * @param icon
 * @return
 */
QString getIconAbsoluteFile(QString icon) {

    if (fileExist(icon)) {
        return icon;
    }

    QFile file;
    QDir dir("/usr/share/icons/");
    dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::Readable);
    QFileInfoList list = dir.entryInfoList();
    for (const auto &item: list) {
        // 继续遍历
        qDebug() << "item=" << item.absoluteFilePath();
        auto size = QDir(item.absoluteFilePath()).entryInfoList(QDir::Dirs | QDir::NoSymLinks | QDir::Readable);
        for (const auto &sizeItem: size) {
            if (fileExist(sizeItem.absoluteFilePath() + "/apps/" + icon + ".png")) {
                return sizeItem.absoluteFilePath() + "/apps/" + icon + ".png";
            } else if (fileExist(sizeItem.absoluteFilePath() + "/actions/" + icon + ".png")) {
                return sizeItem.absoluteFilePath() + "/actions/" + icon + ".png";
            } else if (fileExist(sizeItem.absoluteFilePath() + "/apps/" + icon + ".symbolic.png")) {
                return sizeItem.absoluteFilePath() + "/apps/" + icon + ".symbolic.png";
            } else if (fileExist(sizeItem.absoluteFilePath() + "/actions/" + icon + ".symbolic.png")) {
                return sizeItem.absoluteFilePath() + "/actions/" + icon + ".symbolic.png";
            }
//            qDebug()<<"path="<<sizeItem.absoluteFilePath()+"/apps/"+icon<<" 文件存在=<<"<<fileExist(sizeItem.absoluteFilePath()+"/apps/"+icon+".png");
//            qDebug()<<"path="<<sizeItem.absoluteFilePath()+"/actions/"+icon<<" 文件存在=<<"<<fileExist(sizeItem.absoluteFilePath()+"/actions/"+icon+".png");
//            qDebug()<<"path="<<sizeItem.absoluteFilePath()+"/apps/"+icon<<" 文件存在=<<"<<fileExist(sizeItem.absoluteFilePath()+"/apps/"+icon+".symbolic.png");
//            qDebug()<<"path="<<sizeItem.absoluteFilePath()+"/actions/"+icon<<" 文件存在=<<"<<fileExist(sizeItem.absoluteFilePath()+"/actions/"+icon+".symbolic.png");

        }
    }


//    QString paths[] = {"hicolor/1024x1024", "hicolor/512x512", "hicolor/256x256", "hicolor/192x192", "hicolor/128x128",
//                       "hicolor/96x96", "hicolor/72x72", "hicolor/64x64", "hicolor/48x48", "hicolor/44x44",
//                       "hicolor/36x36", "hicolor/32x32", "hicolor/28x28", "hicolor/24x24", "hicolor/22x22",
//                       "hicolor/16x16", "hicolor/14x14", "locolor/32x32", "locolor/16x16"};
//
//    int size = sizeof(paths) / sizeof(paths[0]);
//    qDebug() << "数组长度=<<" << size;
//    for (int i = 0; i < size - 1; i++) {
//        QString f = "/usr/share/icons/" + paths[i] + "/apps/" + icon + ".png";
//        qDebug() << "路径:" << f;
//        if (fileExist(f)) {
//            return f;
//        }
//    }
    qDebug() << "没有logo文件" << icon.data();

    return {};
}


QPixmap getFileIcon(std::string path) {

    qDebug() << "linux 引入文件=" << path.data();
    QFile file(path.data());
    if (file.open(QFile::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith("Icon=")) {//
                qDebug() << "line = [" << line << "]";
                line = line.remove(0, 5);
                qDebug() << "文件=" << path.data() << "的Icon=[" << line << "]";

                QString r = getIconAbsoluteFile(line);
                qDebug() << "logo r= " << r;
                if (r.isEmpty() || r.isNull()) {
                    qDebug() << "没有logo文件" << path.data();
                    return QPixmap(":/img/200.png").scaled(64, 64);
                } else {
                    return QPixmap(r).scaled(64, 64);
                }
            }
        }

        file.close();

    } else{
        return QPixmap(":/img/200.png").scaled(64, 64);
    }
}