#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMessageBox>
#include <cmath>

#define RADIANS 57.2958

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);

private slots:
    void on_btnOffseX_clicked();

    void on_btnOffsetY_clicked();

    void on_btnRotate_clicked();

    void on_lineOffsetX_returnPressed();

    void on_lineOffsetY_returnPressed();

    void on_btnScale_clicked();

    void on_btnCancel_clicked();

private:
    Ui::MainWindow *ui;
    double checkDouble(QString, bool*);
    QList<double> scaleX;
    QList<double> scaleY;
    QList<double> offsetX;
    QList<double> offsetY;
    QList<double> rotate;
};

#endif // MAINWINDOW_H
