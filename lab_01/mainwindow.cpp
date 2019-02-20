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

double line(QPoint a, QPoint b) {
    return sqrt(pow(a.x() - b.x(), 2) + pow(a.y() - b.y(), 2));
}

double max(double a, double b, double c) {
    return a > b ? a > c ? a : c : b > c ? b : c;
}

double min(double a, double b) {
    return a < b ? a : b;
}


/*
 * Находит угол между высотой и биссектрисой, выходящих из вершины B
 */
double searchAngle(QPoint A, QPoint B, QPoint C) {
    double a = line(A, B); // AB
    double b = line(B, C); // BC
    double c = line(C, A); // AC

    double alpha = acos((a * a + c * c - b * b) / (2 * a * c)); // Угол при вершине A (теорема косинусов)
    double betta = acos((a * a + b * b - c * c) / (2 * a * b)); // Угол при вершине B (теорема косинусов)
    double gamma = 180 - alpha - betta;

    double angle = 90 - min(180 - betta / 2 - alpha,            // Первый случай, когда биссектриса между H и A
                            180 - betta / 2 - gamma);           // Второй случай, когда биссектриса между H и C
                                                                // BH - высота
    return angle;
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter p(this);

    int canvasWidth = width() - 200;
    int canvasHeight = height();
    p.fillRect(0, 0, canvasWidth, canvasHeight, Qt::white);

    int maxX = 0;
    int maxY = 0;

    QList<QPoint> points;
    for (int row = 0; row < ui->listDots->count(); row++) {
        QListWidgetItem *item = ui->listDots->item(row);
        QString str = item->text();
        int x = 0;
        int y = 0;

        bool modeX = true;
        int xMinus = 1;
        int yMinus = 1;

        for (QChar c : str) {
            if (modeX) {
                if (c == ' ') {
                    modeX = false;
                    continue;
                }
                if (c == '-') {
                    xMinus = -1;
                    continue;
                }

                x = x * 10 + c.unicode() - 48;
            } else {
                if (c == '-') {
                    yMinus = -1;
                    continue;
                }

                y = y * 10 + c.unicode() - 48;
            }
        }

        if (maxX < x) maxX = x;
        if (maxY < y) maxY = y;

        points.append(QPoint(x * xMinus, y * yMinus));
    }

    double step = 20;
    if (maxX <= 0 && maxY <= 0) {
        step = 10;
    } else {
        if (maxX <= 0) {
            step = int(canvasHeight / ++maxY / 2);
        } else if (maxY <= 0) {
            step = int(canvasWidth / ++maxX / 2);
        } else {
            if (int(canvasWidth / maxX) <= int(canvasHeight / maxY)) {
                step = int(canvasWidth / ++maxX / 2);
            } else {
                step = int(canvasHeight / ++maxY / 2);
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
        for (int i = canvasWidth / 2; i <= canvasWidth; i += step)
            p.drawLine(i, 0, i, canvasHeight);
        for (int i = canvasWidth / 2; i >= 0; i -= step)
            p.drawLine(i, 0, i, canvasHeight);

        for (int i = canvasHeight / 2; i <= canvasHeight; i += step)
            p.drawLine(0, i, canvasWidth, i);
        for (int i = canvasHeight / 2; i >= 0; i -= step)
            p.drawLine(0, i, canvasWidth, i);
    }

    p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
    p.drawLine(0, canvasHeight / 2, canvasWidth, canvasHeight / 2);
    p.drawLine(canvasWidth / 2, 0, canvasWidth / 2, canvasHeight);

    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    p.setBrush(QBrush(Qt::black));
    for (QPoint point : points) {
        p.drawEllipse(QPoint(int(canvasWidth / 2 + point.x() * step), int(canvasHeight / 2 + point.y() * -step)), 3, 3);
    }

    double angleMax = 0;
    QList<QPoint> maxTriangle;
    for (int i = 0; i < points.size(); i++) {
        for (int j = i; j < points.size(); j++) {
            for (int k = j; k < points.size(); k++) {
                QPoint A = points[i];
                QPoint B = points[j];
                QPoint C = points[k];

                double angle = max(searchAngle(A, B, C),
                                   searchAngle(B, A, C),
                                   searchAngle(A, C, B));

                if (angleMax < angle && angle < 90) {
                    angleMax = angle;
                    maxTriangle = QList<QPoint>();
                    maxTriangle.append(A);
                    maxTriangle.append(B);
                    maxTriangle.append(C);
                }
            }
        }
    }

    if (angleMax > 0) {
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

void MainWindow::on_btnAdd_clicked() {
    QString text = ui->lineEdit->text();
    ui->lineEdit->clear();

    int count = 0;
    bool wasNum = false;
    QChar prev = '\0';
    for (QChar a : text) {
        if (!a.isDigit()) {
            if (a == '-' && (prev == ' ' || prev == '\0')) {
                prev = a;
                continue;
            }

            if (a == ' ') {
                if (!wasNum) {
                    QMessageBox::critical(this, "Ошибка!", "Пробел до чисел!");
                    return;
                }

                count++;
                if (count > 1) {
                    QMessageBox::critical(this, "Ошибка!", "Лишние пробелы!");
                    return;
                } else {
                    prev = a;
                    continue;
                }
            }

            QMessageBox::critical(this, "Ошибка!", "Введены не числа");
            return;
        } else {
            wasNum = true;
        }

        prev = a;
    }

    if (count == 0) {
        QMessageBox::critical(this, "Ошибка!", "Введено одно число!");
        return;
    }

    ui->listDots->addItem(text);

    repaint();
}
