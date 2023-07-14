#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include <stdio.h>

MediaTime::MediaTime(qint64 play_time) //play_time单位为秒
{
    hrs = play_time / 3600;
    min = (play_time - hrs * 3600) / 60;
    sec = play_time - hrs * 3600 - min * 60;
}

QString MediaTime::getTime()
{
//    qDebug() << hrs << ":" << min << ":" << sec;
    return QString("%1:%2:%3").arg(hrs).arg(min).arg(sec);
}
void MediaTime::setTime(qint64 play_time)
{
    hrs = play_time / 3600;
    min = (play_time - hrs * 3600) / 60;
    sec = play_time - hrs * 3600 - min * 60;
}

Media::Media(const char* path)
{
    AVFormatContext *formatCtx = NULL;

    //打开输入流并读取头
    //流要使用avformat_close_input关闭
    //成功时返回=0
    int result=avformat_open_input(&formatCtx, path, NULL, NULL);
    if (result!=0||formatCtx==NULL){
//        return false;
    }

    //读取文件获取流信息，把它存入AVFormatContext中
    //正常时返回>=0
    if (avformat_find_stream_info(formatCtx, NULL) < 0) {
//        return false;
    }

    this->path = QString(path);
    //duration/AV_TIME_BASE单位为秒
    this->duration = MediaTime(formatCtx->duration/AV_TIME_BASE);
    this->current_T = MediaTime(qint64(0));

//    qDebug() << "Constructing Media";
//    qDebug() << "Path: " << this->path << "\nDuration: ";
    this->duration.getTime();

    avformat_close_input(&formatCtx);
    avformat_free_context(formatCtx);
}

QString Media::GetPath()
{
    return this->path;
}

Video::Video(const char* path):Media(path)
{
    //借助析构函数来释放自源
    AudioInfoGuard guard;

    //打开输入流并读取头
    //流要使用avformat_close_input关闭
    //成功时返回=0
    int result=avformat_open_input(&guard.formatCtx, path, NULL, NULL);
    if (result!=0||guard.formatCtx==NULL){
        //qDebug() << "avformat_open_input failed!";
        return ;
    }

    //读取文件获取流信息，把它存入AVFormatContext中
    //正常时返回>=0
    if (avformat_find_stream_info(guard.formatCtx, NULL) < 0) {
        //qDebug() << "avformat_find_stream_info failed!";
        return ;
    }

    for (unsigned int i = 0; i < guard.formatCtx->nb_streams; i++)
    {
        AVStream *in_stream = guard.formatCtx->streams[i];
        if(in_stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            //查找具有匹配编解码器ID的已注册解码器
            //失败返回NULL
            guard.codec = avcodec_find_decoder(in_stream->codecpar->codec_id);
            if(guard.codec==NULL){
                //qDebug() << "guard.codec==NULL";
                return ;
            }

            //分配AVCodecContext并将其字段设置为默认值
            //需要使用avcodec_free_context释放生成的对象
            //如果失败，则返回默认填充或者 NULL
            guard.codecCtx = avcodec_alloc_context3(guard.codec);
            if(guard.codecCtx==NULL){
                //qDebug() << "guard.codecCtx==NULL";
                return ;
            }

            //根据编码器填充上下文参数
            //事实上codecpar包含了大部分解码器相关的信息，这里是直接从AVCodecParameters复制到AVCodecContext
            //成功时返回值>=0
            if(avcodec_parameters_to_context(guard.codecCtx, in_stream->codecpar)<0){
                //qDebug() << "avcodec_parameters_to_context(guard.codecCtx, in_stream->codecpar)<0";
                return ;
            }

            //某些AVCodecContext字段的访问器，已弃用
            //av_codec_set_pkt_timebase(codec_ctx, in_stream->time_base);

            //打开解码器
            //使用给定的AVCodec初始化AVCodecContext
            //在之前必须使用avcodec_alloc_context3()分配上下文
            //成功时返回值=0
            if(avcodec_open2(guard.codecCtx, guard.codec, nullptr)!=0){
                //qDebug() << "avcodec_open2(guard.codecCtx, guard.codec, nullptr)!=0";
                return ;
            }
            int i;
            std::string str_path = path;
            for(i = str_path.length();i > 0;i--)
            {
                if(str_path[i] == '/' || str_path[i] == '\\')
                {
                    break;
                }
            }
            this->name = QString::fromStdString(str_path.substr(i + 1));
            this->bit_rate = QString::number(guard.formatCtx->bit_rate) + "bps";
            this->codec = QString(guard.codec->long_name);
            guard.formatCtx->duration/AV_TIME_BASE;
            this->frame_rate = QString("%1fps").arg(in_stream->avg_frame_rate.num / in_stream->avg_frame_rate.den);
            this->nb_frames = in_stream->avg_frame_rate.num * (guard.formatCtx->duration/AV_TIME_BASE);
            this->resolution = QString("%1*%2").arg(guard.codecCtx->width).arg(guard.codecCtx->height);
//            AVDictionaryEntry* entry = nullptr;
//            while ((entry = av_dict_get(guard.formatCtx->metadata, "", entry, AV_DICT_IGNORE_SUFFIX))) {
//                fprintf(stdout, "key: %s, value: %s\n", entry->key, entry->value);
//            }
        }
    }
}

