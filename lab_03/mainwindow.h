#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMessageBox>
#include <QColorDialog>

#include "line.h"

#define RADIANS 57.2958
#define PI 3.1415926535

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*);

private slots:
    void on_btnLine_clicked();

    void on_btnBundle_clicked();

    void on_btnClear_clicked();

    void on_btnColor_clicked();

    void on_btnBColor_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<Line> lines;

    QColor currentLineColor;
    QColor currentBackgroundColor;
};

#endif // MAINWINDOW_H
