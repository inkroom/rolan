#include "mainwindow.h"
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    qDebug()<<QDir::currentPath();

    QFile data(QDir::currentPath()+"/data.json");

    QFileInfo info(QDir::currentPath()+"/data.json");


    if(info.exists() && data.open(QIODevice::ReadWrite)){
        QTextStream text(&data);
        text.setCodec("UTF-8");
        QString d = text.readAll();


        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(d.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError) {

        }
    }




    EntryWindow w ;


    Item item = {"D:/software/apache-jmeter-5.5/bin/jmeter.bat","jmeter",0};
    w.addItem(item);

    for(int i = 0;i<10;i++){
        if(i%2==0){
            item = {"D:/software/apache-jmeter-5.5/bin/jmeter.bat","jmeter",0};
            w.addItem(item);
            w.saveItem(item);
        }else{
             item = {"D:/software/VLC/vlc.exe","vlc",0};
            w.addItem(item);
            w.saveItem(item);
        }

    }
    item = {"D:/software/apache-jmeter-5.5/bin/jmeter.bat","j-有",1};
    w.addItem(item);
    w.saveItem(item);
    item = {"D:/software/apache-jmeter-5.5/bin/jmeter.bat","j-无",2};
    w.addItem(item);
    w.saveItem(item);
    item = {"D:/software/VLC/vlc.exe","vlc-有",1};
   w.addItem(item);
   w.saveItem(item);
   item = {"D:/software/VLC/vlc.exe","vlc-无",2};
  w.addItem(item);
  w.saveItem(item);

    w.show();


//    QMainWindow w;


//    QWidget * center = new QWidget;


//    QGridLayout * layout = new QGridLayout();

//    QVBoxLayout * vlayout = new QVBoxLayout();



//     QList<Item> items;
//     Item i = {"D:/software/apache-jmeter-5.5/bin/jmeter.bat","jmeter"};
//    items.append(i);
//    i = {"D:/software/VLC/vlc.exe","vlc"};
//    items.append(i);
//    i = {"D:/software/apache-jmeter-5.5/bin/jmeter.bat","jmeter"};
//    items.append(i);
//    i = {"D:/software/VLC/vlc.exe","vlc"};
//    items.append(i);
//    i = {"D:/software/apache-jmeter-5.5/bin/jmeter.bat","jmeter"};
//    items.append(i);
//    i = {"D:/software/VLC/vlc.exe","vlc"};
//    items.append(i);
//    for (int i=0;i<items.size();i++) {

//        ItemWidget* itemWidget = new ItemWidget(items[i]);

//        itemWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);


//qDebug()<<itemWidget->metaObject()->className();
//        itemWidget->setIcon(getFileIcon(items[i].path.toStdString()));
//        itemWidget->setText(items[i].label);
////        layout->addWidget(itemWidget,i / 3,i - ( i / 3) * 3,Qt::AlignCenter );
//        layout->addWidget(itemWidget,i / 3,i - ( i / 3) * 3 );
////        layout->addWidget(itemWidget,i / 3,i - ( i / 3) * 3,0,0, Qt::AlignTop );

////        QObject::connect(itemWidget,&ItemWidget::clicked,&w,&QMainWindow::hide);

//    }


//    vlayout->addLayout(layout);
//    vlayout->addStretch(1);

//    center->setLayout(vlayout);
////    center->setLayout(layout);
//QFile qss(":/style/style.qss");
//if(qss.open(QFile::ReadOnly)){
//    w.setStyleSheet(qss.readAll());
//}
//else{
//    qDebug()<<"qss读取失败";
//   }

//    w.resize(800,600);

//    w.setCentralWidget(center);

//    w.setWindowIcon(QIcon(":/img/200.png"));
//    w.show();

QHotkey hotkey(QKeySequence("Ctrl+Alt+Q"), true, &a);
QObject::connect(&hotkey, &QHotkey::activated, qApp, [&](){
        qDebug() << "Hotkey Activated - the application will quit now";
        //qApp->quit();
        w.show();
    });

    return a.exec();
}
