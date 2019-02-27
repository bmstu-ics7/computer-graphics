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
    QPainter p(this);

    int canvasWidth = width() - 200;
    int canvasHeight = height();

    p.fillRect(0, 0, canvasWidth, canvasHeight, Qt::white);

    int R = 100;
    QPair<int, int> center = QPair<int, int>(canvasWidth / 2, canvasHeight / 2);

    p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
    p.drawEllipse(center.first - R, center.second - R, R * 2, R * 2);

    p.drawEllipse(center.first - R / 3 - R / 8, center.second - R / 2, R / 3, R / 4);
    p.drawEllipse(center.first + R / 3 - R / 8, center.second - R / 2, R / 3, R / 4);

}
