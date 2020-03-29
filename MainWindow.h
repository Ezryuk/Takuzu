#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void startChrono();
    ~MainWindow();

public slots:
    void setNewGame();
    void chronoChanged();

private:
    Ui::MainWindow *_ui;
    QTime * _time;
};

#endif // MAINWINDOW_H
