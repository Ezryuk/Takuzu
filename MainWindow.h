#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <QPushButton>
#include <ModelTakuzu.h>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QWidget *getGrid() const;
    QToolButton *getRedoButton() const;
    QToolButton *getUndoButton() const;
    QLabel *getLabelNbUndo() const;
    QPushButton *getSubmitButton() const;

signals:
    void notifyMapChosen(ModelTakuzu::Difficulty level, int size);
    void notifySizeMapPicked(ModelTakuzu::Difficulty level, int size);
    void notifMapPicked(int mapPicked);

public slots:
    void registerSetNewGame();
    void registerNewGamePicked();
    void registerPickMap(int nbMaps);
    void registerChronoChanged();
    void registerQuitPressed();
    void registerRulesPressed();
    void registerAboutPressed();
    void registerEndGame(bool win);
    void registerShortcutsPressed();
    void registerNumberMap(ModelTakuzu::Difficulty difficulty, int sizeMap, int chosenMap, int nbMaps);

private:
    Ui::MainWindow* _ui;
    QTime* _time;
    QTimer* _chrono;

    QMessageBox::StandardButton setGame(ModelTakuzu::Difficulty &difficulty, QString &size);
    void startChrono();
};

#endif // MAINWINDOW_H*/
