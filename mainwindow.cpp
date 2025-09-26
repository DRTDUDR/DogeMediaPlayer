#include "mainwindow.h"
#include "supportwindow.h"

#include "ui_mainwindow.h"
#include<QGridLayout>
#include<QPushButton>
#include <QInputDialog>
//#include <QColorDialog>
//#include <QFontDialog>
#include <QMediaPlayer>
#include <QMessageBox>
#include <iostream>
#include <QMenuBar>
#include <QToolBar>
#include <QTime>
#include <QDebug>
#include <QDate>

static QDate buildDate;
static QTime buildTime;
QString Version = " 0.6.0_rc";

int flag = 0;
QString fileDirection;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QString Title = "DogeMediaPlayer "+Version;


    setWindowTitle(Title);

    QMenuBar *bar = menuBar();
    this->setMenuBar(bar);  //将菜单栏放入主窗口

    //supportwin = new supportwindow();

    getBuildTime();

    player = new QMediaPlayer(this);//创建视频播放器
    player->setNotifyInterval(1000);//信息更新周期
    player->setVideoOutput(ui->videoPlayer);//视频显示组件


    ui->videoPlayer->installEventFilter(this);
    ui->videoPlayer->setFocusPolicy(Qt::StrongFocus);


    //ui->videoPlayer->setMediaPlayer(player);//设置显示组件的关联播放器

    //ui->pushBu_start->setShortcut(Qt::Key_Space);          // 设置快捷键Space
    //ui->pushBu_toStart->setShortcut(Qt::Key_Left);          // 设置快捷键Left
    //ui->pushBu_toEnd->setShortcut(Qt::Key_Right);           // 设置快捷键Right

    QMenu *fileMenu = bar->addMenu("  文件  ");
    QAction *selectAction = fileMenu->addAction("选择文件");
    selectAction->setIcon(QIcon(":/image/icon/browse-folder-48.png"));
    selectAction->setShortcut(Qt::CTRL | Qt::Key_O);          // 设置快捷键ctrl+o
    connect(selectAction, &QAction::triggered, this, &MainWindow::on_selectAction_clicked);
    fileMenu->addSeparator();

    QAction *inputAction = fileMenu->addAction("网络播放");
    inputAction->setIcon(QIcon(":/image/icon/code-folder-48.png"));
    inputAction->setShortcut(Qt::CTRL | Qt::Key_P);          // 设置快捷键ctrl+p
    connect(inputAction, &QAction::triggered, this, &MainWindow::on_inputAction_clicked);
    fileMenu->addSeparator();

    QMenu * settingMenu = bar->addMenu("  选项  ");
    QAction *settingAction = settingMenu->addAction("  设置  ");
    settingAction->setIcon(QIcon(":/image/icon/setting-48.png"));
    settingAction->setShortcut(Qt::CTRL | Qt::Key_S);          // 置快捷键ctrl+s
    connect(settingAction, &QAction::triggered, this, &MainWindow::on_settingAction_clicked);
    settingMenu->addSeparator();

    QAction *supportAction = settingMenu->addAction("  支持  ");
    supportAction->setIcon(QIcon(":/image/icon/support-48.png"));
    //supportAction->setShortcut(Qt::CTRL | Qt::Key_S);
    connect(supportAction, &QAction::triggered, this, &MainWindow::on_supportAction_clicked);
    settingMenu->addSeparator();

    QAction *aboutAction = settingMenu->addAction("  关于  ");
    aboutAction->setIcon(QIcon(":/image/icon/about-48.png"));
    //supportAction->setShortcut(Qt::CTRL | Qt::Key_S);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::on_aboutAction_clicked);
    settingMenu->addSeparator();

    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this, SLOT(onstateChanged(QMediaPlayer::State)));
    connect(player,SIGNAL(positionChanged(qint64)),this, SLOT(onPositionChanged(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),this, SLOT(onDurationChanged(qint64)));

    ui->volumnSlider->setSliderPosition(100);

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

}
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    // 首先检查事件类型是否是键盘按下
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Escape)
        {
            if (ui->videoPlayer->isFullScreen())
            {
                ui->videoPlayer->setFullScreen(false);
                event->accept();
            }
        }
        else if (keyEvent->key() == Qt::Key_Space)
        {
            on_pushBu_start_clicked();
            event->accept();
        }
        else if (keyEvent->key() == Qt::Key_Left)
        {
            on_pushBu_toStart_clicked();
            event->accept();
        }
        else if (keyEvent->key() == Qt::Key_Right)
        {
            on_pushBu_toEnd_clicked();
            event->accept();
        }
    }

    // 对于所有其他事件，调用基类的 eventFilter
    return QMainWindow::eventFilter(watched, event);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::onPositionChanged(qint64 position)
{//文件播放位置变化
    if (ui->videoSlider->isSliderDown())
        return; //如果正在拖动滑条，退出

    ui->videoSlider->setSliderPosition(position);

    int   secs=position/1000;//秒
    int   mins=secs/60; //分钟
    secs=secs % 60;//余数秒
    positionTime=QString::asprintf("%d:%d",mins,secs);
    ui->label->setText(positionTime+"/"+durationTime);
}

