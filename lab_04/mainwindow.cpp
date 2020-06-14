#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    background = Qt::white;
    foreground = Qt::black;

    QString qss = QString("background-color: %1").arg(foreground.name());
    ui->btnForeground->setStyleSheet(qss);

    QString qssbg = QString("background-color: %1").arg(background.name());
    ui->btnBackground->setStyleSheet(qssbg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    int canvasHeight = height();
    int canvasWidth = width() - MARGIN_RIGHT;

    p.fillRect(0, 0, canvasWidth, canvasHeight, background);
    p.setClipRect(0, 0, canvasWidth, canvasHeight);

    for (Figure* fig : figures) {
        fig->draw(p);
    }

}

void MainWindow::on_btnBackground_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    background = color->getColor();
    QString qss = QString("background-color: %1").arg(background.name());
    ui->btnBackground->setStyleSheet(qss);
    delete color;

    repaint();
}

void MainWindow::on_btnForeground_clicked()
{
    QColorDialog* color = new QColorDialog();
    color->open();
    foreground = color->getColor();
    QString qss = QString("background-color: %1").arg(foreground.name());
    ui->btnForeground->setStyleSheet(qss);
    delete color;
}

void MainWindow::on_btnOneColor_clicked()
{
    foreground = background;
    QString qss = QString("background-color: %1").arg(foreground.name());
    ui->btnForeground->setStyleSheet(qss);
}

void MainWindow::on_btnClear_clicked()
{
    for (Figure* fig : figures) {
        delete fig;
    }

    figures = QList<Figure*>();
    repaint();
}

void MainWindow::on_btnCircle_clicked()
{
    bool keyX = 0, keyY = 0, keyR = 0;
    int Cx = ui->lineCircleX->text().toInt(&keyX);
    int Cy = ui->lineCircleY->text().toInt(&keyY);
    int R = ui->LineCircleR->text().toInt(&keyR);
    Algoritm alg = Algoritm(ui->cmbCircle->currentIndex());

    if (!(keyX || keyY || keyR)) {
       QMessageBox::critical(this, "Ошибка", "Некорректный ввод");
       return;
    }

    figures.append(new Circle(QPoint(Cx, Cy + MARGIN_UP), R, alg, foreground));
    repaint();
}

void MainWindow::on_btnCircles_clicked()
{
    int canvasHeight = height();
    int canvasWidth = width() - MARGIN_RIGHT;

    bool keyStart = 0, keyDelta = 0, keyCount = 0;
    int startR = ui->lineCircleStartR->text().toInt(&keyStart);
    int deltaR = ui->lineCircleDeltaR->text().toInt(&keyDelta);
    int count = ui->lineCircleCount->text().toInt(&keyCount);
    Algoritm alg = Algoritm(ui->cmbCircles->currentIndex());

    if (!(keyStart || keyDelta || keyCount)) {
        QMessageBox::critical(this, "Ошибка", "Некорректный ввод");
        return;
    }

    for (int R = startR, i = 0; i < count; R += deltaR, i++) {
        figures.append(new Circle(QPoint(canvasWidth / 2, canvasHeight / 2 + MARGIN_UP),
                              R, alg, foreground));
    }

    repaint();
}

void MainWindow::on_btnEllipse_clicked()
{
    bool keyX = 0, keyY = 0, keyA = 0, keyB = 0;
    int Cx = ui->lineEllipseX->text().toInt(&keyX);
    int Cy = ui->lineEllipseY->text().toInt(&keyY);
    int a = ui->lineEllipseA->text().toInt(&keyA);
    int b = ui->lineEllipseB->text().toInt(&keyB);;
    Algoritm alg = Algoritm(ui->cmbEllipse->currentIndex());

    if (!(keyX || keyY || keyA || keyB)) {
       QMessageBox::critical(this, "Ошибка", "Некорректный ввод");
       return;
    }

    figures.append(new Ellipse(QPoint(Cx, Cy + MARGIN_UP), a, b, alg, foreground));
    repaint();
}

void MainWindow::on_btnEllipses_clicked()
{
    int canvasHeight = height();
    int canvasWidth = width() - MARGIN_RIGHT;

    bool keyStartA = 0, keyStartB = 0, keyDeltaA = 0, keyDeltaB = 0, keyCount = 0;
    int startA = ui->lineEllipsesA->text().toInt(&keyStartA);
    int startB = ui->lineEllipsesB->text().toInt(&keyStartB);
    int deltaA = ui->lineEllipsesDeltaA->text().toInt(&keyDeltaA);
    int deltaB = ui->lineEllipsesDeltaB->text().toInt(&keyDeltaB);
    int count = ui->lineEllipsesN->text().toInt(&keyCount);
    Algoritm alg = Algoritm(ui->cmbEllipses->currentIndex());

    if (!(keyStartA || keyStartB || keyDeltaA || keyDeltaB || keyCount)) {
        QMessageBox::critical(this, "Ошибка", "Некорректный ввод");
        return;
    }

    for (int a = startA, b = startB, i = 0; i < count; a += deltaA, b += deltaB, i++) {
        figures.append(new Ellipse(QPoint(canvasWidth / 2, canvasHeight / 2 + MARGIN_UP),
                                a, b, alg, foreground));
    }

    repaint();
}
