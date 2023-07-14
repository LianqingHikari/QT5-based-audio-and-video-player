#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>
#include <QListWidget>
#include <QDebug>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVideoWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QFileDialog>
#include <QDir>
#include <mylistwidget.h>
#include <setwindow.h>
#include <setvolume.h>
#include <getframe.h>
#include <QTime>
#include <QVector>
#include <QRegExp>
#include <QMouseEvent>
#include <ui_getframe.h>
#include <ui_setvolume.h>
#include <cstdlib>
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <QPixmap>
#include <QPainter>


extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*Media有关的类声明*/
struct AudioInfoGuard {
    //格式化I/O上下文
    AVFormatContext *formatCtx = NULL;
    //解码器
    AVCodec *codec = NULL;
    //解码器上下文
    AVCodecContext *codecCtx = NULL;

    ~AudioInfoGuard() {
        if(codecCtx){
            avcodec_free_context(&codecCtx);
        }
        if(formatCtx){
            avformat_close_input(&formatCtx);
            avformat_free_context(formatCtx);
        }
    }
};

class MediaTime
{
public:
    MediaTime():sec((qint64(0))),min((qint64(0))),hrs((qint64(0))){};
    MediaTime(qint64 play_time);
    ~MediaTime(){};
    QString getTime();
    void setTime(qint64 play_time);
    qint64 getSec();

private:
    qint64 sec=0;
    qint64 min=0;
    qint64 hrs=0;
};


class Media
{
public:
    Media(const char* path);
    ~Media(){};
    QString GetPath();

protected:
    QString path;
    MediaTime duration;
    MediaTime current_T;
};

class Video : public Media
{
public:
    Video(const char* path);
    ~Video(){};
    QString getInfo();

    QString name;
    QString bit_rate;
    QString frame_rate;
    int nb_frames;
    QString codec;
    QString resolution;
};

class Audio : public Media
{
public:
    Audio(const char* path);
    ~Audio(){};
    QString getInfo();
    QImage getCover();

    QString name;
    QString bit_rate;
    int channel_count;
    QString codec;
    QString album;
    QString artist;
    QImage cover;
    QString lyricpath;
    QImage getWaveform();

};

class Lyric
{
public:
    double time;
    QString text;
    Lyric(double time,QString text);
    ~Lyric();
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*辅助函数*/
    void PlayVedio(QString);//播放视频
    QString getNextAudioOrVedio();//获取下一个播放的内容
    QString getLastAudioOrVedio();//获取上一个播放的内容
    QString getListItemFileAddress(QListWidgetItem *item);//获取列表项中的地址
    void SetCurrentListItemColor(QColor q);//设置当前列表项的颜色
    void SaveLocalCache();//缓存列表项
    void LoadLocalCache();//读取列表项
    void PlayAudioAndVedio(QString file);//播放音视频
    void AddListItem(QString FileAdd);//添加列表项
    int GetCurrentAudioOrVeido();//获取当前
    void setget_Alltime(qint64 playtime);//获取视频时长并设置到标签
    void setget_currenttime(qint64 playtime);//获取当时播放位置并设置
    void settimeslider(qint64 playtime);//设置进度条
    void AddAudioOrVideo(QString path);//载入音视频
    int isAudioOrVideo(QString name);//判断一个文件是音频还是视频
    int FindAudioOrVideoByName(QString FileName);//通过文件名查找数组中的文件
    int isFileNameRepeated(QString FileName);
    int setLRC(QString file);//设置歌词文件
    void setListWidgetLRC(QString FileName);//设置列表项
    void setListCurrentPlayingLRC(double sec);
    void setAlbumSize();

    /*事件函数*/
    bool eventFilter(QObject *watched, QEvent *event);//事件过滤器
    void dragEnterEvent(QDragEnterEvent *event);//拖拽事件
    void dropEvent(QDropEvent *event);//放下事件
    void keyPressEvent(QKeyEvent *event);   //键盘按下事件
    void keyReleaseEvent(QKeyEvent *event); //键盘松开事件
    void mousePressEvent(QMouseEvent *event); //键盘松开事件




private slots:
    void on_PlayButton_clicked();//播放按钮槽函数
    void on_timer_timeout();//计时器槽函数
    void on_FullScreenButton_clicked();//全屏按钮槽函数
    void on_PlayListButton_clicked();//播放列表槽函数
    void on_ListWidget_itemDoubleClicked(QListWidgetItem *item);//播放列表项槽函数
    void on_ModeButton_clicked();//模式按钮槽函数
    void on_NextVedioButton_clicked();//下一首槽函数
    void on_LastVedioButton_clicked();//上一首槽函数

    void on_AddButton_clicked();
    void getduration(qint64 playtime);   //获取时间改变信号
    void getposition(qint64 playtime);
    void UpdateCurrentAudioOrVedio();//通过背景色更新当前播放的音视频文件

    void on_VoiceButton_clicked();

    void on_SetButton_clicked();
    void SetVolumeValue(int value);
    void SetFrameValue(int frame);
    void SetPlaySpeed(int playspeed);
    void ShowFrame(double value);
    void CloseFrame();

private:
    QTimer* FunctionWidgetTimer;
    Ui::MainWindow *ui;

    /*标志位*/
    int PlaySeqence = 1;
    int PlayOrStopTag = 0;
    int PlayRate = 1;
    int CurrentAudioOrVedio = -1;
    int NextSongTag = 0;
    int NowPlayType = 0;//1表示视频，2表示音频
    int frame=1;//表示快进或者后退的帧数
    int ShowFrameCount=0;//计数器，使得鼠标移动的触发没有那么频繁

    /*变量*/
    QMediaPlayer *player =new QMediaPlayer;  //用于解析视频流的媒体播放器
    QVideoWidget *videowidget =new QVideoWidget();  //用于播放视频
    QVBoxLayout *layout = new QVBoxLayout;  //用于指定视频播放的位置
    QAudioOutput *audioOutput = new QAudioOutput; // chooses the default audio routing

    int m_hour;                             //视频总时长
    int m_min;                           //分钟
    int m_sec;                           //秒
    bool m_sliderstate;                     //滑块是否被选中的状态
    int m_slider_crtval;                    //滑块当前值
    SetWindow sw;//设置窗口
    SetVolume Volumew;
    GetFrame gf;
    QPixmap waveform_img;
    int playwidth;


    /*数组*/
    QVector<Video> VideoVector;
    QVector<Audio> AudioVector;
    QVector<Lyric> LyricVector;
};

#endif // MAINWINDOW_H