void MainWindow::onDurationChanged(qint64 duration)
{//文件时长变化
    ui->videoSlider->setMaximum(duration);

    int   secs=duration/1000;//秒
    int   mins=secs/60; //分钟
    secs=secs % 60;//余数秒
    durationTime=QString::asprintf("%d:%d",mins,secs);
    ui->label->setText(positionTime+"/"+durationTime);
}

void MainWindow::on_videoSlider_valueChanged(int value)
{
    player->setPosition(value);
    ui->videoPlayer->setFocus();
}

void MainWindow::on_volumnSlider_valueChanged(int value)
{//调节音量
    player->setVolume(value);
    ui->videoPlayer->setFocus();
}


void MainWindow::on_selectAction_clicked()
{

    qDebug()<<"选择文件";
    fileDirection = QFileDialog::getOpenFileName(this,"打开文件","/","video files(*.avi *.mp4 *.wmv);;All files(*.*)");
    videoPlay(1);
}

void MainWindow::on_inputAction_clicked()
{
    qDebug()<<"网络视频";
    showNetVideoDlg();
    videoPlay(2);
}

void MainWindow::on_settingAction_clicked()
{
    qDebug()<<"设置   ";
}
void MainWindow::on_supportAction_clicked()
{
    qDebug()<<"支持   ";
    //supportwin->show();
    showSupportDlg();
}

void MainWindow::on_aboutAction_clicked()
{
    qDebug()<<"关于   ";
    showAboutDlg();
}

void MainWindow::on_pushBu_start_clicked()
{

    if(flag==0)
    {
        if(fileDirection!=NULL)
        {
            player->play();
            ui->pushBu_start->setStyleSheet("QPushButton{border-image: url(:/image/icon/stop-48.png);}");
            flag=1;
            qDebug()<<"播放"<<"flag::::"<<flag;
        }
        else
            qDebug()<<"未播放"<<"flag::::"<<flag;
    }
    else if(flag==1)
    {
        player->pause();
        ui->pushBu_start->setStyleSheet("QPushButton{border-image: url(:/image/icon/play-48.png);}");
        flag=0;
        qDebug()<<"暂停"<<"flag::::"<<flag;
    }

}

void MainWindow::on_pushBu_toStart_clicked()
{
    if (player->state() == QMediaPlayer::PlayingState)
    {
        int value = player->position();
        value=value-5000;
        if(value<0)
        {
            value=0;
        }
        player->setPosition(value);
    }
    qDebug()<<"快退";
}

void MainWindow::on_pushBu_toEnd_clicked()
{
    if (player->state() == QMediaPlayer::PlayingState)
    {
        int value = player->position();
        value=value+5000;
        if(value>=player->duration())
        {
            value=player->duration();
        }
        player->setPosition(value);
    }
    qDebug()<<"快进";
}

void MainWindow::on_fullScBut_clicked()
{//全屏按钮
    ui->videoPlayer->setFullScreen(true);
    ui->videoPlayer->setFocus();
}




