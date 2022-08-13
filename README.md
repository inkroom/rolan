# rolan
基于QT的程序启动器


## linux打包
```shell
sudo apt pathchelf
sudo wget -c "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage" -O /usr/local/bin/appimagetool
sudo chmod +x /usr/local/bin/appimagetool

mkdir build 
cd build
cp ../cmake-build-release/Rolan ./
linuxdeployqt Rolan -appimage

```
