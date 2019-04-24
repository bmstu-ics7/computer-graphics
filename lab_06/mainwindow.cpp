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

    ui->canvas->setScene(&scene);

    seedPixel = QPoint(0, 0);
    seed = false;
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

        if (numbers.count() == 5) {
            figures.append(Figure(QPoint(numbers[0].toInt(),
                                  numbers[1].toInt()),
                    numbers[2].toInt(),
                    numbers[3].toInt()));
        } else {
            p.append(QPoint(numbers[0].toInt(), numbers[1].toInt()));

            if (numbers.count() == 3) {
                figures.append(Figure(p));
                p = QList<QPoint>();
            }

            if (p.count())
                figures.append(Figure(p));
        }
    }

    return figures;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    scene.clear();
    QPixmap pix;
    QPainter p;
    canvasWidth = ui->canvas->width() - 10;
    canvasHeight = ui->canvas->height() - 10;
    QImage img(canvasWidth, canvasHeight, QImage::Format_ARGB32);
    img.fill(backgroundColor);
    p.begin(&img);

    img.setPixelColor(seedPixel.x(), seedPixel.y(), fillColor);

    p.setPen(QPen(edgeColor, 1));
    p.drawLine(0, 0, canvasWidth - 1, 0);
    p.drawLine(canvasWidth - 1, 0, canvasWidth - 1, canvasHeight - 1);
    p.drawLine(canvasWidth - 1, canvasHeight - 1, 0, canvasHeight - 1);
    p.drawLine(0, canvasHeight - 1, 0, 0);

    QList<Figure> figures = getFigures();
    for (Figure figure : figures) {
        figure.draw(img, edgeColor);
    }

    pix.convertFromImage(img);
    scene.addPixmap(pix);
    p.end();
}

void MainWindow::on_btnStart_clicked()
{
    scene.clear();
    QPixmap pix;
    QPainter p;
    canvasWidth = ui->canvas->width() - 10;
    canvasHeight = ui->canvas->height() - 10;
    QImage img(canvasWidth, canvasHeight, QImage::Format_ARGB32);
    img.fill(backgroundColor);
    p.begin(&img);

    img.setPixelColor(seedPixel.x(), seedPixel.y(), fillColor);

    p.setPen(QPen(edgeColor, 1));
    p.drawLine(0, 0, canvasWidth - 1, 0);
    p.drawLine(canvasWidth - 1, 0, canvasWidth - 1, canvasHeight - 1);
    p.drawLine(canvasWidth - 1, canvasHeight - 1, 0, canvasHeight - 1);
    p.drawLine(0, canvasHeight - 1, 0, 0);

    QList<Figure> figures = getFigures();
    for (Figure figure : figures) {
        figure.draw(img, edgeColor);
    }

    fill(img, ui->chbDelay->isChecked());

    pix.convertFromImage(img);
    scene.addPixmap(pix);
    p.end();
}

void wait()
{
    QApplication::processEvents(QEventLoop::AllEvents, 100);
}

QPair<int, int> MainWindow::drawY(QImage& img, QPoint point)
{
    QRgb b;
    QColor c;

    int left = 0;
    int right = 0;

    for (int x = point.x(), y = point.y(); ; ++x) {
        b = img.pixel(x, y);
        c.setRgb(b);

        if (c == edgeColor) {
            right = x - 1;
            break;
        }

        img.setPixelColor(x, y, fillColor);
    }

    for (int x = point.x(), y = point.y(); ; --x) {
        b = img.pixel(x, y);
        c.setRgb(b);

        if (c == edgeColor) {
            left = x + 1;
            break;
        }

        img.setPixelColor(x, y, fillColor);
    }

    return QPair<int, int>(left, right);
}

void MainWindow::findPixel(QStack<QPoint>& stack, QImage img, QPair<int, int> border, int y)
{
    QRgb b;
    QColor c;

    int x = border.first;
    while (x <= border.second) {
        bool fl = false;

        b = img.pixel(x, y);
        c.setRgb(b);
        while (c != edgeColor && c != fillColor && x <= border.second) {
            fl = true;
            ++x;
            b = img.pixel(x, y);
            c.setRgb(b);
        }

        if (fl == true) {
            b = img.pixel(x, y);
            c.setRgb(b);
            if (x == border.second && c != edgeColor && c != fillColor) {
                stack.push(QPoint(x, y));
            } else {
                stack.push(QPoint(x - 1, y));
            }
        }

        fl = false;
        int xt = x;

        while ((c == edgeColor || c == fillColor) && x < border.second) {
            ++x;
            b = img.pixel(x, y);
            c.setRgb(b);
        }

        if (x == xt) {
            ++x;
        }
    }
}


void MainWindow::fill(QImage& img, bool delay)
{
    QStack<QPoint> stack;
    stack.push(seedPixel);

    while (!stack.isEmpty()) {
        QPoint point = stack.pop();
        QPair<int, int> border = drawY(img, point);
        findPixel(stack, img, border, point.y() + 1);
        findPixel(stack, img, border, point.y() - 1);

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

        if (seed) {
            seedPixel = QPoint(x, y);
            repaint();
            return;
        }

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

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        int x = e->x() - LEFT;
        int y = e->y() - UP;

        if (x < 0 || y < 0 || x > width() || y > height())
            return;

        if (seed) {
            seedPixel = QPoint(x, y);
            repaint();
            return;
        }

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
    qDebug() << e->x() << e->y();
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

    if (e->key() == Qt::Key_Space) {
        seed = true;
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

    if (e->key() == Qt::Key_Space) {
        seed = false;
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
                                         tr("Введите координаты через пробел (+ a и b для эллипса)"),
                                         QLineEdit::Normal,
                                         "0 0", &ok);

    QList<QString> numbers = text.split(" ");
    for (int i = 0; i < numbers.count(); ++i) {
        if (numbers[i] == "")
            numbers.removeAt(i);
    }

    if (numbers.count() == 4 && firstPoint == QPoint(0, 0)) {
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

void MainWindow::on_btnSeed_clicked()
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

    if (firstPoint.x() == 0 && firstPoint.y() == 0)
        seedPixel = QPoint(x, y);

    repaint();
}
