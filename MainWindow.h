#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <ModelTakuzu.h>

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
    void notifyMapChosen(ModelTakuzu::Difficulty level, int size);

public slots:
//    void registerSetNewGame();
    void registerChronoChanged();
    void registerQuitPressed();
    void registerRulesPressed();
    void registerAboutPressed();

private:
    Ui::MainWindow *_ui;
    QTime *_time;
};

#endif // MAINWINDOW_H*/
