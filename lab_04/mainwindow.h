#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QColorDialog>
#include <QList>
#include <QMessageBox>

#include "circle.h"
#include "ellipse.h"

#define MARGIN_RIGHT 305
#define MARGIN_UP 14

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
    void on_btnBackground_clicked();

    void on_btnForeground_clicked();

    void on_btnOneColor_clicked();

    void on_btnClear_clicked();

    void on_btnCircle_clicked();

    void on_btnCircles_clicked();

    void on_btnEllipse_clicked();

    void on_btnEllipses_clicked();

private:
    Ui::MainWindow *ui;
    QColor background;
    QColor foreground;
    QList<Figure*> figures;
};

#endif // MAINWINDOW_H
