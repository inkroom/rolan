#include "entrywindow.h"
#include <QFile>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDir>
#include <QFileInfo>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

EntryWindow::EntryWindow(QWidget *parent)
    : QMainWindow(parent)
{

     center = new QWidget;


     layout = new QGridLayout();

     vlayout = new QVBoxLayout();




     vlayout->addLayout(layout);
     vlayout->addStretch(1);

     center->setLayout(vlayout);
 //    center->setLayout(layout);
     QFile qss(":/style/style.qss");
     if(qss.open(QFile::ReadOnly)){
         setStyleSheet(qss.readAll());
     }
     else{
         qDebug()<<"qss读取失败";
     }

     resize(800,600);

     setCentralWidget(center);

     setWindowIcon(QIcon(":/img/200.png"));

     setAcceptDrops(true);

     installEventFilter(this);


     // 读取配置文件
     filePath = QDir::currentPath()+"/data.json";

     QFile data(filePath);

     QFileInfo info(filePath);


     if(info.exists() && data.open(QIODevice::ReadWrite)){
         QTextStream text(&data);
         text.setCodec("UTF-8");
         QString d = text.readAll();


         QJsonParseError error;



         QJsonDocument jsonDocument = QJsonDocument::fromJson(d.toUtf8(), &error);
         if (error.error == QJsonParseError::NoError) {
          if(jsonDocument.isArray()){
             this->data = jsonDocument.array();
qDebug()<<"array:"<<this->data;
qDebug()<<"size:"<<this->data.size();
             for (int i =0 ;i<this->data.size();i++) {
                 qDebug()<<"i:"<<i;

               QVariantMap map=  this->data.at(i).toVariant().toMap();

               qDebug()<<"json:"<<map;

               Item item;
               item.path = map.take("path").toString();
               item.label = map.take("label").toString();
               item.needConsole = map.take("needConsole").toInt();

               this->addItem(item);
             }

          }
         }
     }else{
        this->data = QJsonArray();
     }


}
EntryWindow::~EntryWindow(){

    // 清理加入的组件


    delete center;
    delete layout;
    delete vlayout;
}


//拖拽事件

void EntryWindow::dragEnterEvent(QDragEnterEvent *event){

    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    for (int i=0;i<urls.size();i++) {

        QString path = urls[i].toLocalFile();

        // 取文件名，不带后缀
       QFileInfo file(path);
       if(file.isExecutable()){//暂时只支持可执行文件
           event->acceptProposedAction();
       }

    }


}
void EntryWindow::dropEvent(QDropEvent *event){
        QList<QUrl> urls = event->mimeData()->urls();
        if (urls.isEmpty()) {
            return;
        }

        for (int i=0;i<urls.size();i++) {

            QString path = urls[i].toLocalFile();

            // 取文件名，不带后缀
           QFileInfo file(path);
           qDebug()<<file.baseName();
            if(file.isExecutable()){
                Item item = {path,file.baseName(),0};
                this->addItem(item);
            }
        }

}

void EntryWindow::mouseDoubleClickEvent(QMouseEvent *event){


    this->hide();

    qDebug()<<"双击了 :" << event->isAccepted();
}


void EntryWindow::addItem(Item item){

    qDebug()<<"新增item<<"<<item.path;
    ItemWidget* itemWidget=new ItemWidget(item);
    int i = list.size() - 1;

    layout->addWidget(itemWidget,i / 3,i - ( i / 3) * 3 );
}

void EntryWindow::saveItem(Item item){
    list.append(item);

    QJsonObject value;
    value.insert("label",QJsonValue(item.label));
    value.insert("path",QJsonValue(item.path));
    value.insert("needConsole",QJsonValue(item.needConsole));

    this->data.append(value);
    // 持久化到文件
    QFile file(this->filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)){

        QJsonDocument doc;
        doc.setArray(this->data);

        QTextStream d(&file);
        d<<doc.toJson(QJsonDocument::JsonFormat::Indented);
    }
}
