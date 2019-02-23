#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QListWidget>
#include <QMessageBox>

#include <cmath>
#include <math.h>

#define RADIAN 57.2958

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);

private slots:
    void on_btnAdd_clicked();

    void on_btnDelAll_clicked();

    void on_btnDel_clicked();

    void on_btnEdit_clicked();

private:
    Ui::MainWindow *ui;
    bool checkTwoInt(QString text);
};

#endif // MAINWINDOW_H
