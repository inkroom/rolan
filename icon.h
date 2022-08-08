#ifndef ICON_H
#define ICON_H

#include <QPixmap>

/**
 * @brief getFileIcon 根据文件获取图标图片
 * @param path 文件路径，绝对路径
 * @return  图片
 */
QPixmap getFileIcon(std::string path);

#endif // ICON_H
