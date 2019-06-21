#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    backgroundColor = Qt::white;
    clipFigureColor = Qt::blue;
    figureColor = Qt::black;
    clipColor = Qt::red;

    QString qss = QString("background-color: %1").arg(backgroundColor.name());
    ui->btnBackgroundColor->setStyleSheet(qss);

    qss = QString("background-color: %1").arg(clipFigureColor.name());
    ui->btnClipFigureColor->setStyleSheet(qss);

    qss = QString("background-color: %1").arg(figureColor.name());
    ui->btnFigureColor->setStyleSheet(qss);

    qss = QString("background-color: %1").arg(clipColor.name());
    ui->btnClipColor->setStyleSheet(qss);

    pressCmd = false;
    pressShift = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QList<Figure> MainWindow::getFigures()
{
    QList<Figure> figures;
    QList<QPoint> p;
    for (int i = 0; i < ui->listDotsFigure->count(); ++i) {
        QList<QString> numbers = ui->listDotsFigure->item(i)->text().split(" ");

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

QList<Figure> MainWindow::getClip()
{
    QList<Figure> figures;
    QList<QPoint> p;
    for (int i = 0; i < ui->listDotsClip->count(); ++i) {
        QList<QString> numbers = ui->listDotsClip->item(i)->text().split(" ");

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
    int canvasWidth = width() - MARGINRIGHT;
    int canvasHeight = height();
    p.setClipRect(0, 0, canvasWidth, canvasHeight);
    p.fillRect(0, 0, canvasWidth, canvasHeight, backgroundColor);

    QList<Figure> clip = getClip();
    QList<Figure> figures = getFigures();

    p.setPen(QPen(clipFigureColor, 1));
    for (Figure figure : clip) {
        figure.draw(p);
    }

    p.setPen(QPen(figureColor, 1));
    for (Figure figure : figures) {
        figure.draw(p);
    }

    p.setPen(QPen(clipColor, 2));
    for (Figure figure : clip) {
        if (figure.isClose())
            figure.clipAll(p, figures);
    }
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        int x = e->x();
        int y = e->y();

        if (pressAlt && !pressCmd) {
            QPoint click(x, y);
            QList<Figure> clips = getClip();
            double minDist = -1;
            QPoint near = click;

            for (Figure figure : clips) {
                QPair<double, QPoint> res = figure.getNearDot(click);
                if (minDist < 0 || res.first < minDist) {
                    minDist = res.first;
                    near = res.second;
                }
            }

            if (minDist <= 10) {
                x = near.x();
                y = near.y();
            } else {
                QList<Figure> figures = getClip();
                QPair<QPoint, QPoint> line;
                double min = -1;
                for (Figure figure : figures) {
                    QPair< double, QPair<QPoint, QPoint> > res =
                            figure.getNear(QPoint(x, y));

                    if (std::fabs(min + 1) < 1e-3 || res.first < min) {
                        min = res.first;
                        line = res.second;
                    }
                }

                double k = double(line.second.y() - line.first.y())
                               / (line.second.x() - line.first.x());
                double b = line.second.y() - k * line.second.x();

                double k_per = -1.0 / k;
                double b_per = y - k_per * x;

                x = int((b_per - b) / (k - k_per));
                y = int(k * x + b);
            }
        }

        if (pressCmd) {
            if (firstForClip.x() == x && firstForClip.y() == y) {
                ui->listDotsClip->addItem(QString::number(x) + " " +
                                      QString::number(y) + " end");
                firstForClip = QPoint(0, 0);
            } else {
                if (pressShift && firstForClip != QPoint(0, 0)) {
                    QString s = ui->listDotsClip->item(ui->listDotsClip->count() - 1)->text();
                    QPoint prev = QPoint(s.split(" ")[0].toInt(), s.split(" ")[1].toInt());

                    int xr = std::abs(prev.x() - x);
                    int yr = std::abs(prev.y() - y);

                    if (xr < yr) {
                        x = prev.x();
                    } else {
                        y = prev.y();
                    }
                }

                ui->listDotsClip->addItem(QString::number(x) + " " +
                                      QString::number(y));
            }

            if (firstForClip.x() == 0 && firstForClip.y() == 0)
                firstForClip = QPoint(x, y);
        } else {
            if (firstForFigure.x() == x && firstForFigure.y() == y) {
                ui->listDotsFigure->addItem(QString::number(x) + " " +
                                      QString::number(y) + " end");
                firstForFigure = QPoint(0, 0);
            } else {
                if (pressShift && firstForFigure != QPoint(0, 0)) {
                    QString s = ui->listDotsFigure->item(ui->listDotsFigure->count() - 1)->text();
                    QPoint prev = QPoint(s.split(" ")[0].toInt(), s.split(" ")[1].toInt());

                    int xr = std::abs(prev.x() - x);
                    int yr = std::abs(prev.y() - y);

                    if (xr < yr) {
                        x = prev.x();
                    } else {
                        y = prev.y();
                    }
                }

                ui->listDotsFigure->addItem(QString::number(x) + " " +
                                      QString::number(y));
            }

            if (firstForFigure.x() == 0 && firstForFigure.y() == 0)
                firstForFigure = QPoint(x, y);
        }
    }

    repaint();
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_Return:
        if (pressCmd) on_btnFinishClip_clicked();
        else on_btnFinishFigure_clicked();
        break;
    case Qt::Key_Control:
        pressCmd = true;
        break;
    case Qt::Key_Shift:
        pressShift = true;
        break;
    case Qt::Key_Alt:
        pressAlt = true;
        break;
    }
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

void MainWindow::on_btnClipFigureColor_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    clipFigureColor = color->getColor();
    QString qss = QString("background-color: %1").arg(clipFigureColor.name());
    ui->btnClipFigureColor->setStyleSheet(qss);
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

void MainWindow::on_btnFinishClip_clicked()
{
    if (firstForClip.x() != 0 || firstForClip.y() != 0) {
        ui->listDotsClip->addItem(QString::number(firstForClip.x()) + " " +
                              QString::number(firstForClip.y()) + " end");
        firstForClip = QPoint(0, 0);
    }

    repaint();
}

void MainWindow::on_btnAddClip_clicked()
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

        ui->listDotsClip->addItem(QString::number(x) + " " +
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
            ui->listDotsClip->addItem(QString::number(x) + " " +
                                  QString::number(y) + " end");
            firstForFigure = QPoint(0, 0);
        } else {
            ui->listDotsClip->addItem(QString::number(x) + " " +
                                  QString::number(y));
        }

        if (firstForFigure.x() == 0 && firstForFigure.y() == 0)
            firstForFigure = QPoint(x, y);
    }

    repaint();
}

void MainWindow::on_btnFinishFigure_clicked()
{
    if (firstForFigure.x() != 0 || firstForFigure.y() != 0) {
        ui->listDotsFigure->addItem(QString::number(firstForFigure.x()) + " " +
                              QString::number(firstForFigure.y()) + " end");
        firstForFigure = QPoint(0, 0);
    }

    repaint();
}

void MainWindow::on_btnAddFigure_clicked()
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

        ui->listDotsFigure->addItem(QString::number(x) + " " +
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
            ui->listDotsFigure->addItem(QString::number(x) + " " +
                                  QString::number(y) + " end");
            firstForFigure = QPoint(0, 0);
        } else {
            ui->listDotsFigure->addItem(QString::number(x) + " " +
                                  QString::number(y));
        }

        if (firstForFigure.x() == 0 && firstForFigure.y() == 0)
            firstForFigure = QPoint(x, y);
    }

    repaint();
}

void MainWindow::on_btnClear_clicked()
{
    ui->listDotsClip->clear();
    ui->listDotsFigure->clear();
    firstForFigure = QPoint(0, 0);
    firstForClip = QPoint(0, 0);
    repaint();
}
