#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scaleX.append(1);
    scaleY.append(1);
    offsetX.append(0);
    offsetY.append(0);
    rotate.append(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    int canvasWidth = width() - 200;
    int canvasHeight = height();

    QPainter p(this);
    p.fillRect(0, 0, canvasWidth, canvasHeight, Qt::white);

    QPair<int, int> centerCoordinate(int(canvasWidth / 2), int(canvasHeight / 2));

    int RX = int(200 * scaleX[scaleX.count() - 1]);
    int RY = int(200 * scaleY[scaleY.count() - 1]);

    double angle = rotate[rotate.count() - 1];

    p.setPen(QPen(Qt::gray, 1, Qt::SolidLine));
    p.drawLine(centerCoordinate.first, 0, centerCoordinate.first, canvasHeight);
    p.drawLine(0, centerCoordinate.second, canvasWidth, centerCoordinate.second);

    p.rotate(-angle);
    double Cx = 0;
    double Cy = 0;

    p.setPen(QPen(Qt::black, 4, Qt::SolidLine));

    int x = int(canvasWidth / 2 + offsetX[offsetX.count() - 1]);
    int y = int(canvasHeight / 2 - offsetY[offsetY.count() - 1]);

    QPair<int, int> center;
    center = QPair<int, int>(int(Cx + (x - Cx) * cos(angle / RADIANS) - (y - Cy) * sin(angle / RADIANS)),
                             int(Cy + (x - Cx) * sin(angle / RADIANS) + (y - Cy) * cos(angle / RADIANS)));

    p.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    p.drawEllipse(center.first - RX, center.second - RY, RX * 2, RY * 2);

    p.drawEllipse(center.first - RX / 2, center.second - RY / 5 * 2, RX / 3, RY / 5);
    p.drawEllipse(center.first + RX / 2 - RX / 3, center.second - RY / 5 * 2, RX / 3, RY / 5);

    QList<QPoint> triangleFirts;
    triangleFirts.append(QPoint(center.first, center.second - RY / 8));
    triangleFirts.append(QPoint(center.first - RX / 6, center.second + RY / 4));
    triangleFirts.append(QPoint(center.first + RX / 6, center.second + RY / 4));
    p.drawLine(triangleFirts[0], triangleFirts[1]);
    p.drawLine(triangleFirts[1], triangleFirts[2]);
    p.drawLine(triangleFirts[2], triangleFirts[0]);

    QList<QPoint> triangleSecond;
    triangleSecond.append(QPoint(center.first, center.second + RY / 3 * 2));
    triangleSecond.append(QPoint(center.first - RX / 3, center.second + RY / 3));
    triangleSecond.append(QPoint(center.first + RX / 3, center.second + RY / 3));
    p.drawLine(triangleSecond[0], triangleSecond[1]);
    p.drawLine(triangleSecond[1], triangleSecond[2]);
    p.drawLine(triangleSecond[2], triangleSecond[0]);

    p.setPen(QPen(Qt::blue, 2, Qt::SolidLine));
    QLine hair1(center.first, center.second - RY, center.first, center.second - RY / 9 * 5);
    p.drawLine(hair1);

    QLine hair2(center.first + RX / 10, center.second - RY + RY / 60, center.first + RX / 4, center.second - RY / 5 * 3);
    p.drawLine(hair2);

    QLine hair3(center.first - RX / 10, center.second - RY + RY / 60, center.first - RX / 4, center.second - RY / 5 * 3);
    p.drawLine(hair3);

    QLine hair4(center.first + RX / 4, center.second - RY + RY/ 30, center.first + RX / 2, center.second - RY / 7 * 5);
    p.drawLine(hair4);

    QLine hair5(center.first - RX / 4, center.second - RY + RY / 30, center.first - RX / 2, center.second - RY / 7 * 5);
    p.drawLine(hair5);
}

double MainWindow::checkDouble(QString text, bool* key)
{
    text.replace(" ", "");

    double result = text.toDouble(key);

    if (!(*key))
        QMessageBox::critical(this, "Ошибка", "Некорректный ввод!");

    return result;
}

void MainWindow::on_btnOffseX_clicked()
{
    QString text = ui->lineOffsetX->text();
    bool key = true;
    double offsetX = checkDouble(text, &key);
    ui->lineOffsetX->clear();

    if (!key)
        return;

    ui->listActions->addItem("Смещение по x: " + QString::number(offsetX));

    this->scaleX.append(this->scaleX[this->rotate.count() - 1]);
    this->scaleY.append(this->scaleY[this->rotate.count() - 1]);
    this->offsetX.append(this->offsetX[this->offsetX.count() - 1] + offsetX);
    this->offsetY.append(this->offsetY[this->offsetY.count() - 1]);
    this->rotate.append(this->rotate[this->rotate.count() - 1]);

    repaint();
}

