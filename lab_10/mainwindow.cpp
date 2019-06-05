#include "mainwindow.h"
#include "ui_mainwindow.h"

double ellipseParaboloid(double x, double z)
{
    return x * x / 20 + z * z / 20;
}

double sinus(double x, double z)
{
    Q_UNUSED(z);
    return 100 * sin(0.05 * x);
}

double trigonmetrial(double x, double z)
{
    return 100 * cos(0.01 * x) * cos(0.01 * x) - 100 * sin(0.01 * z) * sin(0.01 * z);
}

double graphics(double x, double z)
{
    return 50 * acos(sin(0.05 * x) * cos(0.05 * z));
}

double kochka(double x, double z)
{
    return 50 * (-sin(0.0001 * x * x + 0.0001 * z * z) + 1);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    backgroundColor = "#222";
    foregroundColor = Qt::white;

    QString qss = QString("background-color: %1;").arg(backgroundColor.name());
    qss += " border-style: solid; border-width: 1px; border-color: gray;";
    ui->btnBackgroundColor->setStyleSheet(qss);

    qss = QString("background-color: %1;").arg(foregroundColor.name());
    qss += " border-style: solid; border-width: 1px; border-color: gray;";
    ui->btnForegroundColor->setStyleSheet(qss);

    angleX = 0;
    angleY = 0;
    angleZ = 0;

    function = ellipseParaboloid;

    xStart = ui->spinXStart->value();
    zStart = ui->spinZStart->value();
    xEnd = ui->spinXFinish->value();
    zEnd = ui->spinZFinish->value();;
    xDelta = ui->spinXDelta->value();
    zDelta = ui->spinZDelta->value();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawLine(QPainter& painter, const int width, const int height,
                          Point start, Point end,
                          QList<double>& top, QList<double>& bottom)
{
    int x = start.getIntX();
    int y = start.getIntY();

    int dx = end.getIntX() - start.getIntX();
    int dy = end.getIntY() - start.getIntY();
    int sx = dx > 0 ? 1 : dx < 0 ? -1 : 0;
    int sy = dy > 0 ? 1 : dx < 0 ? -1 : 0;
    dx = std::abs(dx);
    dy = std::abs(dy);

    x += width / 2;

    if (dx == 0 && dy == 0 && x >= 0 && x < width) {
        if (y >= top[x]) {
            top[x] = y;
            painter.drawPoint(x, height / 2 - y);
        }

        if (y <= bottom[x]) {
            bottom[x] = y;
            painter.drawPoint(x, height / 2 - y);
        }

        return;
    }

    if (x < 0 || x >= width) return;

    bool change = false;
    if (dy > dx) {
        std::swap(dx, dy);
        change = true;
    }

    int yMaxCur = int(top[x]);
    int yMinCur = int(bottom[x]);
    int e = 2 * dy - dx;

    for (int i = 1; i <= dx; ++i) {
        if (x >= 0 && x < width) {
            if (y >= top[x]) {
                if (y >= yMaxCur) {
                    yMaxCur = y;
                }
                painter.drawPoint(x, height / 2 - y);
            }

            if (y <= bottom[x]) {
                if (y <= yMinCur) {
                    yMinCur = y;
                }
                painter.drawPoint(x, height / 2 - y);
            }
        }

        if (x < 0 || x >= width) return;

        if (e > 0) {
            if (change) {
                if (x < 0 || x >= width) return;
                top[x] = yMaxCur;
                bottom[x] = yMinCur;

                x += sx;

                if (x < 0 || x >= width) return;
                yMaxCur = int(top[x]);
                yMinCur = int(bottom[x]);
            } else {
                y += sy;
            }

            e -= 2 * dx;
        }

        if (!change) {
            if (x < 0 || x >= width) return;
            top[x] = yMaxCur;
            bottom[x] = yMinCur;

            x += sx;

            if (x < 0 || x >= width) return;
            yMaxCur = int(top[x]);
            yMinCur = int(bottom[x]);
        } else {
            y += sy;
        }

        e += 2 * dy;
    }
}

void MainWindow::drawLines(QPainter& painter,
                           const int canvasWidth, const int canvasHeight)
{
    double xRight = -1;
    double yRight = -1;
    double xLeft = -1;
    double yLeft = -1;

    QList< double > top;
    QList< double > bottom;

    for (int i = 0; i < canvasWidth; ++i) {
        top.append(-canvasHeight);
        bottom.append(canvasHeight);
    }

    for (double z = zEnd; z >= zStart; z -= zDelta) {
        Point prev(xStart, function(xStart, z), z);
        prev.rotate(angleX, angleY, angleZ);

        double zBuf = prev.getZ();
        double xPrev = prev.getX();
        double yPrev = prev.getY();

        if (std::abs(xLeft + 1) >= 1e-3) {
            drawLine(painter, canvasWidth, canvasHeight, prev, Point(xLeft, yLeft, 0), top, bottom);
        }

        xLeft = xPrev;
        yLeft = yPrev;

        for (double x = xStart; x <= xEnd; x += xDelta) {
            double y = function(x, z);
            Point current(x, y, z);
            current.rotate(angleX, angleY, angleZ);

            drawLine(painter, canvasWidth, canvasHeight, Point(xPrev, yPrev, 0), current, top, bottom);

            xPrev = current.getX();
            yPrev = current.getY();
        }

        if (std::abs(z - zEnd) >= 1e-3) {
            xRight = xEnd;
            yRight = function(xEnd, z - zDelta);
            Point right(xRight, yRight, z - zDelta);
            right.rotate(angleX, angleY, angleZ);
            xRight = right.getX();
            yRight = right.getY();
            zBuf = right.getZ();
            drawLine(painter, canvasWidth, canvasHeight, Point(xPrev, yPrev, 0), right, top, bottom);
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    int canvasWidth = width() - MARGINRIGHT;
    int canvasHeight = height();

    QPainter painter(this);
    painter.fillRect(0, 0, canvasWidth, canvasHeight, backgroundColor);
    painter.setClipRect(0, 0, canvasWidth, canvasHeight);
    painter.setPen(QPen(foregroundColor, 1));
    drawLines(painter, canvasWidth, canvasHeight);
}

void MainWindow::on_btnBackgroundColor_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    backgroundColor = color->getColor();
    QString qss = QString("background-color: %1; %2").arg(backgroundColor.name(),
                  " border-style: solid; border-width: 1px; border-color: gray;");
    ui->btnBackgroundColor->setStyleSheet(qss);
    delete color;
    repaint();
}

void MainWindow::on_btnForegroundColor_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    foregroundColor = color->getColor();
    QString qss = QString("background-color: %1; %2").arg(foregroundColor.name(),
                  " border-style: solid; border-width: 1px; border-color: gray;");
    ui->btnForegroundColor->setStyleSheet(qss);
    delete color;
    repaint();
}

void MainWindow::on_sliderAngleX_sliderMoved(int position)
{
    ui->lblAngleX->setNum(position);
    angleX = position;

    repaint();
}

void MainWindow::on_sliderAngleY_sliderMoved(int position)
{
    ui->lblAngleY->setNum(position);
    angleY = position;

    repaint();
}

void MainWindow::on_sliderAngleZ_sliderMoved(int position)
{
    ui->lblAngleZ->setNum(position);
    angleZ = position;

    repaint();
}

void MainWindow::on_cmbChangeGraph_activated(int index)
{
    switch (index) {
    case 0:
        function = ellipseParaboloid;
        break;
    case 1:
        function = sinus;
        break;
    case 2:
        function = trigonmetrial;
        break;
    case 3:
        function = graphics;
        break;
    case 4:
        function = kochka;
        break;
    }

    repaint();
}

void MainWindow::on_spinXStart_valueChanged(double arg1)
{
    xStart = arg1;
    repaint();
}

void MainWindow::on_spinXFinish_valueChanged(double arg1)
{
    xEnd = arg1;
    repaint();
}

void MainWindow::on_spinXDelta_valueChanged(double arg1)
{
    xDelta = arg1;
    repaint();
}

void MainWindow::on_spinZStart_valueChanged(double arg1)
{
    zStart = arg1;
    repaint();
}

void MainWindow::on_spinZFinish_valueChanged(double arg1)
{
    zEnd = arg1;
    repaint();
}

void MainWindow::on_spinZDelta_valueChanged(double arg1)
{
    zDelta = arg1;
    repaint();
}

void MainWindow::on_sliderAngleX_valueChanged(int value)
{
    angleX = value;
    repaint();
}

void MainWindow::on_sliderAngleY_valueChanged(int value)
{
    angleY = value;
    repaint();
}

void MainWindow::on_sliderAngleZ_valueChanged(int value)
{
    angleZ = value;
    repaint();
}
