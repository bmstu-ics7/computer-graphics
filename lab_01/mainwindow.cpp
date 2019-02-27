/*
 * На плоскости дано множество точек. Найти такой треугольник, у которого угол между биссектрисой и высотой,
 * выходящих из одной вершины, максимален (для каждого треугольника рассматривать все три вершины и выбирать максимальное
 * значение угла и номер вершины для вывода в графическом режиме)
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this) ;
    repaint();
}

MainWindow::~MainWindow() {
    delete ui;
}

double line(QPointF a, QPointF b) {
    return sqrt(pow(a.x() - b.x(), 2) + pow(a.y() - b.y(), 2));
}

double max(double a, double b, double c) {
    return a > b ? a > c ? a : c : b > c ? b : c;
}

double min(double a, double b) {
    return a < b ? a : b;
}

/*********************************************************************
 * Находит угол между высотой и биссектрисой, выходящих из вершины B *
 *********************************************************************/

double searchAngle(QPointF A, QPointF B, QPointF C) {
    double a = line(A, B); // AB
    double b = line(B, C); // BC
    double c = line(C, A); // AC

    if (a + b <= c + 10e-6 ||
        a + c <= b + 10e-6 ||
        c + b <= a + 10e-6)
        return 0;

    double alpha = acos((a * a + c * c - b * b) / (2 * a * c)) * RADIAN;    // Угол при вершине A (теорема косинусов)
    double betta = acos((a * a + b * b - c * c) / (2 * a * b)) * RADIAN;    // Угол при вершине B (теорема косинусов)
    double gamma = 180 - alpha - betta;

    double angle = 90 - min(180 - betta / 2 - alpha,                        // Первый случай, когда биссектриса между H и A
                            180 - betta / 2 - gamma);                       // Второй случай, когда биссектриса между H и C
                                                                            // BH - высота
    return angle;
}

/*********************************************************************
 *                   Находит координату биссектрисы                  *
 *********************************************************************/

QPointF findBisectrisa(QList<QPointF> maxTriangle, int indexAngle) {
    QPointF A = maxTriangle[indexAngle];
    QPointF B;
    QPointF C;

    if (indexAngle == 0) {
        B = maxTriangle[1];
        C = maxTriangle[2];
    } else if (indexAngle == 1) {
        B = maxTriangle[0];
        C = maxTriangle[2];
    } else {
        B = maxTriangle[0];
        C = maxTriangle[1];
    }

    double c = line(A, B);
    double b = line(C, A);

    double x = (c * C.x() + b * B.x()) / (c + b);
    double y = (c * C.y() + b * B.y()) / (c + b);

    return QPointF(x, -y);
}

/*********************************************************************
 *                     Находит координату высоты                     *
 *********************************************************************/

