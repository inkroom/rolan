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

EntryWindow::EntryWindow(CustomTitleBar *titleBar, QWidget *parent)
        : QMainWindow(parent) {

    center = new QWidget;


    layout = new QGridLayout();

    vlayout = new QVBoxLayout();
    vlayout->setContentsMargins(0, 0, 0, 0);

    if (titleBar) {
        titleBar->setParent(this);

        connect(titleBar,&CustomTitleBar::quit,this,&EntryWindow::hide);
        vlayout->addWidget(titleBar);
        this->setWindowFlags(Qt::Dialog | Qt::Tool | Qt::CustomizeWindowHint);
    }

    vlayout->addLayout(layout);
    vlayout->addStretch(1);

    center->setLayout(vlayout);
    //    center->setLayout(layout);
    QFile qss(":/style/style.qss");
    if (qss.open(QFile::ReadOnly)) {
        setStyleSheet(qss.readAll());
    } else {
        qDebug() << "qss读取失败";
    }

    resize(800, 600);

    setCentralWidget(center);

    setWindowIcon(QIcon(":/img/200.png"));

    setAcceptDrops(true);

    installEventFilter(this);


    // 读取配置文件
    filePath = QDir::currentPath() + "/data.json";

    QFile data(filePath);

    QFileInfo info(filePath);


    if (info.exists() && data.open(QIODevice::ReadWrite)) {
        QTextStream text(&data);
        text.setCodec("UTF-8");
        QString d = text.readAll();


        QJsonParseError error;


        QJsonDocument jsonDocument = QJsonDocument::fromJson(d.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError) {
            if (jsonDocument.isArray()) {
                this->data = jsonDocument.array();
                qDebug() << "array:" << this->data;
                qDebug() << "size:" << this->data.size();
                for (int i = 0; i < this->data.size(); i++) {
                    qDebug() << "i:" << i;

                    QVariantMap map = this->data.at(i).toVariant().toMap();

                    qDebug() << "json:" << map;

                    Item item;
                    item.path = map.take("path").toString();
                    item.label = map.take("label").toString();
                    item.needConsole = map.take("needConsole").toInt();
                    item.type = map.take("type").toInt();
                    item.icon = map.take("icon").toString();
                    item.index = i;

                    this->addItem(item);
                    list.append(item);
                }

            }
        }
    } else {
        this->data = QJsonArray();
    }


}

EntryWindow::~EntryWindow() {

    // 清理加入的组件


    delete center;
    delete layout;
    delete vlayout;
}


//拖拽事件

void EntryWindow::dragEnterEvent(QDragEnterEvent *event) {

    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    for (int i = 0; i < urls.size(); i++) {

        QString path = urls[i].toLocalFile();

        // 取文件名，不带后缀
        QFileInfo file(path);
#ifdef _WIN32
//        if(file.isExecutable()){//暂时只支持可执行文件
            event->acceptProposedAction();
//        }
#elif __linux__
        qDebug() << "linux 要拖进来的文件=" << file.baseName();

        if (file.suffix() == "desktop") {// linux 文件
            event->acceptProposedAction();
        }
#elif
#endif

    }


}

void EntryWindow::dropEvent(QDropEvent *event) {
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    for (int i = 0; i < urls.size(); i++) {

        QString path = urls[i].toLocalFile();

        // 取文件名，不带后缀
        QFileInfo file(path);
        qDebug() << file.baseName();
#ifdef _WIN32
        if( file.isExecutable()){
            Item item = {path,file.baseName(),0, static_cast<unsigned short>(list.size()+1),0};
            this->addItem(item);
            this->saveItem(item);
        }else{
            Item item = {path,file.baseName(),0, static_cast<unsigned short>(list.size()+1),-1};
            this->addItem(item);
            this->saveItem(item);
        }
#elif __linux__
        qDebug() << "linux 拖进来的文件=" << file.baseName();

        if (file.suffix() == "desktop") {// linux 文件
            Item item = {path, file.baseName(), 0,static_cast<unsigned short>(list.size()+1)};
            this->addItem(item);
            this->saveItem(item);
        }
#elif
#endif


    }

}

void EntryWindow::mouseDoubleClickEvent(QMouseEvent *event) {


    this->hide();

    qDebug() << "双击了 :" << event->isAccepted();
}

void EntryWindow::save() {
    // 持久化到文件
    QFile file(this->filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {

        QJsonDocument doc;
        doc.setArray(this->data);

        QTextStream d(&file);
        d.setCodec("UTF-8");
        d << doc.toJson(QJsonDocument::JsonFormat::Indented);
    }
}


void EntryWindow::addItem(Item item) {

    qDebug() << item.index << "  新增item<<" << item.path;
    ItemWidget *itemWidget = new ItemWidget(item);
    int i = list.size();


    qDebug() << "row=" << (i / 3) << "  column=" << (i - (i / 3) * 3) << " i=" << i;
    layout->addWidget(itemWidget, i / 3, i - (i / 3) * 3);

    // 监听点击事件
    connect(itemWidget, &ItemWidget::clickAction, this, [&](Item it) {
        this->hide();
    });

    // 监听删除事件
    connect(itemWidget, &ItemWidget::deleteAction, this, [&](Item it) {
        qDebug() << "要删除 " << it.label.isEmpty();
        qDebug() << "要删除 " << it.label;
        qDebug() << "要删除 " << it.index;


        qDebug() << "当前一共有<<" << QString::number(this->list.size()) << "  " << QString::number(this->data.size());
        // it.index不一定是在数组中的下标，所以要遍历删除
        for (int i = 0; i < this->list.size(); i++) {
            qDebug() << " i=" + QString::number(i) << " " << this->list.at(i).label << " it.label=" << it.label
                     << " index = " << this->list.at(i).index << " it.index=" << it.index;
            if (this->list.at(i).index == it.index) {
                qDebug() << "移除第" << i << "个元素 " << this->list.at(i).label;
                this->list.removeAt(i);
                this->data.removeAt(i);
                break;
            }
        }

        // 需要做一个拷贝，让list从0开始添加
        QList<Item> newList(this->list);
        this->list.clear();

        // 移除组件，只是移除某个组件会影响行列分布，所以要全部移除，然后重新添加

        QLayoutItem *layoutItem = nullptr;
        while ((layoutItem = layout->takeAt(0))) {
            if (layoutItem->widget()) {
                layoutItem->widget()->setParent(nullptr);
            }
            delete layoutItem;
        }
//
        for (const auto &i_item: newList) {
            addItem(i_item);
            this->list.append(i_item);//重新加入数据

        }


        this->save();

    });

//    connect(itemWidget, SIGNAL(deleteAction(Item)),this, SLOT(deleteItem(Item)));

}

void EntryWindow::saveItem(Item item) {
    list.append(item);

    QJsonObject value;
    value.insert("label", QJsonValue(item.label));
    value.insert("path", QJsonValue(item.path));
    value.insert("needConsole", QJsonValue(item.needConsole));
    value.insert("type", QJsonValue(item.type));
    value.insert("icon",QJsonValue(item.icon));
    this->data.append(value);
    this->save();
}


void EntryWindow::mousePressEvent(QMouseEvent *e) {
    last = e->globalPos();
}

void EntryWindow::mouseMoveEvent(QMouseEvent *e) {
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x() + dx, y() + dy);
}

void EntryWindow::mouseReleaseEvent(QMouseEvent *e) {
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x() + dx, y() + dy);
}
