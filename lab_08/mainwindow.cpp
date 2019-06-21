#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    backgroundColor = Qt::white;
    figureColor = Qt::blue;
    clipColor = Qt::red;
    edgeColor = Qt::black;

    QString qss = QString("background-color: %1").arg(backgroundColor.name());
    ui->btnBackgroundColor->setStyleSheet(qss);

    qss = QString("background-color: %1").arg(figureColor.name());
    ui->btnFigureColor->setStyleSheet(qss);

    qss = QString("background-color: %1").arg(clipColor.name());
    ui->btnClipColor->setStyleSheet(qss);

    qss = QString("background-color: %1").arg(edgeColor.name());
    ui->btnEdgeColor->setStyleSheet(qss);

    firstForLine = QPoint(0, 0);
    firstForFigure = QPoint(0, 0);

    pressCmd = false;
    pressShift = false;
    pressAlt = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QList<Figure> MainWindow::getFigures()
{
    QList<Figure> figures;
    QList<QPoint> p;
    for (int i = 0; i < ui->listDots->count(); ++i) {
        QList<QString> numbers = ui->listDots->item(i)->text().split(" ");

        p.append(QPoint(numbers[0].toInt(), numbers[1].toInt()));

        if (numbers.count() == 3) {
            figures.append(Figure(p));
            p = QList<QPoint>();
        }
    }

    if (p.count())
        figures.append(Figure(p));

    return figures;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    int canvasWidth = width() - MARGINLEFT;
    int canvasHeight = height();
    p.fillRect(0, 0, canvasWidth, canvasHeight, backgroundColor);
    p.setClipRect(0, 0, canvasWidth, canvasHeight);

    QList<Figure> figures = getFigures();

    p.setPen(QPen(figureColor, 1));
    for (Figure figure : figures) {
        figure.draw(p);
    }

    p.setPen(QPen(edgeColor, 1));
    for (Line line : lines) {
        line.draw(p);
    }

    p.setPen(QPen(clipColor, 2));
    for (Figure figure : figures) {
        if (figure.isClose())
            figure.clipAll(p, lines);
    }
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        int x = e->x();
        int y = e->y();

        if (pressCmd) {
            if (firstForLine == QPoint(0, 0)) {
                firstForLine = QPoint(x, y);
            } else {
                if (pressShift) {
                    int xr = std::abs(firstForLine.x() - x);
                    int yr = std::abs(firstForLine.y() - y);

                    if (xr < yr) {
                        x = firstForLine.x();
                    } else {
                        y = firstForLine.y();
                    }
                }

                Line line(firstForLine, QPoint(x, y));
                lines.append(line);
                firstForLine = QPoint(0, 0);
                ui->listLines->addItem("(" + QString::number(line.getA().x()) +
                                       ";" + QString::number(line.getA().y()) +
                                       ") (" + QString::number(line.getB().x()) +
                                       ";" + QString::number(line.getB().y()) + ")");
            }
        } else if (pressAlt) {
            if (altState == 1) {
                ++altState;

                QList<Figure> figures = getFigures();
                double min = -1;
                for (Figure figure : figures) {
                    QPair< double, QPair<QPoint, QPoint> > res =
                            figure.getNear(QPoint(x, y));

                    if (std::fabs(min + 1) < 1e-3 || res.first < min) {
                        min = res.first;
                        altLine = res.second;
                    }
                }
            } else if (altState == 2) {
                ++altState;

                altStart = QPoint(x, y);
            } else if (altState == 3) {
                altState = 1;

                if (altLine.first.x() == altLine.second.x()) {
                    altFinish = QPoint(altStart.x(), y);
                } else if (altLine.first.y() == altLine.second.y()) {
                    altFinish = QPoint(x, altStart.y());
                } else {
                    double k = double(altLine.second.y() - altLine.first.y())
                                   / (altLine.second.x() - altLine.first.x());
                    double b = altStart.y() - k * altStart.x();

                    double k_per = -1.0 / k;
                    double b_per = y - k_per * x;

                    int new_x = int((b_per - b) / (k - k_per));
                    int new_y = int(k * new_x + b);

                    altFinish = QPoint(new_x, new_y);
                }

                Line line(altStart, altFinish);
                lines.append(line);
                firstForLine = QPoint(0, 0);
                ui->listLines->addItem("(" + QString::number(line.getA().x()) +
                                       ";" + QString::number(line.getA().y()) +
                                       ") (" + QString::number(line.getB().x()) +
                                       ";" + QString::number(line.getB().y()) + ")");
            }
        } else {
            if (firstForFigure.x() == x && firstForFigure.y() == y) {
                ui->listDots->addItem(QString::number(x) + " " +
                                      QString::number(y) + " end");
                firstForFigure = QPoint(0, 0);
            } else {
                if (pressShift && firstForFigure != QPoint(0, 0)) {
                    QString s = ui->listDots->item(ui->listDots->count() - 1)->text();
                    QPoint prev = QPoint(s.split(" ")[0].toInt(), s.split(" ")[1].toInt());

                    int xr = std::abs(prev.x() - x);
                    int yr = std::abs(prev.y() - y);

                    if (xr < yr) {
                        x = prev.x();
                    } else {
                        y = prev.y();
                    }
                }

                ui->listDots->addItem(QString::number(x) + " " +
                                      QString::number(y));
            }

            if (firstForFigure.x() == 0 && firstForFigure.y() == 0)
                firstForFigure = QPoint(x, y);
        }
    }

    repaint();
}

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_Control:
        pressCmd = false;
        break;
    case Qt::Key_Shift:
        pressShift = false;
        break;
    case Qt::Key_Alt:
        pressAlt = false;
        altState = 1;
        break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_Return:
        on_btnFinish_clicked();
        break;
    case Qt::Key_Control:
        pressCmd = true;
        break;
    case Qt::Key_Shift:
        pressShift = true;
        break;
    case Qt::Key_Alt:
        pressAlt = true;
        altState = 1;
        break;
    }
}