void MainWindow::getBuildTime()
{
    buildDate = QLocale( QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
    buildTime = QTime::fromString(__TIME__, "hh:mm:ss");

    qDebug() << QString("%1 %2").arg(buildDate.toString("yyyy.MM.dd")).arg(buildTime.toString());
}


void MainWindow::videoPlay(int type)
{
    if(type==1)
        player->setMedia(QUrl::fromLocalFile(fileDirection));//设置播放文件
    if(type==2)
        player->setMedia(QMediaContent(QUrl(fileDirection)));
    ui->pushBu_start->setStyleSheet("QPushButton{border-image: url(:/image/icon/stop-48.png);}");
    player->play();
    flag =1;

}
void MainWindow::videoPause()
{
    ui->pushBu_start->setStyleSheet("QPushButton{border-image: url(:/image/icon/play-48.png);}");
    player->pause();
    flag =0;
}








void  MainWindow::mousePressEvent(QMouseEvent *event)
{//鼠标事件，单击控制暂停和继续播放
//    if (event->button()==Qt::LeftButton)
//    {
//        if (player->state()==QMediaPlayer::PlayingState)
//            player->pause();
//        else
//            player->play();
//    }
//    MainWindow::mousePressEvent(event);

}

void MainWindow::showAboutDlg()
{
    QMessageBox aboutMsgBox;
    aboutMsgBox.setWindowTitle(tr("关于"));	//设置消息框的标题

    QPushButton *yesBtn=aboutMsgBox.addButton(tr("Doge!!!!!!!!!!!!!!!!!!!"),QMessageBox::ActionRole);
    //supportMsgBox.

    aboutMsgBox.setText(tr("<html><b>DogeMediaPlayer %1</b></html><br><br>"
                           "Built with Qt 5.14.2 (MinGW, 64-bit)<br>"
                           "Build Date: %2 %3<br><br>"
                           "Copyright (C) 2023, DRTDUDR.<br>"
                           "Licensed under the MIT License."
                             ).arg(Version).arg(buildDate.toString("yyyy.MM.dd")).arg(buildTime.toString())
                        );
    aboutMsgBox.setIconPixmap(QPixmap(":/image/icon/DogeMediaPlayer-128x.png"));
    aboutMsgBox.exec();
    return;
}

void MainWindow::showSupportDlg()
{
    QMessageBox supportMsgBox;
    supportMsgBox.setWindowTitle(tr("支持"));	//设置消息框的标题

    QPushButton *yesBtn=supportMsgBox.addButton(tr("好的捏"),QMessageBox::ActionRole);

    supportMsgBox.setText(tr("<p style='text-align: center;'><img src='qrc://image/icon/DogeMediaPlayer-256x.png'></p>"

                             "<p style='text-align: center;'><html><b>DogeMediaPlayer %1 </b></html><br>"
                             "<p style='text-align: center;'>不提供可靠支持！！！！！！！"
                             "<p style='text-align: center;'>Bug不一定会修！！！！！！！"
                             "<p style='text-align: center;'>给你源码自己改！！！！！！！<br>"
                             "<p style='text-align: center;'>源代码请访问：<br>"
                             "<a href='https://github.com/DRTDUDR/DogeMediaPlayer'>https://github.com/DRTDUDR/DogeMediaPlayer</a>"
                             ).arg(Version).arg(buildDate.toString("yyyy.MM.dd")).arg(buildTime.toString())
                          );

    //supportMsgBox.setIconPixmap(QPixmap(":/image/icon/DogeMediaPlayer-128x.png"));
    supportMsgBox.exec();
    return;
}

QString MainWindow:: showNetVideoDlg(QWidget *parent) {
    QString url;

    QDialog *dialog = new QDialog(parent);
    dialog->setWindowTitle(tr("输入链接"));

    QLineEdit *urlLineEdit = new QLineEdit(dialog);
    QPushButton *okButton = new QPushButton(tr("确定"), dialog);
    QPushButton *cancelButton = new QPushButton(tr("取消"), dialog);

    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->addWidget(urlLineEdit);
    mainLayout->addLayout(buttonLayout);

    if (dialog->exec() == QDialog::Accepted) {
        url = urlLineEdit->text();
    }
    fileDirection=url;
    delete dialog;
    return url;
}
