#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>

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
    QWidget *getGrid() const;
    QToolButton *getRedoButton() const;
    QToolButton *getUndoButton() const;

signals:
    void notifyMapChosen(QString size, QString level);

public slots:
    void registerSetNewGame();
    void registerChronoChanged();

private:
    Ui::MainWindow *_ui;
    QTime *_time;
};

#endif // MAINWINDOW_H
