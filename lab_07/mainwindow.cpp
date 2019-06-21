#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    canvasWidth = width() - LEFTMARGIN;
    canvasHeight = height();

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

    firstForLine = QPoint(-1, -1);
    firstForFigure = QPoint(-1, -1);

    pressCmd = false;
    pressShift = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    canvasWidth = width() - LEFTMARGIN;
    canvasHeight = height();

    QPainter p(this);

    p.fillRect(0, 0 , canvasWidth, canvasHeight, backgroundColor);

    p.setPen(QPen(figureColor, 1));
    for (Figure figure : figures) {
        figure.draw(p);
    }
    p.drawPoint(firstForFigure);

    p.setPen(QPen(edgeColor, 1));
    for (Line line : lines) {
        line.draw(p);
    }
    p.drawPoint(firstForLine);

    p.setPen(QPen(clipColor, 2));
    for (Figure figure : figures) {
        figure.clipAll(p, lines);
    }
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        QPoint p(e->x(), e->y());

        if (pressCmd) {
            if (firstForFigure == QPoint(-1, -1)) {
                firstForFigure = p;
            } else {
                Figure figure(firstForFigure.x(), p.x(), firstForFigure.y(), p.y());
                figures.append(figure);
                ui->listFigures->addItem("Xл: " + QString::number(figure.Left()) +
                                         " Xп: " + QString::number(figure.Right()) + "\n" +
                                         "Yв: " + QString::number(figure.Up()) +
                                         " Yн: " + QString::number(figure.Down()));

                firstForFigure = QPoint(-1, -1);
            }
        } else {
            if (firstForLine == QPoint(-1, -1)) {
                firstForLine = p;
            } else {
                if (pressShift) {
                    int x = absx(firstForLine.x() - p.x());
                    int y = absx(firstForLine.y() - p.y());

                    if (x < y) {
                        p = QPoint(firstForLine.x(), p.y());
                    } else {
                        p = QPoint(p.x(), firstForLine.y());
                    }
                }

                Line line = Line(firstForLine, p);
                lines.append(line);
                ui->listLines->addItem("(" + QString::number(line.getA().x()) +
                                       ";" + QString::number(line.getA().y()) +
                                       ") (" + QString::number(line.getB().x()) +
                                       ";" + QString::number(line.getB().y()) + ")");
                firstForLine = QPoint(-1, -1);
            }
        }
    }

    repaint();
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_Shift:
        pressShift = true;
        return;
    case Qt::Key_Control:
        pressCmd = true;
        return;
    default:
        return;
    }

}

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_Shift:
        pressShift = false;
        return;
    case Qt::Key_Control:
        pressCmd = false;
        return;
    default:
        return;
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

void MainWindow::on_btnClear_clicked()
{
    lines = QList<Line>();
    figures = QList<Figure>();
    ui->listLines->clear();
    ui->listFigures->clear();
    repaint();
}

void MainWindow::on_btnAdd_clicked()
{
    bool key1, key2, key3, key4;
    int left = ui->lineXLeft->text().toInt(&key1);
    int right = ui->lineXRight->text().toInt(&key2);
    int up = ui->lineYUp->text().toInt(&key3) + UPMARGIN;
    int down = ui->lineYDown->text().toInt(&key4) + UPMARGIN;

    if (!(key1 && key2 && key3 && key4)) {
        QMessageBox::critical(this, "Ошибка", "Некорректные данные");
        return;
    }

    Figure figure(left, right, up, down);
    figures.append(figure);
    ui->listFigures->addItem("Xл: " + QString::number(left) +
                             " Xп: " + QString::number(right) +
                             "Yв: " + QString::number(up) +
                             " Yн: " + QString::number(down));

    repaint();
}

void MainWindow::on_btnAddLine_clicked()
{
    bool key1, key2, key3, key4;
    int xStart = ui->lineStartX->text().toInt(&key1);
    int yStart = ui->lineStartY->text().toInt(&key2) + UPMARGIN;
    int xFinish = ui->lineFinishX->text().toInt(&key3);
    int yFinish = ui->lineFinishY->text().toInt(&key4) + UPMARGIN;

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
