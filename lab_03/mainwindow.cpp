#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentLineColor = Qt::black;
    currentBackgroundColor = Qt::white;

    QString qss = QString("background-color: %1").arg(currentLineColor.name());
    ui->btnColor->setStyleSheet(qss);

    QString qssbg = QString("background-color: %1").arg(currentBackgroundColor.name());
    ui->btnBColor->setStyleSheet(qssbg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    int canvasWidht = width() - 210;
    int canvasHeight = height();

    p.fillRect(0, 0, canvasWidht, canvasHeight, currentBackgroundColor);

    p.setPen(QPen(Qt::gray, 1, Qt::SolidLine));
    p.drawLine(canvasWidht / 2, 0, canvasWidht / 2, canvasHeight);
    p.drawLine(0, canvasHeight / 2, canvasWidht, canvasHeight / 2);

    for (Line line : lines) {
        QList<Point> points = line.draw();
        if (line.isLibrary()) {
            QPoint po1 = points[0].getPoint();
            QPoint po2 = points[1].getPoint();
            p.setPen(QPen(points[0].getColor(), 1));

            p.drawLine(canvasWidht / 2 + po1.x(), canvasHeight / 2 - po1.y(),
                       canvasWidht / 2 + po2.x(), canvasHeight / 2 - po2.y());
        } else {
            for (Point point : points) {
                QPoint po = point.getPoint();
                p.setPen(QPen(point.getColor(), 1));

                p.drawPoint(canvasWidht / 2 + po.x(), canvasHeight / 2 - po.y());
            }
        }
    }
}

void MainWindow::on_btnLine_clicked()
{
    int alg = ui->cmbAlghoritms->currentIndex();

    bool key = false;
    double startX = ui->lineStartX->text().toDouble(&key);
    if (!key) {
        QMessageBox::critical(this, "Ошибка", "Некорректный ввод!");
        return;
    }

    double startY = ui->lineStartY->text().toDouble(&key);
    if (!key) {
        QMessageBox::critical(this, "Ошибка", "Некорректный ввод!");
        return;
    }

    double finishX = ui->lineFinishX->text().toDouble(&key);
    if (!key) {
        QMessageBox::critical(this, "Ошибка", "Некорректный ввод!");
        return;
    }

    double finishY = ui->lineFinishY->text().toDouble(&key);
    if (!key) {
        QMessageBox::critical(this, "Ошибка", "Некорректный ввод!");
        return;
    }

    lines.append(Line(startX, startY, finishX, finishY, currentLineColor, alg));

    repaint();
}

void MainWindow::on_btnBundle_clicked()
{
    int alg = ui->cmbAlgBundle->currentIndex();

    bool key = false;
    double step = ui->lineStepBundle->text().toDouble(&key);
    if (!key) {
        QMessageBox::critical(this, "Ошибка", "Некорректный ввод!");
        return;
    }

    double length = ui->lineLengthBundle->text().toDouble(&key);
    if (!key) {
        QMessageBox::critical(this, "Ошибка", "Некорректный ввод!");
        return;
    }

    for (double angle = 0; angle <= 360; angle += step) {
        lines.append(Line(0, 0, length * cos(angle * PI / 180), length * sin(angle * PI / 180), currentLineColor, alg));
    }

    repaint();
}

void MainWindow::on_btnClear_clicked()
{
    lines = QList<Line>();
    repaint();
}

void MainWindow::on_btnColor_clicked()
{
    QColorDialog* q = new QColorDialog();
    q->open();
    currentLineColor = q->getColor();
    QString qss = QString("background-color: %1").arg(currentLineColor.name());
    ui->btnColor->setStyleSheet(qss);
    delete q;
}

void MainWindow::on_btnBColor_clicked()
{
    QColorDialog* q = new QColorDialog();
    q->open();
    currentBackgroundColor = q->getColor();
    QString qss = QString("background-color: %1").arg(currentBackgroundColor.name());
    ui->btnBColor->setStyleSheet(qss);
    delete q;

    repaint();
}
