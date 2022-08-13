#include "icon.h"
#include <windows.h>
#include <shellapi.h>
#include <winuser.h>
#include <commctrl.h>
#include <initguid.h>
#include <commoncontrols.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <QtWin>



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