QString Video::getInfo()
{
    QString output = "";
    output = "Name: " + this->name + "\n" +
            "Path: " + this->path + "\n" +
            "Codec: " + this->codec + "\n" +
            "Bit Rate: " + this->bit_rate + "\n" +
            "Frame Rate: " + this->frame_rate + "\n" +
            QString("Frame Num: %1").arg(this->nb_frames) + "\n" +
            "Resolution: " + this->resolution + "\n" +
            "Duration: " + this->duration.getTime() + "\n";
//    qDebug() << "Name: " + this->name;
//    qDebug() << "Path: " + this->path;
//    qDebug() << "Codec: " + this->codec;
//    qDebug() << "Bit Rate: " + this->bit_rate;
//    qDebug() << "Frame Rate: " + this->frame_rate;
//    qDebug() << QString("Frame Num: %1").arg(this->nb_frames);
//    qDebug() << "Resolution: " + this->resolution;
//    qDebug() << "Duration: " + this->duration.getTime();
    return output;
}

Audio::Audio(const char* path):Media(path)
{
    //借助析构函数来释放自源
    AudioInfoGuard guard;

    //打开输入流并读取头
    //流要使用avformat_close_input关闭
    //成功时返回=0
    int result=avformat_open_input(&guard.formatCtx, path, NULL, NULL);
    if (result!=0||guard.formatCtx==NULL){
        //qDebug() << "avformat_open_input failed!";
        return ;
    }

    //读取文件获取流信息，把它存入AVFormatContext中
    //正常时返回>=0
    if (avformat_find_stream_info(guard.formatCtx, NULL) < 0) {
        //qDebug() << "avformat_find_stream_info failed!";
        return ;
    }

    for (unsigned int i = 0; i < guard.formatCtx->nb_streams; i++)
    {
        if (guard.formatCtx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC) {
            AVPacket pkt = guard.formatCtx->streams[i]->attached_pic;
            //使用QImage读取完整图片数据（注意，图片数据是为解析的文件数据，需要用QImage::fromdata来解析读取）
            QImage img = QImage::fromData((uchar*)pkt.data, pkt.size);
            this->cover = img;
//            ui->label_screen->setPixmap(QPixmap::fromImage(img));
            break;
        }

        AVStream *in_stream = guard.formatCtx->streams[i];
        if(in_stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){
            //查找具有匹配编解码器ID的已注册解码器
            //失败返回NULL
            guard.codec = avcodec_find_decoder(in_stream->codecpar->codec_id);
            if(guard.codec==NULL){
                //qDebug() << "guard.codec==NULL";
                return ;
            }

            //分配AVCodecContext并将其字段设置为默认值
            //需要使用avcodec_free_context释放生成的对象
            //如果失败，则返回默认填充或者 NULL
            guard.codecCtx = avcodec_alloc_context3(guard.codec);
            if(guard.codecCtx==NULL){
                //qDebug() << "guard.codecCtx==NULL";
                return ;
            }

            //根据编码器填充上下文参数
            //事实上codecpar包含了大部分解码器相关的信息，这里是直接从AVCodecParameters复制到AVCodecContext
            //成功时返回值>=0
            if(avcodec_parameters_to_context(guard.codecCtx, in_stream->codecpar)<0){
                //qDebug() << "avcodec_parameters_to_context(guard.codecCtx, in_stream->codecpar)<0";
                return ;
            }

            //某些AVCodecContext字段的访问器，已弃用
            //av_codec_set_pkt_timebase(codec_ctx, in_stream->time_base);

            //打开解码器
            //使用给定的AVCodec初始化AVCodecContext
            //在之前必须使用avcodec_alloc_context3()分配上下文
            //成功时返回值=0
            if(avcodec_open2(guard.codecCtx, guard.codec, nullptr)!=0){
                //qDebug() << "avcodec_open2(guard.codecCtx, guard.codec, nullptr)!=0";
                return ;
            }
            this->bit_rate = QString::number(guard.formatCtx->bit_rate) + "bps";
            this->channel_count = guard.codecCtx->channels;
            this->codec = QString(guard.codec->long_name);
            AVDictionaryEntry* entry = nullptr;
            while ((entry = av_dict_get(guard.formatCtx->metadata, "", entry, AV_DICT_IGNORE_SUFFIX))) {
//                //qDebug() << entry->key << entry->value;
                if(strcmp(entry->key, "title") == 0)
                    this->name = QString(entry->value);
                if(strcmp(entry->key, "album") == 0)
                    this->album = QString(entry->value);
                if(strcmp(entry->key, "artist") == 0)
                    this->artist = QString(entry->value);
            }
        }
    }
    this->name=QFileInfo(path).fileName();
    this->lyricpath="";

    // 生成波形图
    /* 1. 包含头文件: "windows.h", <cstdlib>, <stdio.h>, <io.h>, <direct.h>.
     * 2. 在Audio类中添加成员函数: QImage get_waveform(), 不添加数据成员QImage waveform的原因是节省内存RAM
     * 3. 和 2.同理,提取封面图也可以改用这种方法节省内存
     * 4. MediaTime类增加了一个函数qint64 getSec()用于提取总时长的秒数
    */
    // 建立波形图缓存文件夹 wave_form_imgs


    if (_access("wave_form_imgs",0) == -1)
    {
        //qDebug() << "The file/dir doesn't exisit";
        _mkdir("wave_form_imgs");
    }
    std::string ffmpeg_exe_path = "ffmpeg";
    std::string output_path = "wave_form_imgs\\" + this->name.toStdString() + ".png";
    qint64 audio_length = this->duration.getSec();
    std::string waveform_img_width = QString::number(audio_length * 10).toStdString();  // 音频长度为72秒，则波形图宽度为720p
    std::string waveform_img_height = "70";  // 建议直接设置成 ui->label_waveform的高度
    std::string cmd = ffmpeg_exe_path + " -i " + this->path.toStdString() + " -filter_complex \"showwavespic=s=" + waveform_img_width + "x" + waveform_img_height + ":colors=0x66ccff\" -frames:v 1 " + output_path;
    //qDebug() << QString::fromStdString(cmd);
    WinExec(cmd.c_str(), SW_HIDE);
    while(1)  //检查波形图是否已经生成
    {
        if(_access(output_path.c_str(), 0) == -1);
        else
            break;
    }
}

