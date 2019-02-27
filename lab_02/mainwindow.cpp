#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *) {
    int canvasWidth = width() - 200;
    int canvasHeight = height();

    QPainter p(this);
    p.fillRect(0, 0, canvasWidth, canvasHeight, Qt::white);

    QPair<int, int> center(canvasWidth / 2, canvasHeight / 2);
    int R = 200;

    p.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    p.drawEllipse(center.first - R, center.second - R, R * 2, R * 2);

    p.drawEllipse(center.first - R / 2, center.second - R / 5 * 2, R / 3, R / 5);
    p.drawEllipse(center.first + R / 2 - R / 3, center.second - R / 5 * 2, R / 3, R / 5);

    QList<QPoint> triangleFirts;
    triangleFirts.append(QPoint(center.first, center.second - R / 8));
    triangleFirts.append(QPoint(center.first - R / 6, center.second + R / 4));
    triangleFirts.append(QPoint(center.first + R / 6, center.second + R / 4));
    p.drawLine(triangleFirts[0], triangleFirts[1]);
    p.drawLine(triangleFirts[1], triangleFirts[2]);
    p.drawLine(triangleFirts[2], triangleFirts[0]);

    QList<QPoint> triangleSecond;
    triangleSecond.append(QPoint(center.first, center.second + R / 3 * 2));
    triangleSecond.append(QPoint(center.first - R / 3, center.second + R / 3));
    triangleSecond.append(QPoint(center.first + R / 3, center.second + R / 3));
    p.drawLine(triangleSecond[0], triangleSecond[1]);
    p.drawLine(triangleSecond[1], triangleSecond[2]);
    p.drawLine(triangleSecond[2], triangleSecond[0]);

    p.setPen(QPen(Qt::blue, 2, Qt::SolidLine));
    QLine hair1(center.first, center.second - R, center.first, center.second - R / 9 * 5);
    p.drawLine(hair1);

    QLine hair2(center.first + R / 10, center.second - R + R / 60, center.first + R / 4, center.second - R / 5 * 3);
    p.drawLine(hair2);

    QLine hair3(center.first - R / 10, center.second - R + R / 60, center.first - R / 4, center.second - R / 5 * 3);
    p.drawLine(hair3);

    QLine hair4(center.first + R / 4, center.second - R + R / 30, center.first + R / 2, center.second - R / 7 * 5);
    p.drawLine(hair4);

    QLine hair5(center.first - R / 4, center.second - R + R / 30, center.first - R / 2, center.second - R / 7 * 5);
    p.drawLine(hair5);
}
