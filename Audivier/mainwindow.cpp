#include "mainwindow.h"
#include "ui_mainwindow.h"

/*-------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------主函数-----------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //基本设置
    setAcceptDrops(true);// 设置之后才能拖拽文件到窗口
    videowidget->setMouseTracking(1);//设置播放容器的鼠标追踪
    QList<QWidget*> labelList = videowidget->findChildren<QWidget*>();//找出videowidget的所有子控件，将他们的鼠标追踪设置为1
    for(int i = 0; i < labelList.size(); i++)
    {
        labelList.at(i)->setMouseTracking(1);
    }

    ui->VideoScreenLabel->installEventFilter(this);//播放标签设置事件过滤器
    ui->AudioWidget->installEventFilter(this);
    ui->PlayListWidget->setVisible(0);
    ui->AudioWidget->setVisible(0);  
    ui->LRCWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //垂直滚动条
    ui->LRCWidget->setSpacing(4);

    FunctionWidgetTimer=new QTimer(this);//定义功能栏定时器
    FunctionWidgetTimer->setInterval(5000);//设置间隔为5s

    LoadLocalCache();//载入数据

    QFile qssFile(":/images/style.qss");
    qssFile.open(QFile::ReadOnly);
    QString styleSheet = tr(qssFile.readAll());
    this->setStyleSheet(styleSheet);
    qssFile.close();

    //连接操作
    connect(FunctionWidgetTimer,SIGNAL(timeout()),this,SLOT(on_timer_timeout()));//关联定时器与定时器槽函数

    m_sliderstate = false;
    connect(ui->ProgressSlider, &PlayerSlider::mousepress, [=](double val){
        m_sliderstate = true;
        PlayOrStopTag = 0;
        player->pause();
        ui->PlayButton->setIcon(QIcon(":/images/Play.png"));
        //qDebug() << "progress slider pressed by mouse" << val;
    });
    connect(ui->ProgressSlider, &PlayerSlider::mouserelease, [=](double val){
        m_sliderstate = false;
        player->setPosition(val*1000);
        ui->ProgressSlider->setValue(val*1000);
        PlayOrStopTag = 1;
        player->play();
        ui->PlayButton->setIcon(QIcon(":/images/Stop.png"));
        qint64 playtime = player->position();
        //qDebug() << "progress slider released by mouse" << val;
    });

    connect(ui->ListWidget,SIGNAL(ItemDragSignal()),this,SLOT(UpdateCurrentAudioOrVedio()));
    connect(&Volumew,SIGNAL(VolumeSignal(int)),this,SLOT(SetVolumeValue(int)));
    connect(&sw,SIGNAL(FrameSignal(int)),this,SLOT(SetFrameValue(int)));
    connect(&sw,SIGNAL(PlaySpeedSignal(int)),this,SLOT(SetPlaySpeed(int)));
    connect(ui->ProgressSlider,SIGNAL(SliderMouseMove(double)),this,SLOT(ShowFrame(double)));
    connect(ui->ProgressSlider,SIGNAL(SliderMouseLeave()),this,SLOT(CloseFrame()));

    //子窗口

    //qDebug()<<isAudioOrVideo("interviewer.mp3");
//    Audio TempAudio("C:\\Users\\hikari\\Desktop\\Download\\interviewer.mp3");
//    AudioVector.append(TempAudio);
//    qDebug()<<FindAudioOrVideoByName("interviewer.mp3");
    //ui->MainWidget->setWindowOpacity(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*-------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------辅助函数---------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/

//播放视频
void MainWindow::PlayVedio(QString file)
{
//    if(file==" ")//单曲播放中，下一个为空
//    {
//        return;
//    }

//    AVFormatContext    *pFormatCtx;
//    int                i, videoindex;
//    AVCodecContext    *pCodecCtx;
//    AVCodec            *pCodec;
//    AVFrame    *pFrame, *pFrameRGB;
//    unsigned char *out_buffer;
//    AVPacket *packet;
//    int ret, got_picture;
//    struct SwsContext *img_convert_ctx;

//    int l=file.length();
//    char* filepath;
//    filepath=new char[l];
//    for(int i=0;i<l;i++)
//    {
//        filepath[i]=file[i].toLatin1();
//    }
//    filepath[l]='\0';

//    QFileInfo f(file);
//    if(f.isFile()==0)
//    {
//        QMessageBox::critical(this, tr("错误"), tr("文件不存在"));
//        return;
//    }

//    //初始化编解码库
////    av_register_all();//创建AVFormatContext对象，与码流相关的结构。
//    pFormatCtx = avformat_alloc_context();
//    //初始化pFormatCtx结构
//    if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0){
//        printf("Couldn't open input stream.\n");
//        return ;
//    }
//    //获取音视频流数据信息
//    if (avformat_find_stream_info(pFormatCtx, NULL) < 0){
//        printf("Couldn't find stream information.\n");
//        return ;
//    }
//    videoindex = -1;
//    //nb_streams视音频流的个数，这里当查找到视频流时就中断了。
//    for (i = 0; i < pFormatCtx->nb_streams; i++)
//        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
//            videoindex = i;
//            break;
//    }
//    if (videoindex == -1){
//        printf("Didn't find a video stream.\n");
//        return ;
//    }
//    //获取视频流编码结构
//    pCodecCtx = pFormatCtx->streams[videoindex]->codec;
//    //查找解码器
//    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
//    if (pCodec == NULL){
//        printf("Codec not found.\n");
//        return ;
//    }
//    //用于初始化pCodecCtx结构
//    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
//        printf("Could not open codec.\n");
//        return ;
//    }
//    //创建帧结构，此函数仅分配基本结构空间，图像数据空间需通过av_malloc分配
//    pFrame = av_frame_alloc();
//    pFrameRGB = av_frame_alloc();
//    //创建动态内存,创建存储图像数据的空间
//    //av_image_get_buffer_size获取一帧图像需要的大小
//    out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1));
//    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, out_buffer,
//        AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1);

//    packet = (AVPacket *)av_malloc(sizeof(AVPacket));
//    //Output Info-----------------------------
//    printf("--------------- File Information ----------------\n");
//    //此函数打印输入或输出的详细信息
//    av_dump_format(pFormatCtx, 0, filepath, 0);
//    printf("-------------------------------------------------\n");
//    //初始化img_convert_ctx结构
//    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
//        pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
//    //av_read_frame读取一帧未解码的数据
//    while (av_read_frame(pFormatCtx, packet) >= 0){
//        //如果是视频数据
//        if (packet->stream_index == videoindex){
//            //解码一帧视频数据
//            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
//            if (ret < 0){
//                printf("Decode Error.\n");
//                return ;
//            }
//            if (got_picture){
//                sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
//                    pFrameRGB->data, pFrameRGB->linesize);
//                QImage img((uchar*)pFrameRGB->data[0],pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
//                ui->VideoScreenLabel->setPixmap(QPixmap::fromImage(img));
//                Delay(40/PlayRate);
//            }
//        }
//        av_free_packet(packet);
//    }
//    sws_freeContext(img_convert_ctx);
//    av_frame_free(&pFrameRGB);
//    av_frame_free(&pFrame);
//    avcodec_close(pCodecCtx);
//    avformat_close_input(&pFormatCtx);
//    QString NextAudioOrVedio=getNextAudioOrVedio();
//    PlayVedio(NextAudioOrVedio);
}

void MainWindow::PlayAudioAndVedio(QString file)
{
    player->stop();
    if(file==" ")//单曲播放中，下一个为空
    {
        return;
    }

    int l=file.length();
    char* filepath;
    filepath=new char[l];
    for(int i=0;i<l;i++)
    {
        filepath[i]=file[i].toLatin1();
    }
    filepath[l]='\0';

    QFileInfo f(file);
    if(f.isFile()==0)
    {
        QMessageBox::critical(this, tr("错误"), tr("未选中文件"));
        return;
    }


    /*界面设置*/
    if(isAudioOrVideo(f.fileName())==1)
    {
        ui->AudioWidget->setVisible(0);
        ui->VideoScreenLabel->setVisible(1);
        NowPlayType=1;//将当前播放的类型设置为视频
    }
    else if(isAudioOrVideo(f.fileName())==2)
    {
        ui->AudioInfoLabel->clear();
        ui->LRCWidget->clear();
        ui->AudioWidget->setVisible(1);
        ui->WaveLabel->setVisible(1);
        ui->VideoScreenLabel->setVisible(0);
        int AlbumSizeH=ui->MainWidget->size().height()/2;
        int AlbumSizeW=ui->MainWidget->size().width()/2;
        if(AlbumSizeH>AlbumSizeW)
        {
            AlbumSizeH=AlbumSizeW;
        }
        else
        {
            AlbumSizeW=AlbumSizeH;
        }
        QSize AlbumSize(AlbumSizeW,AlbumSizeH);
        if(!AudioVector[FindAudioOrVideoByName(f.fileName())].cover.isNull())//如果文件存在
        {
            ui->AlbumCoverLabel->setPixmap(QPixmap::fromImage(AudioVector[FindAudioOrVideoByName(f.fileName())].cover).scaled(AlbumSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        }
        else
        {
            ui->AlbumCoverLabel->setPixmap(QPixmap::fromImage(QImage(":/images/MusicAlbums.png")).scaled(AlbumSize));
        }
        setListWidgetLRC(f.fileName());
        NowPlayType=2;//将当前播放的类型设置为音频
        this->waveform_img = QPixmap::fromImage(AudioVector[FindAudioOrVideoByName(f.fileName())].getWaveform());
    }
    PlayOrStopTag=1;//设置播放状态为“正在播放”
    ui->PlayButton->setIcon(QIcon(":/images/Stop.png"));//设置图标

    /*播放设置*/
    layout->setContentsMargins(0,0,0,0);
    player->setSource(QUrl::fromLocalFile(filepath));  //打开音视频流文件
    videowidget->resize(ui->VideoScreenLabel->size());  //把视频输出图像的大小重新调整为label的大小
    //指定视频播放的位置
    layout->addWidget(videowidget);
    ui->VideoScreenLabel->setLayout(layout);

    player->setVideoOutput(videowidget);  //设置视频输出附加到媒体播放器
    player->setAudioOutput(audioOutput);
    player->play();  //开始播放视频

    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(getduration(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(getposition(qint64)));

    //QString NextAudioOrVedio=getNextAudioOrVedio();
    //PlayAudioAndVedio(NextAudioOrVedio);
}



//获取列表项对应的地址
QString MainWindow::getListItemFileAddress(QListWidgetItem *item)
{
    QStringList SplitList=item->text().split("\t");
    return SplitList[1];
}

//索引到下一个音视频的地址
QString MainWindow::getNextAudioOrVedio()
{
    if(CurrentAudioOrVedio==-1)return "";//如果没有任何音视频可以播放，直接返回
    int PlayListLength=ui->ListWidget->count();
    QString FileName;
    if(PlaySeqence==1)
    {
        return " ";
    }
    else if(PlaySeqence==2)
    {
        SetCurrentListItemColor(QColor(255,255,255));
        CurrentAudioOrVedio=(CurrentAudioOrVedio+1)%PlayListLength;//循环
        SetCurrentListItemColor(QColor(255,182,193));
        FileName=ui->ListWidget->item(CurrentAudioOrVedio)->text();
        if(isAudioOrVideo(FileName)==1)
        {
            return VideoVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
        else if(isAudioOrVideo(FileName)==2)
        {
            return AudioVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
        //获取指针对应的列表项对应的地址
    }
    else if(PlaySeqence==3)
    {
        int TempRandNum=QRandomGenerator::global()->bounded(PlayListLength);
        SetCurrentListItemColor(QColor(255,255,255));
        CurrentAudioOrVedio=(CurrentAudioOrVedio+TempRandNum)%PlayListLength;//让下一个音视频被选中
        SetCurrentListItemColor(QColor(255,182,193));
        FileName=ui->ListWidget->item(CurrentAudioOrVedio)->text();
        if(isAudioOrVideo(FileName)==1)
        {
            return VideoVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
        else if(isAudioOrVideo(FileName)==2)
        {
            return AudioVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
    }
    else if(PlaySeqence==4)
    {
        FileName=ui->ListWidget->item(CurrentAudioOrVedio)->text();
        if(isAudioOrVideo(FileName)==1)
        {
            return VideoVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
        else if(isAudioOrVideo(FileName)==2)
        {
            return AudioVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
    }
}

QString MainWindow::getLastAudioOrVedio()
{
    if(CurrentAudioOrVedio==-1)return "";//如果没有任何音视频可以播放，直接返回
    int PlayListLength=ui->ListWidget->count();
    QString FileName;
    if(PlaySeqence==1)
    {
        return " ";
    }
    else if(PlaySeqence==2)
    {
        SetCurrentListItemColor(QColor(255,255,255));
        CurrentAudioOrVedio=(CurrentAudioOrVedio+PlayListLength-1)%PlayListLength;//循环
        SetCurrentListItemColor(QColor(255,182,193));
        FileName=ui->ListWidget->item(CurrentAudioOrVedio)->text();
        if(isAudioOrVideo(FileName)==1)
        {
            return VideoVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
        else if(isAudioOrVideo(FileName)==2)
        {
            return AudioVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
    }
    else if(PlaySeqence==3)
    {
        SetCurrentListItemColor(QColor(255,255,255));
        int TempRandNum=QRandomGenerator::global()->bounded(PlayListLength);
        CurrentAudioOrVedio=(CurrentAudioOrVedio+TempRandNum)%PlayListLength;//让下一个音视频被选中
        SetCurrentListItemColor(QColor(255,182,193));
        FileName=ui->ListWidget->item(CurrentAudioOrVedio)->text();
        if(isAudioOrVideo(FileName)==1)
        {
            return VideoVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
        else if(isAudioOrVideo(FileName)==2)
        {
            return AudioVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
    }
    else if(PlaySeqence==4)
    {
        FileName=ui->ListWidget->item(CurrentAudioOrVedio)->text();
        if(isAudioOrVideo(FileName)==1)
        {
            return VideoVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
        else if(isAudioOrVideo(FileName)==2)
        {
            return AudioVector[FindAudioOrVideoByName(FileName)].GetPath();
        }
    }
}

void MainWindow::SaveLocalCache()
{
    QFile* myfile=new QFile("./Cache.txt");//创建一个输出文件的文档
    if (myfile->open(QFile::WriteOnly|QIODevice::Truncate))//注意WriteOnly是往文本中写入的时候用，ReadOnly是在读文本中内容的时候用，Truncate表示将原来文件中的内容清空
    {
        QTextStream out(myfile);
        for(int i=0;i<ui->ListWidget->count();i++)
        {
            QString OutPutString="";
            QString OutPutFileName=ui->ListWidget->item(i)->text();//文件名在列表中获取
            QString OutPutFilePath="";
            QString OutPutLRCPath="";
            if(isAudioOrVideo(OutPutFileName)==1)//如果文件为视频
            {
                OutPutFilePath=VideoVector[FindAudioOrVideoByName(OutPutFileName)].GetPath();//从视频数组中依据文件名获取文件路径
            }
            else if(isAudioOrVideo(OutPutFileName)==2)//如果文件为音频
            {
                OutPutFilePath=AudioVector[FindAudioOrVideoByName(OutPutFileName)].GetPath();//从音频数组中依据文件名获取文件路径
                if(AudioVector[FindAudioOrVideoByName(OutPutFileName)].lyricpath!="")
                {
                    OutPutLRCPath=AudioVector[FindAudioOrVideoByName(OutPutFileName)].lyricpath;
                    OutPutLRCPath+="\n";
                }
            }
            OutPutString=OutPutString+OutPutFilePath+"\n"+OutPutLRCPath;
            out<<OutPutString;//进行输出
        }
        out<<"EOF";
    }
    myfile->close();
}

void MainWindow::LoadLocalCache()
{
    QFile* myfile=new QFile("./Cache.txt");//创建一个输出文件的文档
    QString ErrorFileName="";
    int ErrorTag=0;
    if (myfile->open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream out(myfile);
        while(1)
        {
            QString tempstr;
            tempstr=out.readLine();
            if(tempstr=="EOF")//如果已经到了缓存文件的最后
            {
                break;
            }
            else
            {
                QFileInfo file(tempstr);//读取文件路径
                if(file.isFile()==0)//判断文件能否正常打开
                {//如果不能
                    ErrorFileName+=file.fileName();
                    ErrorFileName+="\n";
                    ErrorTag=1;
                }
                else//如果能，加入列表
                {
                    if(isAudioOrVideo(file.fileName())==1)//如果是视频
                    {
                        Video TempVideo(file.filePath().toLatin1().data());//加入视频数组
                        VideoVector.append(TempVideo);
                        ui->ListWidget->addItem(file.fileName());//加入播放列表
                    }
                    else if(isAudioOrVideo(file.fileName())==2)//如果是音频
                    {
                        Audio TempAudio(file.filePath().toLatin1().data());//加入音频数组
                        AudioVector.append(TempAudio);
                        ui->ListWidget->addItem(file.fileName());//加入播放列表
                    }
                    else if(isAudioOrVideo(file.fileName())==3)//如果是LRC文件
                    {
                        if(!setLRC(file.filePath()))
                        {
                            ErrorFileName+=file.fileName();
                            ErrorFileName+="\n";
                            ErrorTag=1;
                        }
                    }
                }
            }
        }
    }
    if(ErrorTag==1)QMessageBox::critical(this, tr("错误"), tr("以下文件无法获取：\n\n%1").arg(ErrorFileName));//如果出现了错误文件
    myfile->close();
    SaveLocalCache();
}

void MainWindow::AddListItem(QString FileAdd)
{
    QFileInfo file(FileAdd);
    //QString AbsPath=file.absoluteFilePath();//获取绝对路径
    QString FileName=file.fileName();//获取文件名
    //qDebug()<<FileName;
    //QString temp=FileName+"\t"+AbsPath;
    QString temp=FileName;
    QListWidgetItem* item=new QListWidgetItem(temp);
    ui->ListWidget->addItem(item);//添加列表项
    CurrentAudioOrVedio=ui->ListWidget->count()-1;//设置指针
}


void MainWindow::setget_Alltime(qint64 playtime)
{
    playtime /= 1000;
    ui->ProgressSlider->setMinimum(0);
    ui->ProgressSlider->setMaximum(playtime);

    m_hour = playtime / 3600;
    m_min = (playtime - m_hour * 3600) / 60;
    m_sec = playtime - m_hour * 3600 - m_min * 60;
    //qDebug() << "duration: " << m_hour << " " << m_min << " " << m_sec;
    QString str = QString("%1:%2:%3").arg(m_hour).arg(m_min).arg(m_sec);
    ui->TotalTimeLabel->setText(str);
}

void MainWindow::setget_currenttime(qint64 playtime)
{
    setListCurrentPlayingLRC(playtime);
    int playtime_int = playtime/1000;
    double playtime_double =double(playtime)/1000;
    settimeslider(playtime_int);
    int h = playtime_int / 3600;
    int m = (playtime_int - h * 3600) / 60;
    int s = playtime_int - h * 3600 - m * 60;
    QString str = QString("%1:%2:%3").arg(h).arg(m).arg(s);
    ui->CurrentTimeLabel->setText(str);
    int playwidth;
    //playwidth=this->width()-ui->ListWidget->width()-60;
    playwidth=ui->MainWidget->width();


    //ui->AlbumCoverLabel->resize(ui->MainWidget->size()/2);

    if(player->mediaStatus()==QMediaPlayer::EndOfMedia)
    {
        on_NextVedioButton_clicked();
    }

    // 对this->waveform_img进行绘制和显示的更新
        /* 1. 总时长由 this->waveform_img.width()获得，同时也是waveform的宽度
         * 2. 能显示的宽度为 ui->label_waveform.width
         * 3. 根据当前时间 playtime决定所显示的 pixmap部分
         * 4. ui->label_waveform即为波形图的空间qlabel
         */
        int x, wave_width, /*brush_width,*/ x1;  // 当播放时长的10倍（设置了waveform.png的宽度时播放时长（秒）的十倍的值个像素）未及波形图显示框的一半时，x=0
        if(playtime_double * 10 < playwidth / 2)
        {
            x = 0;
    //        brush_width = playtime * 10;
            x1 = playtime_double * 10;
            wave_width = playwidth > this->waveform_img.width()? this->waveform_img.width() : playwidth;
        }
        else
        {
            x = playtime_double * 10 - playwidth / 2;
    //        brush_width = ui->label_waveform->width() / 2;
            x1 = playwidth / 2;
            wave_width = playwidth > this->waveform_img.width() - x? this->waveform_img.width() - x : playwidth;
        }
        //qDebug() << "playtime * 10: " << playtime_double * 10 << "\tx: " << x << "\twidth: " << wave_width;
        QPixmap show_pixmap = this->waveform_img.QPixmap::copy(x, 0,  playwidth, ui->AudioShowWidget->height());

        // 绘制波形图的位置线
        QPainter painter(&show_pixmap);
    //    QColor color(Qt::black);
    //    color.setAlpha(100);
    //    painter.setPen(Qt::NoPen);
    //    QBrush brush(color);
    //    painter.setBrush(brush);

    //    QRectF posRect(0, 0, brush_width, ui->label_waveform->height());
    //    painter.drawRect(posRect);
        painter.setPen(Qt::red);
        painter.drawLine(x1, 0, x1, ui->AudioShowWidget->height());

        ui->WaveLabel->setPixmap(show_pixmap);
}

void MainWindow::settimeslider(qint64 playtime)
{
    if(!m_sliderstate)
        ui->ProgressSlider->setValue(playtime);
}


//播放视频的延迟设置
//void Delay(int msec)
//{
//    QTime dieTime = QTime::currentTime().addMSecs(msec);
//    while( QTime::currentTime() < dieTime )
//        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
//}

void MainWindow::AddAudioOrVideo(QString path)
{
    QFileInfo FileInfo = QFileInfo(path);
    QString FileSuffix = FileInfo.suffix();
    if(isAudioOrVideo(path)==2)//如果是音频
    {
        Audio TempAudio(path.toLatin1().data());
        AudioVector.append(TempAudio);
    }
    else if(isAudioOrVideo(path)==1)//如果是视频
    {
        Video TempVideo(path.toLatin1().data());
        VideoVector.append(TempVideo);
    }
}

int MainWindow::isAudioOrVideo(QString name)
{
    QFileInfo QFI(name);
    if(QFI.suffix()=="mp4"||QFI.suffix()=="wmv"||QFI.suffix()=="avi")
    {
        return 1;
    }
    else if(QFI.suffix()=="mp3"||QFI.suffix()=="flac")
    {
        return 2;
    }
    else if(QFI.suffix()=="lrc")
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

int MainWindow::FindAudioOrVideoByName(QString FileName)
{
    if(isAudioOrVideo(FileName)==1)//如果是视频
    {
        int i=0;
        for(;i<VideoVector.length();i++)
        {
            if(FileName==VideoVector[i].name)
            {
                return i;
            }
        }
        return i;
    }
    else if(isAudioOrVideo(FileName)==2)//如果是音频
    {
        int i=0;
        for(;i<AudioVector.length();i++)
        {
            if(FileName==AudioVector[i].name)
            {
                return i;
            }
        }
        return i;
    }
    return -1;
}

int MainWindow::isFileNameRepeated(QString FileName)
{
    for(int i=0;i<VideoVector.length();i++)
    {
        if(FileName==VideoVector[i].name)
        {
            return 1;
        }
    }
    for(int i=0;i<AudioVector.length();i++)
    {
        if(FileName==AudioVector[i].name)
        {
            return 1;
        }
    }
    return 0;
}

int MainWindow::setLRC(QString file)
{
    for(int i=0;i<AudioVector.length();i++)
    {
        if(QFileInfo(file).fileName().split(".")[0]==AudioVector[i].name.split(".")[0])
        {
            AudioVector[i].lyricpath=file;
            return 1;
        }
    }
    return 0;
}
void MainWindow::setListWidgetLRC(QString FileName)
{
    for(int i=0;i<AudioVector.length();i++)
    {
        if(AudioVector[i].name==FileName)
        {
            ui->LRCWidget->clear();
            LyricVector.clear();
            if(AudioVector[i].lyricpath!="")
            {
                QFile TempFile(AudioVector[i].lyricpath);
                TempFile.open(QIODevice::ReadOnly|QIODevice::Text);
                QByteArray All=TempFile.readAll();
                QString s(All);
                QStringList list = s.split("\n");//QString字符串分割函数
                QString AudioInfo="";
                for(int i=0;i<list.length()-1;i++)
                {
                    QString text=list[i].split("]")[1];
                    QString textl=list[i].split("]")[0];
                    if(textl.mid(1,2)=="ti")
                    {
                        AudioInfo+="<b><font size=\"5\"> ";
                        AudioInfo+=textl.mid(4,textl.length()-4);
                        AudioInfo+="</font></b>";
                        AudioInfo+="<br>";
                        continue;
                    }
                    if(textl.mid(1,2)=="ar")
                    {
                        AudioInfo+="<font size=\"3\"> 歌手：";
                        AudioInfo+=textl.mid(4,textl.length()-4);
                        AudioInfo+="</font>";
                        AudioInfo+="<br>";
                        continue;
                    }
                    if(textl.mid(1,2)=="al")
                    {
                        AudioInfo+="<font size=\"3\">专辑：";
                        AudioInfo+=textl.mid(4,textl.length()-4);
                        AudioInfo+="</font>";
                        AudioInfo+="<br>";
                        continue;
                    }
                    if(textl.mid(1,2)=="by")
                    {
                        continue;
                    }
                    if(textl.mid(1,6)=="offset")
                    {
                        continue;
                    }
                    double time=list[i].split("]")[0].split("[")[1].split(":")[0].toDouble()*60000+list[i].split("]")[0].split("[")[1].split(":")[1].toDouble()*1000;
                    Lyric TempLRC(time,text);
                    LyricVector.append(TempLRC);
                    ui->LRCWidget->addItem(text);
                }
                ui->AudioInfoLabel->setText(AudioInfo);
            }
            else
            {
                ui->LRCWidget->addItem("暂无歌词，敬请欣赏音乐");
            }
            break;
        }
    }
    for(int i=0;i<ui->LRCWidget->count();i++)
    {
        ui->LRCWidget->item(i)->setTextAlignment(Qt::AlignCenter);
    }
}

void MainWindow::setListCurrentPlayingLRC(double sec)
{
    double msec=sec;
    int i=0;
    for(;i<LyricVector.length();i++)
    {
        if(LyricVector[i].time>msec)
        {
            break;
        }
    }
    ui->LRCWidget->setCurrentRow(i-1);
}
void MainWindow::setAlbumSize()
{

}
/*-------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------事件函数---------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
//播放标签事件过滤
bool MainWindow::eventFilter(QObject* obj,QEvent *ev)
{
    if(obj==ui->VideoScreenLabel||obj==ui->AudioWidget)//如果鼠标的移动进入了播放标签
    {
        if(ev->type()==QEvent::MouseMove&&this->isFullScreen()==1)//如果鼠标在动且处于全屏模式
        {
            ui->FunctionWidget->setVisible(1);//功能栏出现
            FunctionWidgetTimer->stop();//定时器重新计时
            FunctionWidgetTimer->start();
            this->setCursor(Qt::ArrowCursor); //显示鼠标
            return 1;
        }
    }
    return QWidget::eventFilter(obj,ev);
}

//拖拽进入事件
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    // 拖拽事件触发
    if( event->mimeData()->hasUrls() )
    {
        event->acceptProposedAction();
    }
}

//拖拽松开事件
void MainWindow::dropEvent(QDropEvent *event)
{
    int TypeErrorTag=0;//判断是否有读入错误的文件
    int RepeatedErrorTag=0;//判断文件是否重复
    int LRCErrorTag=0;//判断歌词文件是否重复
    // 创建可接受文件类型列表、
    QString TypeErrorFileName="";
    QString RepeatedErrorFileName="";
    QString LRCErrorFileName="";
    QStringList acceptedFileTypes;
    acceptedFileTypes<<"mp4"<<"avi"<<"wmv"<<"mp3"<<"flac"<<"lrc"<<"flv";
    for(int i=0;i<event->mimeData()->urls().count();i++)
    {
        // 取到拖拽文件转换成文件
        QFileInfo file(event->mimeData()->urls().at(i).toLocalFile());
        // 查看文件后缀名  是否在列表acceptedFileTypes中
        if(acceptedFileTypes.contains(file.suffix().toLower()))
        {
            if(!isFileNameRepeated(file.fileName()))//如果文件名没有重复
            {
                if(isAudioOrVideo(file.fileName())==1)
                {
                    Video TempVideo(file.filePath().toLatin1().data());
                    VideoVector.append(TempVideo);
                    AddListItem(event->mimeData()->urls().at(i).toLocalFile());// 让程序 同意将这个文件 读入
                }
                else if(isAudioOrVideo(file.fileName())==2)
                {
                    Audio TempAudio(file.filePath().toLatin1().data());
                    AudioVector.append(TempAudio);
                    AddListItem(event->mimeData()->urls().at(i).toLocalFile());// 让程序 同意将这个文件 读入
                }
                else if(isAudioOrVideo(file.fileName())==3)
                {
                    if(!setLRC(file.filePath()))
                    {
                        LRCErrorFileName+=file.fileName();
                        LRCErrorFileName+="\n";
                        LRCErrorTag=1;
                    }
                }
            }
            else
            {
                RepeatedErrorFileName+=file.fileName();
                RepeatedErrorFileName+="\n";
                RepeatedErrorTag=1;
            }
        }
        else//如果不是可以接受的文件类型，则报错
        {
            TypeErrorFileName+=file.fileName();
            TypeErrorFileName+="\n";
            TypeErrorTag=1;
        }
    }
    if(TypeErrorTag==1)QMessageBox::critical(this, tr("错误"), tr("以下文件：\n\n%1\n类型非法,读入失败").arg(TypeErrorFileName));//如果出现了错误文件
    if(RepeatedErrorTag==1)QMessageBox::critical(this, tr("错误"), tr("以下文件：\n\n%1\n文件名重复,读入失败").arg(RepeatedErrorFileName));//如果出现了错误文件
    if(LRCErrorTag==1)QMessageBox::critical(this, tr("错误"), tr("以下歌词文件：\n\n%1\n无法找到对应音频").arg(LRCErrorFileName));//如果出现了错误文件
    // 程序同意将文件加载，  下一步 读取文件
    SaveLocalCache();
}

//列表项“双击”槽函数（播放音视频）
void MainWindow::on_ListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if(CurrentAudioOrVedio!=-1)
    {
        for(int i=0;i<ui->ListWidget->count();i++)
        {
            ui->ListWidget->item(i)->setBackground(QColor(255,255,255));
        }
        //SetCurrentListItemColor(QColor(255,255,255));
    }
    CurrentAudioOrVedio=ui->ListWidget->currentRow();
    SetCurrentListItemColor(QColor(255,182,193));
    /*根据类型播放视频*/
    if(isAudioOrVideo(item->text())==1)
    {
        PlayAudioAndVedio(VideoVector[FindAudioOrVideoByName(item->text())].GetPath());
    }
    else if(isAudioOrVideo(item->text())==2)
    {
        PlayAudioAndVedio(AudioVector[FindAudioOrVideoByName(item->text())].GetPath());
    }
    PlayOrStopTag=1;
    ui->PlayButton->setIcon(QIcon(":/images/Stop.png"));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // 如果是鼠标右键按下
    if(event->button() == Qt::RightButton)
    {
        QMenu * ContextMenu = new QMenu;
        ContextMenu->addAction(new QAction("音视频信息",this));
        if(ui->ListWidget->currentItem())
        {
            QString filename=ui->ListWidget->currentItem()->text();
            if(isAudioOrVideo(filename)==1)
            {
                QString Info_name="文件名：";
                Info_name+=VideoVector[FindAudioOrVideoByName(filename)].name;
                QString Info_bit_rate="比特率：";
                Info_bit_rate+=VideoVector[FindAudioOrVideoByName(filename)].bit_rate;
                QString Info_codec="编解码器：";
                Info_codec+=VideoVector[FindAudioOrVideoByName(filename)].codec;

                QString Info_frame_rate="帧率：";
                Info_frame_rate+=VideoVector[FindAudioOrVideoByName(filename)].frame_rate;
                QString Info_resolution="分辨率：";
                Info_resolution+=VideoVector[FindAudioOrVideoByName(filename)].resolution;

                ContextMenu->addAction(new QAction(Info_name,this));
                ContextMenu->addAction(new QAction(Info_resolution,this));
                ContextMenu->addAction(new QAction(Info_frame_rate,this));
                ContextMenu->addAction(new QAction(Info_bit_rate,this));
                ContextMenu->addAction(new QAction(Info_codec,this));
            }
            else if(isAudioOrVideo(filename)==2)
            {
                QString Info_name="文件名：";
                Info_name+=AudioVector[FindAudioOrVideoByName(filename)].name;
                QString Info_bit_rate="比特率：";
                Info_bit_rate+=AudioVector[FindAudioOrVideoByName(filename)].bit_rate;
                QString Info_codec="编解码器：";
                Info_codec+=AudioVector[FindAudioOrVideoByName(filename)].codec;

                QString Info_channel_count="频道数：";
                Info_channel_count+=QString::number(AudioVector[FindAudioOrVideoByName(filename)].channel_count);
                QString Info_album="专辑：";
                Info_album+=AudioVector[FindAudioOrVideoByName(filename)].album;
                QString Info_artist="艺术家：";
                Info_artist+=AudioVector[FindAudioOrVideoByName(filename)].artist;

                ContextMenu->addAction(new QAction(Info_name,this));
                ContextMenu->addAction(new QAction(Info_artist,this));
                ContextMenu->addAction(new QAction(Info_album,this));
                ContextMenu->addAction(new QAction(Info_bit_rate,this));
                ContextMenu->addAction(new QAction(Info_channel_count,this));
                ContextMenu->addAction(new QAction(Info_codec,this));
            }
        }
        ContextMenu->exec(event->globalPos());
    }
}

//“键盘按下”事件
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Delete)
    {
       QList<QListWidgetItem*> ListWidgetItemList=ui->ListWidget->selectedItems();

       for(int i=0;i<ListWidgetItemList.size();i++)
       {
           if(ui->ListWidget->row(ListWidgetItemList[i])==CurrentAudioOrVedio&&PlayOrStopTag==1)//如果想要删除的是当前视频，且视频正在播放，提示错误并返回
           {
               QMessageBox::critical(NULL, "错误", tr("当前正在播放的音视频:\n%1\n无法从播放列表被删除！").arg(ListWidgetItemList[i]->text()), QMessageBox::Yes, QMessageBox::Yes);
               continue;
           }
           else if(ui->ListWidget->row(ListWidgetItemList[i])==CurrentAudioOrVedio&&PlayOrStopTag==0)//如果想要删除的是当前视频且视频暂停，可以删除
           {
               player->stop();
               delete player;
               player=new QMediaPlayer;
               CurrentAudioOrVedio=-1;
           }
           else if(ui->ListWidget->row(ListWidgetItemList[i])<CurrentAudioOrVedio)//如果要删除的是当前音视频之前的，那么指针-1
           {
               CurrentAudioOrVedio--;
           }
           else if(ui->ListWidget->row(ListWidgetItemList[i])>CurrentAudioOrVedio)//如果要删除的是当前音视频之后的，不进行操作
           {

           }
           if(isAudioOrVideo(ListWidgetItemList[i]->text())==1)
           {
               VideoVector.erase(VideoVector.begin()+FindAudioOrVideoByName(ListWidgetItemList[i]->text()));
           }
           else
           {
               AudioVector.erase(AudioVector.begin()+FindAudioOrVideoByName(ListWidgetItemList[i]->text()));
           }
           QListWidgetItem* j = ListWidgetItemList[i];
           delete j;
       }
       SaveLocalCache();
       return;
    }
    else if(ev->modifiers()==Qt::ControlModifier && ev->key()==Qt::Key_F)//全屏
    {
        //qDebug()<<"F";
        on_FullScreenButton_clicked();
    }

    else if(ev->modifiers()==Qt::ControlModifier && ev->key()==Qt::Key_Left)//上一首
    {
        //qDebug()<<"left";
        QString NextAudioOrVedio=getLastAudioOrVedio();
        PlayAudioAndVedio(NextAudioOrVedio);
    }
    else if(ev->modifiers()==Qt::ControlModifier && ev->key()==Qt::Key_Right)//下一首
    {
        //qDebug()<<"right";
        QString NextAudioOrVedio=getNextAudioOrVedio();
        PlayAudioAndVedio(NextAudioOrVedio);
    }
    else if(ev->modifiers()==Qt::ControlModifier && ev->key()==Qt::Key_I)//打开文件导入窗口
    {
        on_AddButton_clicked();
    }
    else if(ev->modifiers()==Qt::ControlModifier && ev->key()==Qt::Key_Down)//调节音量
    {
//        on_VoiceButton_clicked();
//        this->Volumew.ui->verticalSlider->setFocus();
//        int value=this->Volumew.ui->verticalSlider->value()*100;
//        value=value-1;
//        this->Volumew.ui->verticalSlider->setValue(double(value)/100);
//        SetVolumeValue(value);
    }
    else if(ev->key()==Qt::Key_Left)//逐帧后退
    {
        if(CurrentAudioOrVedio==-1)return;//必须要先选中视频
        if(NowPlayType==2)return;//当前播放的必须是视频

        /*暂停*/
        PlayOrStopTag=0;
        ui->PlayButton->setIcon(QIcon(":/images/Play.png"));
        player->pause();

        int TempFrameRate=VideoVector[FindAudioOrVideoByName(ui->ListWidget->item(CurrentAudioOrVedio)->text())].frame_rate.split("f")[0].toInt();
        qint64 move = (qint64)(this->player->position() - double(frame)*1000/TempFrameRate);
        if(move<0)move=0;//避免上溢
        player->setPosition(move);
    }
    else if(ev->key()==Qt::Key_Right)//逐帧快进
    {
        if(CurrentAudioOrVedio==-1)return;//必须要先选中视频
        if(NowPlayType==2)return;//当前播放的必须是视频

        /*暂停*/
        PlayOrStopTag=0;
        ui->PlayButton->setIcon(QIcon(":/images/Play.png"));
        player->pause();

        int TempFrameRate=VideoVector[FindAudioOrVideoByName(ui->ListWidget->item(CurrentAudioOrVedio)->text())].frame_rate.split("f")[0].toInt();
        qint64 move = (qint64)(this->player->position() + double(frame)*1000/TempFrameRate);
        if(move>this->player->duration())move=this->player->duration();//避免上溢
        player->setPosition(move);
    }
    else if(ev->key()==Qt::Key_Escape)//退出全屏
    {
        if(this->isFullScreen()==1)
        {
            on_FullScreenButton_clicked();
        }
    }
    else if(ev->key()==Qt::Key_Space)//开始|暂停
    {
        on_PlayButton_clicked();
    }


    QWidget::keyPressEvent(ev);
}

//“键盘松开”事件
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    QWidget::keyReleaseEvent(ev);
}



/*-------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------槽函数-----------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------*/
//“播放按钮槽函数”
void MainWindow::on_PlayButton_clicked()
{
    if(PlayOrStopTag==0)
    {
        player->play();
        PlayOrStopTag=1;
        ui->PlayButton->setIcon(QIcon(":/images/Stop.png"));

    }
    else if(PlayOrStopTag==1)
    {
        player->pause();
        PlayOrStopTag=0;
        ui->PlayButton->setIcon(QIcon(":/images/Play.png"));
    }
}

//“全屏”按钮槽函数
void MainWindow::on_FullScreenButton_clicked()
{
    if(this->isFullScreen()==0)//如果当前不是全屏
    {
        if(NowPlayType==1)
        {
            ui->FunctionWidget->setVisible(0);//功能栏隐藏
            this->setWindowFlags(Qt::FramelessWindowHint);//最小化、窗口化、退出按钮隐藏
            this->setCursor(Qt::BlankCursor); //隐藏鼠标
        }
        ui->PlayListWidget->setVisible(0);//将播放列表设置为不可见
        this->showFullScreen();//当前窗口全屏
        ui->FullScreenButton->setIcon(QIcon(":/images/SmallScreen.png"));//改变全屏按钮的图表
        ui->centralwidget->layout()->setContentsMargins(0,0,0,0);//将边框设置为0
        ui->VideoScreenLabel->setFrameShape(QFrame::Shape(QFrame::NoFrame));//将播放标签设置为无边框
    }
    else//如果当前是全屏
    {
        if(NowPlayType==1)
        {
            ui->FunctionWidget->setVisible(1);//功能栏出现
            this->setCursor(Qt::ArrowCursor); //显示鼠标
            this->setWindowFlags(Qt::Window);//最小化、窗口化、退出按钮出现
        }
        this->showNormal();//恢复正常尺寸
        FunctionWidgetTimer->stop();//计时器停止即使，避免功能栏被隐藏
        ui->FullScreenButton->setIcon(QIcon(":/images/FullScreen.png"));//改变全屏按钮的图表
        ui->centralwidget->layout()->setContentsMargins(11,11,11,11);//将边框设置为11
        //ui->VideoScreenLabel->setFrameShape(QFrame::Shape(QFrame::WinPanel));//将播放标签设置为有边框
        this->resize(847,580);//设置为原大小
        //使得屏幕居中显示
        QScreen *screen = QGuiApplication::primaryScreen ();
        QRect screenRect =  screen->availableVirtualGeometry();
        this->move((screenRect.width() - this->width())/ 2, (screenRect.height() - this->height()) /2);
    }
    if(NowPlayType==2)
    {
        int AlbumSizeH=ui->MainWidget->size().height()/2;
        int AlbumSizeW=ui->MainWidget->size().width()/2;
        if(AlbumSizeH>AlbumSizeW)
        {
            AlbumSizeH=AlbumSizeW;
        }
        else
        {
            AlbumSizeW=AlbumSizeH;
        }
        QSize AlbumSize(AlbumSizeW,AlbumSizeH);
        ui->AlbumCoverLabel->setPixmap(QPixmap::fromImage(AudioVector[FindAudioOrVideoByName(ui->ListWidget->item(this->CurrentAudioOrVedio)->text())].cover).scaled(AlbumSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    }
}

//计时器槽函数
void MainWindow::on_timer_timeout()//计时器时间到
{
    if(NowPlayType==1)
    {
        ui->FunctionWidget->setVisible(0);//隐藏功能栏
        ui->PlayListWidget->setVisible(0);//隐藏播放列表
        FunctionWidgetTimer->stop();//停止计时
        this->setCursor(Qt::BlankCursor); //隐藏鼠标
    }
}

//显示播放列表按钮槽函数
void MainWindow::on_PlayListButton_clicked()
{
    if(ui->PlayListWidget->isVisible()==0)
    {
        ui->PlayListWidget->setVisible(1);
    }
    else
    {
        ui->PlayListWidget->setVisible(0);
    }
}


//“播放”按钮槽函数
void MainWindow::on_ModeButton_clicked()
{
    if(PlaySeqence==1)
    {
        PlaySeqence++;
        ui->ModeButton->setText("顺序播放");
        ui->ModeButton->setIcon(QIcon(":/images/PlayInOrder.png"));
        ui->ModeButton->setIconSize(QSize(17,16));
    }
    else if(PlaySeqence==2)
    {
        PlaySeqence++;
        ui->ModeButton->setText("随机播放");
        ui->ModeButton->setIcon(QIcon(":/images\\PlayRandom.png"));
        ui->ModeButton->setIconSize(QSize(17,16));
    }
    else if(PlaySeqence==3)
    {
        PlaySeqence++;
        ui->ModeButton->setText("单曲循环");
        ui->ModeButton->setIcon(QIcon(":/images\\PlaySingleCycle.png"));
        ui->ModeButton->setIconSize(QSize(17,16));
    }
    else
    {
        PlaySeqence=1;
        ui->ModeButton->setText("单曲播放");
        ui->ModeButton->setIcon(QIcon(":/images\\PlaySingle.png"));
        ui->ModeButton->setIconSize(QSize(17,16));
    }
}

//“下一个”按钮槽函数
void MainWindow::on_NextVedioButton_clicked()
{
    QString NextAudioOrVedio=getNextAudioOrVedio();
    PlayAudioAndVedio(NextAudioOrVedio);
}

//"上一个"按钮槽函数
void MainWindow::on_LastVedioButton_clicked()
{
    QString NextAudioOrVedio=getLastAudioOrVedio();
    PlayAudioAndVedio(NextAudioOrVedio);
}

void MainWindow::SetCurrentListItemColor(QColor q)
{
    ui->ListWidget->item(CurrentAudioOrVedio)->setBackground(q);
}

//“添加文件”按钮槽函数
void MainWindow::on_AddButton_clicked()
{
    QString CurPath=QDir::currentPath();//将选择的路径先设置为当前文件夹
    QString Title="选择音视频文件";//标题
    QString Filter="所有文件(*.*);;视频文件(*.avi *.wmv *.mp4);;音频文件(*.mp3 *.flac);;歌词文件(*.lrc);;";//文件过滤器
    QStringList fileList= QFileDialog::getOpenFileNames(this,Title,CurPath,Filter);
    QString RepeatedErrorFileName="";
    QString LRCErrorFileName="";
    int isRepeatedError=0;
    int isLRCError=0;
    for(int i=0;i<fileList.count();i++)
    {
        if(isFileNameRepeated(QFileInfo(fileList[i]).fileName()))//如果文件名重复了
        {
            isRepeatedError=1;
            RepeatedErrorFileName+=QFileInfo(fileList[i]).fileName();
            RepeatedErrorFileName+="\n";
        }
        else//如果没有重复
        {
            if(isAudioOrVideo(QFileInfo(fileList[i]).fileName())==3)//如果是歌词文件
            {
                if(setLRC(QFileInfo(fileList[i]).filePath()))
                {
                    //setListWidgetLRC(QFileInfo(fileList[i]).fileName());
                }
                else
                {
                    isLRCError=1;
                    LRCErrorFileName+=QFileInfo(fileList[i]).fileName();
                    LRCErrorFileName+="\n";
                }
            }
            else
            {
                AddListItem(fileList[i]);//在播放列表显示
                AddAudioOrVideo(fileList[i]);//载入数组
            }
        }
    }
    if(isRepeatedError==1)QMessageBox::critical(this, tr("错误"), tr("以下文件：\n\n%1\n文件名重复,读入失败").arg(RepeatedErrorFileName));//如果出现了错误文件
    if(isLRCError==1)QMessageBox::critical(this, tr("错误"), tr("以下歌词文件：\n\n%1\n未找到对应音频,读入失败").arg(LRCErrorFileName));//如果出现了错误文件
    SaveLocalCache();
}

void MainWindow::getduration(qint64 playtime)
{
    if(!m_sliderstate)
    {
        playtime = player->duration();
        setget_Alltime(playtime);
    }
}

void MainWindow::getposition(qint64 playtime)
{
    if(!m_sliderstate)
    {
        playtime = player->position();
        setget_currenttime(playtime);
    }
}

void MainWindow::UpdateCurrentAudioOrVedio()
{
    for(int i=0;i<ui->ListWidget->count();i++)
    {
        if(ui->ListWidget->item(i)->background()==QBrush(QColor(255,182,193)))
        {
            CurrentAudioOrVedio=i;
            break;
        }
    }
    SaveLocalCache();
}


void MainWindow::on_VoiceButton_clicked()
{
    Volumew.setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
    //sw.setWindowFlags(sw.windowFlags() | Qt::WindowStaysOnTopHint);
    //Volumew.move(this->pos()+ui->FunctionWidget->pos()+ui->ButtonWidget->pos()+ui->ModeWidget->pos()+ui->VoiceButton->pos());
    //Volumew.move(this->pos()+ui->centralwidget->pos()+ui->MainWidget->pos()+ui->FunctionWidget->pos()+ui->ButtonWidget->pos()+ui->ModeWidget->pos()+ui->VoiceButton->pos());
    Volumew.move(this->geometry().x()+ui->centralwidget->geometry().x()+ui->MainWidget->geometry().x()+ui->FunctionWidget->geometry().x()+ui->ButtonWidget->geometry().x()+ui->ModeWidget->geometry().x()+ui->VoiceButton->geometry().x()-4,this->geometry().y()+ui->centralwidget->geometry().y()+ui->MainWidget->geometry().y()+ui->FunctionWidget->geometry().y()+ui->ButtonWidget->geometry().y()+ui->ModeWidget->geometry().y()+ui->VoiceButton->geometry().y()-95);
    Volumew.show();
}

void MainWindow::on_SetButton_clicked()
{
    sw.setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
    //sw.setWindowFlags(sw.windowFlags() | Qt::WindowStaysOnTopHint);
    sw.move(this->geometry().x()+ui->centralwidget->geometry().x()+ui->MainWidget->geometry().x()+ui->FunctionWidget->geometry().x()+ui->ButtonWidget->geometry().x()+ui->ModeWidget->geometry().x()+ui->SetButton->geometry().x()-4,this->geometry().y()+ui->centralwidget->geometry().y()+ui->MainWidget->geometry().y()+ui->FunctionWidget->geometry().y()+ui->ButtonWidget->geometry().y()+ui->ModeWidget->geometry().y()+ui->SetButton->geometry().y()-98);
    sw.show();
}
void MainWindow::SetVolumeValue(int value)
{
    //qDebug()<<"set";
    audioOutput->setVolume(float(value)/100);
    if(value==0)
    {
        ui->VoiceButton->setIcon(QIcon(":/images/Mute.png"));
    }
    else
    {
        ui->VoiceButton->setIcon(QIcon(":/images/Voice.png"));
    }
}

void MainWindow::SetFrameValue(int frame)
{
    this->frame=frame;
}

void MainWindow::ShowFrame(double value)
{
    if(NowPlayType!=1)return;//如果是音频，不显示

    ShowFrameCount++;
    if(ShowFrameCount<=5)
    {
        return;
    }
    else
    {
        ShowFrameCount=0;
    }

    /*窗口设置*/
    gf.setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
    gf.move(this->geometry().x()+ui->centralwidget->geometry().x()+ui->MainWidget->geometry().x()+ui->FunctionWidget->geometry().x()+ui->ProcessWidget->geometry().x()+ui->ProgressSlider->geometry().x()+ui->ProgressSlider->size().width()*value,this->geometry().y()+ui->centralwidget->geometry().y()+ui->MainWidget->geometry().y()+ui->FunctionWidget->geometry().y()+ui->ProcessWidget->geometry().y()+ui->ProgressSlider->geometry().y()-ui->ProgressSlider->size().height()-gf.size().height());

    /*提取具体帧*/
    Video tempv=VideoVector[FindAudioOrVideoByName(ui->ListWidget->item(CurrentAudioOrVedio)->text())];
    QString file=tempv.GetPath().toLatin1();
    //const char* file_path = tempv.GetPath().toLatin1();

    int l=file.length();
    char* file_path;
    file_path=new char[l];
    for(int i=0;i<l;i++)
    {
        file_path[i]=file[i].toLatin1();
    }
    file_path[l]='\0';

    //qDebug()<<file_path;

    int ret = -1, index = 0, image_size = -1;
    int videoStream = -1, got_picture = -1, numBytes= -1;
    uint8_t *out_buffer = NULL;

    AVFormatContext *pFormatCtx = NULL;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;
    AVPacket *packet = NULL;
    AVFrame *pFrame = NULL, *pFrameRGB = NULL;
    struct SwsContext *img_convert_ctx = NULL;

    av_register_all();
    pFormatCtx = avformat_alloc_context();
    if(NULL == pFormatCtx){
        //qDebug() << "avformat_alloc_context() failed.";
    }

    // 1. 打开视频文件;
    ret = avformat_open_input(&pFormatCtx, file_path, NULL, NULL);
    if(ret < 0){
        //qDebug() << "avformat_open_input() failed.";
    }

    // 2. 读取视频文件信息;
    ret = avformat_find_stream_info(pFormatCtx, NULL);
    if (ret < 0) {
        //qDebug() << "avformat_find_stream_info() failed.";
    }

    // 3. 获取视频流
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
        }
    }
    if (videoStream == -1) {
        //qDebug() << "coun't find a video stream.";
    }

    // 4. 根据上面得到的视频流类型打开对应的解码器
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        //qDebug() <<"Decoder not found.";
    }
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        //qDebug() <<"Ddecoder couldn't open.";
    }

    // 5. 分配并初始化一个视频packet
    image_size = pCodecCtx->width * pCodecCtx->height;
    packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
    if(!packet){
        //qDebug() <<"malloc() failed.";
    }
    ret = av_new_packet(packet, image_size);
    if(ret < 0){
        //qDebug() <<"av_new_packet() failed.";
    }

    // 6. 分配两个frame分别存放yuv和rgb数据
    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();
    if(!pFrame || !pFrameRGB){
//        qDebug() <<"pFrame:" << pFrame
//               <<", pFrameRGB:" << pFrameRGB
//              <<" av_frame_alloc() failed";
    }

    // 7. 分配一个struct SwsContext结构体, 填充源图像和目标图像信息(为接下来的转换做准备)
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
                pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
                AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

    // 8. pFrameRGB和out_buffer都是已经申请到的一段内存, 会将pFrameRGB的数据按RGB24格式自动"关联(转换并放置)"到out_buffer。
    numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width,pCodecCtx->height);
    out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    ret = avpicture_fill((AVPicture *) pFrameRGB, out_buffer, AV_PIX_FMT_RGB24,
                pCodecCtx->width, pCodecCtx->height);
    if (!img_convert_ctx || numBytes<0 || !out_buffer || ret<0) {
//        qDebug() <<"img_convert_ctx:" << img_convert_ctx
//               <<", numBytes:" << numBytes
//              <<", out_buffer:" << out_buffer
//             <<", ret:" <<ret;
    }

    // 9. 按时间寻找需要的帧
    ret = av_seek_frame(pFormatCtx, -1, int(player->duration()*value/1000) * AV_TIME_BASE, AVSEEK_FLAG_BACKWARD); // this->preview_frame_time，单位为s，即为预览图所在时间点

    while(1){
        if (av_read_frame(pFormatCtx, packet) < 0) {
            break;
        }

        if (packet->stream_index == videoStream) {
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,packet);
            if (ret < 0) {
                //qDebug() << "decode error.";
            }
        }

        if (got_picture) {
            if(pFrame->key_frame == 1)  // 提取关键帧
            {
                sws_scale(img_convert_ctx,
                        (uint8_t const * const *) pFrame->data,
                        pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                        pFrameRGB->linesize);

                // 将提取的关键帧显示到label_preview上
                QImage image(pFrameRGB->data[0], pCodecCtx->width, pCodecCtx->height, pFrameRGB->linesize[0], QImage::Format_RGB888);
                gf.ui->label->setPixmap(QPixmap::fromImage(image));
                gf.ui->label->setScaledContents(1);

            }
        }

        index++;
        if(index > 100){
            break;
        }
    }

    gf.show();
}

void MainWindow::CloseFrame()
{
    if(NowPlayType==2)return;
    gf.close();
}

void MainWindow::SetPlaySpeed(int playspeed)
{
    player->setPlaybackRate(playspeed);
}

