#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qdialog.h"
#include "supportwindow.h"
#include <QMainWindow>
#include <QLineEdit>
#include <QGridLayout>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QProgressBar>
#include <QFileDialog>
#include <QLabel>
#include <QKeyEvent>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);



    ~MainWindow();

private:
    Ui::MainWindow *ui;
    supportwindow *supportwin;

    QPushButton *fileBtn;
    QLineEdit *fileLineEdit;
    QGridLayout *mainLayout;
    QLabel *label;
    QMediaPlayer *player;//视频播放器
    QString durationTime;//
    QString positionTime;//



private slots:
    void showAboutDlg();
    void showSupportDlg();
    QString showNetVideoDlg(QWidget *parent = nullptr);
    void videoPlay(int type);
    void videoPause();
    void getBuildTime();

    bool eventFilter(QObject *watched, QEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void onPositionChanged(qint64 duration);
    void onDurationChanged(qint64 duration);
    void on_videoSlider_valueChanged(int value);
    void on_volumnSlider_valueChanged(int value);

    void on_selectAction_clicked();
    void on_settingAction_clicked();
    void on_aboutAction_clicked();
    void on_inputAction_clicked();
    void on_supportAction_clicked();

    void on_pushBu_start_clicked();
    void on_pushBu_toStart_clicked();
    void on_pushBu_toEnd_clicked();
    void on_fullScBut_clicked();

};
#endif // MAINWINDOW_H