QPointF findHeight(QList<QPointF> maxTriangle, int indexAngle) {
    QPointF A = maxTriangle[indexAngle];
    QPointF B;
    QPointF C;

    if (indexAngle == 0) {
        B = maxTriangle[1];
        C = maxTriangle[2];
    } else if (indexAngle == 1) {
        B = maxTriangle[0];
        C = maxTriangle[2];
    } else {
        B = maxTriangle[0];
        C = maxTriangle[1];
    }

    if (fabs(B.x() - C.x()) <= 10e-6)
        return QPointF(B.x(), A.y());

    if (fabs(B.y() - C.y()) <= 10e-6)
        return QPointF(A.x(), B.y());

    double k = double(B.y() - C.y()) / (B.x() - C.x());
    double b = B.y() - k * B.x();

    double k_new = -1 / k;
    double b_new = A.y() - k_new * A.x();

    double heightX = (b_new - b) / (k - k_new);
    double heightY = k * heightX + b;

    return QPointF(heightX, heightY);
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter p(this);

    int canvasWidth = width() - 200;
    int canvasHeight = height() - 80;
    p.fillRect(0, 0, canvasWidth, canvasHeight, Qt::white);

    double maxX = 0;
    double maxY = 0;

    QList<QPointF> points;
    for (int row = 0; row < ui->listDots->count(); row++) {
        QListWidgetItem *item = ui->listDots->item(row);
        QString str = item->text();
        double x = str.split(" ")[0].toDouble();
        double y = str.split(" ")[1].toDouble();
        points.append(QPointF(x, y));

        if (maxX < fabs(x)) maxX = fabs(x);
        if (maxY < fabs(y)) maxY = fabs(y);
    }

    double step = 20;
    if (maxX <= 0 && maxY <= 0) {
        step = 10;
    } else {
        if (maxX <= 0) {
            step = double(canvasHeight) / ++maxY / 2;
        } else if (maxY <= 0) {
            step = double(canvasWidth) / ++maxX / 2;
        } else {
            if (double(canvasWidth / maxX) <= int(canvasHeight / maxY)) {
                step = double(canvasWidth) / ++maxX / 2;
            } else {
                step = double(canvasHeight) / ++maxY / 2;
            }
        }
    }

    if (step <= 1) {
        if (int(canvasWidth / maxX) <= int(canvasHeight / maxY)) {
            maxX += maxX / 5;
            step = double(canvasWidth) / maxX / 2;
        } else {
            maxY += maxY / 5;
            step = double(canvasHeight) / maxY / 2;
        }
    }

    if (step > 4) {
        p.setPen(QPen(Qt::gray, 1, Qt::SolidLine, Qt::FlatCap));
        for (double i = canvasWidth / 2; i <= canvasWidth; i += step)
            p.drawLine(int(i), 0, int(i), canvasHeight);
        for (double i = canvasWidth / 2; i >= 0; i -= step)
            p.drawLine(int(i), 0, int(i), canvasHeight);

        for (double i = canvasHeight / 2; i <= canvasHeight; i += step)
            p.drawLine(0, int(i), canvasWidth, int(i));
        for (double i = canvasHeight / 2; i >= 0; i -= step)
            p.drawLine(0, int(i), canvasWidth, int(i));
    }

    p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
    p.drawLine(0, canvasHeight / 2, canvasWidth, canvasHeight / 2);
    p.drawLine(canvasWidth / 2, 0, canvasWidth / 2, canvasHeight);

    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    p.setBrush(QBrush(Qt::black));
    for (QPointF point : points) {
        p.drawEllipse(QPointF(floor(canvasWidth / 2 + point.x() * step), floor(canvasHeight / 2 + point.y() * -step)), 3, 3);
    }

    double angleMax = 0;
    QList<QPointF> maxTriangle;
    int indexAngle = 0;
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            for (int k = j + 1; k < points.size(); k++) {
                QPointF A = points[i];
                QPointF B = points[j];
                QPointF C = points[k];

                double angleB = searchAngle(A, B, C);
                double angleA = searchAngle(C, A, B);
                double angleC = searchAngle(B, C, A);

                double angle = max(angleA, angleB, angleC);

                if (angleMax < angle && angle < 90) {
                    angleMax = angle;
                    maxTriangle = QList<QPointF>();
                    maxTriangle.append(A);
                    maxTriangle.append(B);
                    maxTriangle.append(C);

                    if (abs(angle - angleA) <= 10e-6) indexAngle = 0;
                    if (abs(angle - angleB) <= 10e-6) indexAngle = 1;
                    if (abs(angle - angleC) <= 10e-6) indexAngle = 2;
                }
            }
        }
    }

    if (angleMax > 0) {
        /**************************************
         *       Рисование треугольника       *
         **************************************/

        p.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap));

        p.drawLine(int(canvasWidth  / 2 + maxTriangle[0].x() *  step),
                   int(canvasHeight / 2 + maxTriangle[0].y() * -step),
                   int(canvasWidth  / 2 + maxTriangle[1].x() *  step),
                   int(canvasHeight / 2 + maxTriangle[1].y() * -step));

        p.drawLine(int(canvasWidth  / 2 + maxTriangle[1].x() *  step),
                   int(canvasHeight / 2 + maxTriangle[1].y() * -step),
                   int(canvasWidth  / 2 + maxTriangle[2].x() *  step),
                   int(canvasHeight / 2 + maxTriangle[2].y() * -step));

        p.drawLine(int(canvasWidth  / 2 + maxTriangle[2].x() *  step),
                   int(canvasHeight / 2 + maxTriangle[2].y() * -step),
                   int(canvasWidth  / 2 + maxTriangle[0].x() *  step),
                   int(canvasHeight / 2 + maxTriangle[0].y() * -step));

        /**************************************
         *       Рисование биссектрисы        *
         **************************************/

        QPointF bisectrisa = findBisectrisa(maxTriangle, indexAngle);

        p.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));

        p.drawLine(int(canvasWidth  / 2 + maxTriangle[indexAngle].x() *  step),
                   int(canvasHeight / 2 + maxTriangle[indexAngle].y() * -step),
                   int(canvasWidth  / 2 + bisectrisa.x() *  step),
                   int(canvasHeight / 2 + bisectrisa.y() *  step));

        /**************************************
         *          Рисование высоты          *
         **************************************/

        QPointF height = findHeight(maxTriangle, indexAngle);

        p.setPen(QPen(Qt::darkGreen, 1, Qt::SolidLine, Qt::FlatCap));

        p.drawLine(int(canvasWidth  / 2 + maxTriangle[indexAngle].x() *  step),
                   int(canvasHeight / 2 + maxTriangle[indexAngle].y() * -step),
                   int(canvasWidth  / 2 + height.x() *  step),
                   int(canvasHeight / 2 + height.y() * -step));

        /**************************************
         *          Рисование текста          *
         **************************************/

        p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));

        p.drawText(int(canvasWidth  / 2 + maxTriangle[0].x() *  step + 5),
                   int(canvasHeight / 2 + maxTriangle[0].y() * -step + 5),
                   QString("(" + QString::number(maxTriangle[0].x()) + ";"
                               + QString::number(maxTriangle[0].y()) + ")"));

        p.drawText(int(canvasWidth  / 2 + maxTriangle[1].x() *  step + 5),
                   int(canvasHeight / 2 + maxTriangle[1].y() * -step + 5),
                   QString("(" + QString::number(maxTriangle[1].x()) + ";"
                               + QString::number(maxTriangle[1].y()) + ")"));

        p.drawText(int(canvasWidth  / 2 + maxTriangle[2].x() *  step + 5),
                   int(canvasHeight / 2 + maxTriangle[2].y() * -step + 5),
                   QString("(" + QString::number(maxTriangle[2].x()) + ";"
                               + QString::number(maxTriangle[2].y()) + ")"));
    }
}

