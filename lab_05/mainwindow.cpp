#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    edgeColor = Qt::black;
    fillColor = Qt::red;
    backgroundColor = Qt::white;

    QString qss = QString("background-color: %1").arg(edgeColor.name());
    ui->btnColorEdge->setStyleSheet(qss);

    qss = QString("background-color: %1").arg(fillColor.name());
    ui->btnColorFill->setStyleSheet(qss);

    qss = QString("background-color: %1").arg(backgroundColor.name());
    ui->btnColorBackground->setStyleSheet(qss);

    firstPoint = QPoint(0, 0);
    horizontal = false;
    vertical = false;

    canvasWidth = width() - MARGINRIGHT;
    canvasHeight = height();

    stop = true;

    ui->canvas->setScene(&scene);
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
    scene.clear();
    QPixmap pix;
    QPainter p;
    QImage img(ui->canvas->width() - 1, ui->canvas->height() - 1, QImage::Format_ARGB32);
    img.fill(backgroundColor);
    p.begin(&img);
    p.setPen(QPen(edgeColor, 1));

    QList<Figure> figures = getFigures();
    for (Figure figure : figures) {
        figure.draw(img, edgeColor);
    }

    pix.convertFromImage(img);
    scene.addPixmap(pix);
    p.end();
}

void wait()
{
    QApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::fill(QImage& img, QList<Figure> figures, bool delay)
{
    QList<QPoint> points;

    for (Figure figure : figures) {
        QList<QPoint> p = figure.getPoints();
        if (p[0] == p[p.count() - 1] && p.count() > 1)
            points += p;
    }

    if (points.count() == 0) {
        stop = true;
        return;
    }

    int ymin = points[0].y(), ymax = points[0].y();
    for (QPoint point : points) {
        if (point.y() < ymin) ymin = point.y();
        if (point.y() > ymax) ymax = point.y();
    }

    for (int y = ymin; y <= ymax; ++y) {
        if (!borders[y].use())
            continue;

        bool flag = false;

        for (int x = borders[y].left(); x <= borders[y].right(); ++x) {
            QRgb b = img.pixel(x, y);
            QColor c;
            c.setRgb(b);

            if (c == edgeColor) {
                flag = !flag;
            }

            if (flag) {
                img.setPixelColor(x, y, fillColor);
            }
        }

        if (delay) {
            scene.clear();
            QPixmap pix;
            pix.convertFromImage(img);
            scene.addPixmap(pix);
            wait();
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        int x = e->x() - LEFT;
        int y = e->y() - UP;

        if (x < 0 || y < 0 || x > width() || y > height())
            return;

        if (horizontal && ui->listDots->count())
            y = ui->listDots->item(ui->listDots->count() - 1)->
                    text().split(" ")[1].toInt();

        if (vertical && ui->listDots->count())
            x = ui->listDots->item(ui->listDots->count() - 1)->
                    text().split(" ")[0].toInt();

        if (firstPoint.x() == x && firstPoint.y() == y) {
            ui->listDots->addItem(QString::number(x) + " " +
                                  QString::number(y) + " end");
            firstPoint = QPoint(0, 0);
        } else {
            ui->listDots->addItem(QString::number(x) + " " +
                                  QString::number(y));
        }

        if (firstPoint.x() == 0 && firstPoint.y() == 0)
            firstPoint = QPoint(x, y);
    }

    repaint();
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Shift) {
        horizontal = true;
        vertical = false;
    }

    if (e->key() == Qt::Key_Control) {
        vertical = true;
        horizontal = false;
    }

    if (e->key() == Qt::Key_Return) {
        on_btnFigure_clicked();
    }

    if (e->key() == Qt::Key_Backspace) {
        on_btnClear_clicked();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Shift) {
        horizontal = false;
    }

    if (e->key() == Qt::Key_Control) {
        vertical = false;
    }
}

void MainWindow::on_btnColorEdge_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    edgeColor = color->getColor();
    QString qss = QString("background-color: %1").arg(edgeColor.name());
    ui->btnColorEdge->setStyleSheet(qss);
    delete color;
    repaint();
}

void MainWindow::on_btnColorFill_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    fillColor = color->getColor();
    QString qss = QString("background-color: %1").arg(fillColor.name());
    ui->btnColorFill->setStyleSheet(qss);
    delete color;
    repaint();
}

void MainWindow::on_btnColorBackground_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    backgroundColor = color->getColor();
    QString qss = QString("background-color: %1").arg(backgroundColor.name());
    ui->btnColorBackground->setStyleSheet(qss);
    delete color;
    repaint();
}

void MainWindow::on_btnAdd_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Ввод точки"),
                                         tr("Введите координаты через пробел"),
                                         QLineEdit::Normal,
                                         "0 0", &ok);

    QList<QString> numbers = text.split(" ");
    for (int i = 0; i < numbers.count(); ++i) {
        if (numbers[i] == "")
            numbers.removeAt(i);
    }

    if (numbers.count() != 2) {
        return;
    }

    bool xOk, yOk;
    int x = numbers[0].toInt(&xOk);
    int y = numbers[1].toInt(&yOk);

    if (!xOk || !yOk) {
        return;
    }

    if (firstPoint.x() == x && firstPoint.y() == y) {
        ui->listDots->addItem(QString::number(x) + " " +
                              QString::number(y) + " end");
        firstPoint = QPoint(0, 0);
    } else {
        ui->listDots->addItem(QString::number(x) + " " +
                              QString::number(y));
    }

    if (firstPoint.x() == 0 && firstPoint.y() == 0)
        firstPoint = QPoint(x, y);

    repaint();
}

void MainWindow::on_btnDelete_clicked()
{
    QListWidgetItem* current = ui->listDots->currentItem();
    if (current)
        delete current;

    repaint();
}

void MainWindow::on_btnClear_clicked()
{
    ui->listDots->clear();
    firstPoint = QPoint(0, 0);
    repaint();
}

void MainWindow::on_btnFigure_clicked()
{
    if (firstPoint.x() != 0 || firstPoint.y() != 0) {
        ui->listDots->addItem(QString::number(firstPoint.x()) + " " +
                              QString::number(firstPoint.y()) + " end");
        firstPoint = QPoint(0, 0);
    }

    repaint();
}

void MainWindow::on_btnStart_clicked()
{
    scene.clear();
    QPixmap pix;
    QPainter p;
    QImage img(ui->canvas->width(), ui->canvas->height(), QImage::Format_ARGB32);
    img.fill(backgroundColor);
    p.begin(&img);
    p.setPen(QPen(edgeColor, 1));

    QList<Figure> figures = getFigures();

    borders = QList<Border>();
    for (int i = 0; i < ui->canvas->height(); ++i)
            borders.append(Border());

    for (Figure figure : figures) {
        figure.drawFirst(img, edgeColor, backgroundColor, borders);
    }

    fill(img, figures, ui->chbDelay->isChecked());

    for (Figure figure : figures) {
        figure.draw(img, edgeColor);
    }

    pix.convertFromImage(img);
    scene.addPixmap(pix);
    p.end();
}