void MainWindow::on_btnOffsetY_clicked()
{
    QString text = ui->lineOffsetY->text();
    bool key = true;
    double offsetY = checkDouble(text, &key);
    ui->lineOffsetY->clear();

    if (!key)
        return;

    ui->listActions->addItem("Смещение по y: " + QString::number(offsetY));

    this->scaleX.append(this->scaleX[this->rotate.count() - 1]);
    this->scaleY.append(this->scaleY[this->rotate.count() - 1]);
    this->offsetX.append(this->offsetX[this->offsetX.count() - 1]);
    this->offsetY.append(this->offsetY[this->offsetY.count() - 1] + offsetY);
    this->rotate.append(this->rotate[this->rotate.count() - 1]);

    repaint();
}

void MainWindow::on_lineOffsetX_returnPressed()
{
    on_btnOffseX_clicked();
}

void MainWindow::on_lineOffsetY_returnPressed()
{
    on_btnOffsetY_clicked();
}

void MainWindow::on_btnScale_clicked()
{
    QString MxText = ui->lineScaleCenterX->text();
    QString MyText = ui->lineScaleCenterY->text();
    QString scaleXText = ui->lineScaleX->text();
    QString scaleYText = ui->lineScaleY->text();
    bool key = true;

    ui->lineScaleCenterX->clear();
    ui->lineScaleCenterY->clear();
    ui->lineScaleX->clear();
    ui->lineScaleY->clear();

    double Mx = checkDouble(MxText, &key);
    if (!key)
        return;

    double My = checkDouble(MyText, &key);
    if (!key)
        return;

    double scaleX = checkDouble(scaleXText, &key);
    if (!key)
        return;

    double scaleY = checkDouble(scaleYText, &key);
    if (!key)
        return;

    ui->listActions->addItem("Масшт.: ("
                             + QString::number(Mx) + ";"
                             + QString::number(My) + ")"
                             + " Kx = " + QString::number(scaleX)
                             + "; Ky = " + QString::number(scaleY));

    QPair<double, double> center;
    center = QPair<double, double>(this->offsetX[this->offsetX.count() - 1],
                                   this->offsetY[this->offsetY.count() - 1]);

    QPair<double, double> newCenter;
    newCenter = QPair<double, double>(scaleX * center.first + (1 - scaleX) * Mx,
                                      scaleY * center.second + (1 - scaleY) * My);

    this->scaleX.append(this->scaleX[this->rotate.count() - 1] * scaleX);
    this->scaleY.append(this->scaleY[this->rotate.count() - 1] * scaleY);
    this->offsetX.append(newCenter.first);
    this->offsetY.append(newCenter.second);
    this->rotate.append(this->rotate[this->rotate.count() - 1]);

    repaint();
}

void MainWindow::on_btnRotate_clicked()
{
    QString CxText = ui->lineRotateCenterX->text();
    QString CyText = ui->lineRotateCenterY->text();
    QString angleText = ui->lineRotate->text();
    bool key = true;

    ui->lineRotateCenterX->clear();
    ui->lineRotateCenterY->clear();
    ui->lineRotate->clear();

    double Cx = checkDouble(CxText, &key);
    if (!key)
        return;

    double Cy = checkDouble(CyText, &key);
    if (!key)
        return;

    double angle = checkDouble(angleText, &key);
    if (!key)
        return;

    ui->listActions->addItem("Вращ.: ("
                             + QString::number(Cx) + ";"
                             + QString::number(Cy) + ") "
                             + "Θ = " + QString::number(angle));

    QPair<double, double> center;
    center = QPair<double, double>(this->offsetX[this->offsetX.count() - 1],
                                   this->offsetY[this->offsetY.count() - 1]);

    QPair<double, double> newCenter;
    newCenter = QPair<double, double>(Cx + (center.first - Cx) * cos(angle / RADIANS) - (center.second - Cy) * sin(angle / RADIANS),
                                      Cy + (center.first - Cx) * sin(angle / RADIANS) + (center.second - Cy) * cos(angle / RADIANS));

    this->scaleX.append(this->scaleX[this->rotate.count() - 1]);
    this->scaleY.append(this->scaleY[this->rotate.count() - 1]);
    this->offsetX.append(newCenter.first);
    this->offsetY.append(newCenter.second);
    this->rotate.append(this->rotate[this->rotate.count() - 1] + angle);

    repaint();
}

void MainWindow::on_btnCancel_clicked()
{
    int index = ui->listActions->count() - 1;
    QListWidgetItem* item;

    if (index >= 0) {
        item =  ui->listActions->item(index);
        delete item;

        scaleX.removeLast();
        scaleY.removeLast();
        offsetX.removeLast();
        offsetY.removeLast();
        rotate.removeLast();

        repaint();
    }
}
