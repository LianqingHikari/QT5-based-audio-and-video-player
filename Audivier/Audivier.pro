QT       += core gui
QT       += multimedia
QT += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    changebuttoncolor.cpp \
    getframe.cpp \
    main.cpp \
    mainwindow.cpp \
    media.cpp \
    mylistwidget.cpp \
    playerslider.cpp \
    setvolume.cpp \
    setwindow.cpp

HEADERS += \
    changebuttoncolor.h \
    getframe.h \
    mainwindow.h \
    mylistwidget.h \
    playerslider.h \
    setvolume.h \
    setwindow.h

FORMS += \
    getframe.ui \
    mainwindow.ui \
    setvolume.ui \
    setwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/ffmpeg/include

LIBS += $$PWD/ffmpeg/lib/avcodec.lib \
        $$PWD/ffmpeg/lib/avdevice.lib \
        $$PWD/ffmpeg/lib/avfilter.lib \
        $$PWD/ffmpeg/lib/avformat.lib  \
        $$PWD/ffmpeg/lib/avutil.lib   \
        $$PWD/ffmpeg/lib/postproc.lib  \
        $$PWD/ffmpeg/lib/swresample.lib  \
        $$PWD/ffmpeg/lib/swscale.lib

RESOURCES += \
    images.qrc
RC_ICONS = Audivier.ico
