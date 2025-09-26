#ifndef SUPPORTWINDOW_H
#define SUPPORTWINDOW_H

#include <QWidget>



namespace Ui {
class supportwindow;
}

class supportwindow : public QWidget
{
    Q_OBJECT

public:
    explicit supportwindow(QWidget *parent = nullptr);
    ~supportwindow();

private slots:
    void on_closeBu_clicked();

private:
    Ui::supportwindow *ui;
};

#endif // SUPPORTWINDOW_H
