## 系统支持平台

1. 硬件环境
   1. CPU
   2. 内存：4G以上内存
2. 软件环境
   1. 操作系统：Windows 10.
   2. C++版本： c++17.



## 集成环境与动态库文件

| 工具   | 版本   | 组织        | 链接                     |
| ------ | ------ | ----------- | ------------------------ |
| QT 6   | 6.2.2  | Qt Company  | https://www.qt.io/zh-cn/ |
| FFmpeg | 4.3.2  | FFmpeg team | http://ffmpeg.org/       |
| SDL2   | 2.0.20 | GNU LGPL 2  | http://www.libsdl.org/   |



## 下载使用



从Audivier项目https://github.com/Perter-Li/Audivier下载项目基础代码

```
git clone https://github.com/Perter-Li/Audivier.git
```



项目树如下

```
.
|-- Audivier.ico
|-- Audivier.pro
|-- Audivier.pro.user
|-- Audivier_resource.rc
|-- Executable
|   |-- Audivier.exe
|   `-- Audivier_Win64.zip
|-- Makefile
|-- Makefile.Debug
|-- Makefile.Release
|-- README.md
|-- SDL2
|-- changebuttoncolor.cpp
|-- changebuttoncolor.h
|-- debug
|   |-- Audivier.exe
|   |-- Audivier_resource_res.o
|   |-- SDL2.dll
|   |-- SDL2.lib
|   |-- SDL2main.lib
|   |-- SDL2test.lib
|   |-- avcodec-58.dll
|   |-- avdevice-58.dll
|   |-- avfilter-7.dll
|   |-- avformat-58.dll
|   |-- avutil-56.dll
|   |-- changebuttoncolor.o
|   |-- ffmpeg.exe
|   |-- ffplay.exe
|   |-- ffprobe.exe
|   |-- getframe.o
|   |-- main.o
|   |-- mainwindow.o
|   |-- media.o
|   |-- moc_changebuttoncolor.cpp
|   |-- moc_changebuttoncolor.o
|   |-- moc_getframe.cpp
|   |-- moc_getframe.o
|   |-- moc_mainwindow.cpp
|   |-- moc_mainwindow.o
|   |-- moc_mylistwidget.cpp
|   |-- moc_mylistwidget.o
|   |-- moc_playerslider.cpp
|   |-- moc_playerslider.o
|   |-- moc_predefs.h
|   |-- moc_setvolume.cpp
|   |-- moc_setvolume.o
|   |-- moc_setwindow.cpp
|   |-- moc_setwindow.o
|   |-- mylistwidget.o
|   |-- playerslider.o
|   |-- postproc-55.dll
|   |-- qrc_images.cpp
|   |-- qrc_images.o
|   |-- setvolume.o
|   |-- setwindow.o
|   |-- swresample-3.dll
|   `-- swscale-5.dll
|-- ffmpeg
|-- getframe.cpp
|-- getframe.h
|-- getframe.ui
|-- images
|   |-- 1x.png
|   |-- 2x.png
|   |-- 4x.png
|   |-- FiveFrame.png
|   |-- FullScreen.png
|   |-- Information.png
|   |-- Last.png
|   |-- MusicAlbums.png
|   |-- Mute.png
|   |-- Next.png
|   |-- OneFrame.png
|   |-- Play.png
|   |-- PlayInOrder.png
|   |-- PlayList.png
|   |-- PlayRandom.png
|   |-- PlaySingle.png
|   |-- PlaySingleCycle.png
|   |-- Plus.png
|   |-- Set.png
|   |-- SmallScreen.png
|   |-- Stop.png
|   |-- Voice.png
|   |-- logo.jpg
|   `-- style.qss
|-- images.qrc
|-- listrightclickmenu.cpp
|-- listrightclickmenu.h
|-- main.cpp
|-- mainwindow.cpp
|-- mainwindow.h
|-- mainwindow.ui
|-- media.cpp
|-- mylistwidget.cpp
|-- mylistwidget.h
|-- object_script.Audivier.Debug
|-- object_script.Audivier.Release
|-- playerslider.cpp
|-- playerslider.h
|-- release
|   |-- Audivier.exe
|   |-- Audivier_resource_res.o
|   |-- SDL2.dll
|   |-- SDL2.lib
|   |-- SDL2main.lib
|   |-- SDL2test.lib
|   |-- avcodec-58.dll
|   |-- avdevice-58.dll
|   |-- avfilter-7.dll
|   |-- avformat-58.dll
|   |-- avutil-56.dll
|   |-- changebuttoncolor.o
|   |-- ffmpeg.exe
|   |-- ffplay.exe
|   |-- ffprobe.exe
|   |-- getframe.o
|   |-- main.o
|   |-- mainwindow.o
|   |-- media.o
|   |-- moc_changebuttoncolor.cpp
|   |-- moc_changebuttoncolor.o
|   |-- moc_getframe.cpp
|   |-- moc_getframe.o
|   |-- moc_mainwindow.cpp
|   |-- moc_mainwindow.o
|   |-- moc_mylistwidget.cpp
|   |-- moc_mylistwidget.o
|   |-- moc_playerslider.cpp
|   |-- moc_playerslider.o
|   |-- moc_predefs.h
|   |-- moc_setvolume.cpp
|   |-- moc_setvolume.o
|   |-- moc_setwindow.cpp
|   |-- moc_setwindow.o
|   |-- mylistwidget.o
|   |-- playerslider.o
|   |-- postproc-55.dll
|   |-- qrc_images.cpp
|   |-- qrc_images.o
|   |-- setvolume.o
|   |-- setwindow.o
|   |-- swresample-3.dll
|   `-- swscale-5.dll
|-- res
|   |-- Interface.png
|   |-- pic
|   `-- project.png
|-- setvolume.cpp
|-- setvolume.h
|-- setvolume.ui
|-- setwindow.cpp
|-- setwindow.h
`-- setwindow.ui

```



Executable文件中包含着项目可执行文件，其中Audivier.exe可直接运行程序，Audivier_Win64.zip中为项目安装包，可用于选择下载路径以及添加注册表之类的信息。





其余部分的为项目的源代码，若想要进行执行，应该提前下载好QT开发环境，并打开Audivier.pro文件。打开成功以后，需要设定一下生成的项目文件路径，否则会出现无法找到动态库的问题。其设置如下，用QT打开本项目需要将项目生成路径设定在*.pro文件同级目录下。其余路保持不变即可，此时便可以正常运行程序代码。

![](/res/pic/project.png)



如果出现项目无法找到动态库文件的问题，可以将动态库重新自定义添加至项目生成文件，这里主要涉及到两个动态库，ffmpeg和SDL

将动态库文件均进行添加即可。



ffmpeg

```
Audivier/ffmpeg/bin

avcodec-58.dll   avformat-58.dll  ffplay.exe       swresample-3.dll
avdevice-58.dll  avutil-56.dll    ffprobe.exe      swscale-5.dll
avfilter-7.dll   ffmpeg.exe       postproc-55.dll

```

SDL

```
Audivier/SDL2/lib/x64

SDL2.dll  SDL2.lib  SDL2main.lib  SDL2test.lib
```



## 程序执行界面

当项目执行成功，则会出现如下界面：

![](/res/pic/Interface.png)



相关的功能运行，可参考功能演示视频，位于/res/功能演示.mkv 中。



注：

当前程序无法识别包含中文路径的音视频文件，因此在导入文件时，注意不进行中文的导入即可。