void MainWindow::on_btnBackgroundColor_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    backgroundColor = color->getColor();
    QString qss = QString("background-color: %1").arg(backgroundColor.name());
    ui->btnBackgroundColor->setStyleSheet(qss);
    delete color;
    repaint();
}

void MainWindow::on_btnFigureColor_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    figureColor = color->getColor();
    QString qss = QString("background-color: %1").arg(figureColor.name());
    ui->btnFigureColor->setStyleSheet(qss);
    delete color;
    repaint();
}

void MainWindow::on_btnEdgeColor_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    edgeColor = color->getColor();
    QString qss = QString("background-color: %1").arg(edgeColor.name());
    ui->btnEdgeColor->setStyleSheet(qss);
    delete color;
    repaint();
}

void MainWindow::on_btnClipColor_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    clipColor = color->getColor();
    QString qss = QString("background-color: %1").arg(clipColor.name());
    ui->btnClipColor->setStyleSheet(qss);
    delete color;
    repaint();
}

void MainWindow::on_btnFinish_clicked()
{
    if (firstForFigure.x() != 0 || firstForFigure.y() != 0) {
        ui->listDots->addItem(QString::number(firstForFigure.x()) + " " +
                              QString::number(firstForFigure.y()) + " end");
        firstForFigure = QPoint(0, 0);
    }

    repaint();
}

void MainWindow::on_btnClear_clicked()
{
    ui->listDots->clear();
    ui->listLines->clear();
    lines = QList<Line>();
    repaint();
}

void MainWindow::on_btnAdd_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Ввод точки"),
                                         tr("Введите координаты через пробел (+ a и b для эллипса)"),
                                         QLineEdit::Normal,
                                         "0 0", &ok);

    QList<QString> numbers = text.split(" ");
    for (int i = 0; i < numbers.count(); ++i) {
        if (numbers[i] == "")
            numbers.removeAt(i);
    }

    if (numbers.count() == 4 && firstForFigure == QPoint(0, 0)) {
        bool xOk, yOk, aOk, bOk;
        int x = numbers[0].toInt(&xOk);
        int y = numbers[1].toInt(&yOk);
        int a = numbers[2].toInt(&aOk);
        int b = numbers[3].toInt(&bOk);

        if (!xOk || !yOk || !aOk || !bOk) {
            return;
        }

        ui->listDots->addItem(QString::number(x) + " " +
                              QString::number(y) + " " +
                              QString::number(a) + " " +
                              QString::number(b) + " ellipse");
    }

    if (numbers.count() == 2) {
        bool xOk, yOk;
        int x = numbers[0].toInt(&xOk);
        int y = numbers[1].toInt(&yOk);

        if (!xOk || !yOk) {
            return;
        }

        if (firstForFigure.x() == x && firstForFigure.y() == y) {
            ui->listDots->addItem(QString::number(x) + " " +
                                  QString::number(y) + " end");
            firstForFigure = QPoint(0, 0);
        } else {
            ui->listDots->addItem(QString::number(x) + " " +
                                  QString::number(y));
        }

        if (firstForFigure.x() == 0 && firstForFigure.y() == 0)
            firstForFigure = QPoint(x, y);
    }

    repaint();
}

void MainWindow::on_btnAddLine_clicked()
{
    bool key1, key2, key3, key4;
    int xStart = ui->lineStartX->text().toInt(&key1);
    int yStart = ui->lineStartY->text().toInt(&key2);
    int xFinish = ui->lineFinishX->text().toInt(&key3);
    int yFinish = ui->lineFinishY->text().toInt(&key4);

    if (!(key1 && key2 && key3 && key4)) {
        QMessageBox::critical(this, "Ошибка", "Некорректные данные");
        return;
    }

    Line line(QPoint(xStart, yStart), QPoint(xFinish, yFinish));
    lines.append(line);
    ui->listLines->addItem("(" + QString::number(line.getA().x()) +
                           ";" + QString::number(line.getA().y()) +
                           ") (" + QString::number(line.getB().x()) +
                           ";" + QString::number(line.getB().y()) + ")");
    repaint();
}