QImage Audio::getCover()
{
    return this->cover;
}

QImage Audio::getWaveform()
{
    //qDebug() << QString::fromStdString("wave_form_imgs\\" + this->name.toStdString() + ".png");
    return QImage(QString::fromStdString("wave_form_imgs\\" + this->name.toStdString() + ".png"));
}

qint64 MediaTime::getSec()
{
    return this->hrs * 3600 + this->min * 60 + this->sec;
}


QString Audio::getInfo()
{
    QString output = "";
    output = "Name: " + this->name + "\n" +
            "Path: " + this->path + "\n" +
            "Artist: " + this->artist + "\n" +
            "Album: " + this->album + "\n" +
            "Codec: " + this->codec + "\n" +
            "Bit Rate: " + this->bit_rate + "\n" +
            QString("Channels: %1").arg(this->channel_count) + "\n" +
            "Duration: " + this->duration.getTime() + "\n";
//    qDebug() << "Name: " + this->name;
//    qDebug() << "Path: " + this->path;
//    qDebug() << "Artist: " + this->artist;
//    qDebug() << "Album: " + this->album ;
//    qDebug() << "Codec: " + this->codec ;
//    qDebug() << "Bit Rate: " + this->bit_rate ;
//    qDebug() << QString("Channels: %1").arg(this->channel_count);
//    qDebug() << "Duration: " + this->duration.getTime();
    return output;
}

Lyric::Lyric(double time,QString text)
{
    this->time=time;
    this->text=text;
}
Lyric::~Lyric()
{

}
