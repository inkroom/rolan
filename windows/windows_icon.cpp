#include "icon.h"
#include <windows.h>
#include <initguid.h>
#include <shellapi.h>
#include <winuser.h>
#include <commctrl.h>
#include <commoncontrols.h>
#include <QtWin>
#include "ApplicationItem.h"
#include <QDebug>
#include <QFileInfo>
#include <QProcess>
#include <QMouseEvent>
// 鼠标点击事件

void ItemWidget::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << "鼠标点击了:" << item.path;

    if (event->button() == Qt::RightButton) {//右键弹出popMenu
        menu->exec(QCursor::pos());
        return;
    }

    if (item.path == 1) {//打开网页
        system(("explorer " + item.path).toStdString().c_str());
        emit clickAction(item);
        return;
    }

    QFileInfo info(item.path);


    bool needConsole = false;


    QString command;


    if (item.path.endsWith(".bat")) {
        if (item.needConsole == 0 || item.needConsole == 1) {// 输出控制台
            needConsole = true;
            command = "start cmd /k \"%1 && exit\"";
        } else if (item.needConsole == 2) {//bat不输出控制台
            command = "%1";
        }

    } else {
        if (item.needConsole == 0 || item.needConsole == 2) {//非bat默认不输出控制台
            command = "%1";
        } else if (item.needConsole == 1) {//输出控制台
            command = "start %1";
            needConsole = true;
        }
    }


    command = command.arg(item.path);

    qDebug() << "执行的命令:" << command;

    if (needConsole) {//需要控制台
        system((command).toUtf8().data());

    } else {
        QProcess *po = new QProcess;
        po->start(command);
    }


    event->ignore();



//    if(info.isExecutable()){// 其余可执行文件不需要控制台
//        qDebug()<<"可执行";
//        system((command).toUtf8().data());

////         system(item.path.toUtf8().data());

////        QProcess*po = new QProcess;
////        po->start(item.path);
//        emit clicked(item);
////        QString s = "start "+item.path+" \n";
////        po->write(s.toUtf8());
//    }else{
//        qDebug()<<"不可执行:"<<item.path;
//    }




}

// 获取文件图标
HICON fileIcon(std::string extention,int* iIcon)
{
    HICON icon = NULL;
    if (extention.length() > 0)
    {
        LPCSTR name = extention.c_str();

        SHFILEINFOA info;
        // // 除了SHGFI_ICON之外还有SHGFI_LARGEICON(大图标), SHGFI_SMALLICON(小图标)
        if (SHGetFileInfoA(name,
            FILE_ATTRIBUTE_NORMAL,
            &info,
            sizeof(info),
            SHGFI_SYSICONINDEX | SHGFI_LARGEICON | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES))
        {
            icon = info.hIcon;
            *iIcon = info.iIcon;
        }
    }



    return icon;
}

// 获取文件类型
std::string fileType(std::string extention)
{

    std::string type = "";
    if (extention.length() > 0)
    {
        LPCSTR name = extention.c_str();

        SHFILEINFOA info;
        if (SHGetFileInfoA(name,
            FILE_ATTRIBUTE_NORMAL,
            &info,
            sizeof(info),
            SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES))
        {
            type = info.szTypeName;
        }
    }

    return type;
}

// 获取文件夹图标
HICON folderIcon()
{
    std::string str = "folder";
    LPCSTR name = str.c_str();

    HICON icon = NULL;

    SHFILEINFOA info;
    if (SHGetFileInfoA(name,
        FILE_ATTRIBUTE_DIRECTORY,
        &info,
        sizeof(info),
        SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES))
    {
        icon = info.hIcon;
    }

    return icon;
}

// 获取文件夹类型
std::string folderType()
{
    std::string str = "folder";
    LPCSTR name = str.c_str();

    std::string type;

    SHFILEINFOA info;
    if (SHGetFileInfoA(name,
        FILE_ATTRIBUTE_DIRECTORY,
        &info,
        sizeof(info),
        SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES))
    {
        type = info.szTypeName;
    }

    return type;
}

HICON getLargeIcon(int index){
        IImageList* g_hImageList256 = NULL;

        SHGetImageList(SHIL_EXTRALARGE, IID_IImageList, (void**)&g_hImageList256);


        HICON h;

        HRESULT res=   g_hImageList256->GetIcon(index,ILD_IMAGE,&h);
        if(res == S_OK){
            return  h;
        }else{
          return NULL;
        }

}



QPixmap getFileIcon(std::string path){


    int index=0;
    HICON h = fileIcon(path,&index);

    h = getLargeIcon(index);

    return QtWin::fromHICON(h);
}