/*********************************************************************
 *                      Проверка входных данных                      *
 *********************************************************************/

bool MainWindow::checkTwoInt(QString text) {
    QList<QString> numbers = text.split(" ");
    for (int i = 0; i < numbers.count(); i++) {
        if (numbers[i] == "") {
            numbers.removeAt(i);
            i--;
        }
    }

    if (numbers.count() != 2) {
        QMessageBox::critical(this, "Ошибка", "Введено не 2 числа");
        return false;
    }

    double x = 0;
    double y = 0;

    bool keyX = false;
    bool keyY = false;

    x = numbers[0].toDouble(&keyX);
    y = numbers[1].toDouble(&keyY);

    if (!(keyY && keyX)) {
        QMessageBox::critical(this, "Ошибка", "Введены не числа");
        return false;
    }

    return true;
}

void MainWindow::on_btnAdd_clicked() {
    QString text = ui->lineEdit->text();
    ui->lineEdit->clear();

    if (checkTwoInt(text)) {
        ui->listDots->addItem(text);
        repaint();
    }
}

void MainWindow::on_btnDelAll_clicked()
{
    ui->listDots->clear();
    repaint();
}

void MainWindow::on_btnDel_clicked()
{
    QListWidgetItem *current = ui->listDots->currentItem();
    delete current;

    repaint();
}

void MainWindow::on_btnEdit_clicked()
{
    QString text = ui->lineEdit->text();
    ui->lineEdit->clear();

    if (checkTwoInt(text)) {
        if (ui->listDots->currentItem())
            ui->listDots->currentItem()->setText(text);

        repaint();
    }
}
