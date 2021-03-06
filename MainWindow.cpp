#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QGraphicsEffect>
#include <QInputDialog>
#include <QPropertyAnimation>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _ui->submitButton->setEnabled(false);

    connect(_ui->newGame, SIGNAL(triggered()), this, SLOT(registerSetNewGame()));
    connect(_ui->newGamePicked, SIGNAL(triggered()), this, SLOT(registerNewGamePicked()));
    connect(_ui->actionQuit, SIGNAL(triggered()), this, SLOT(registerQuitPressed()));
    connect(_ui->actionRules, SIGNAL(triggered()), this, SLOT(registerRulesPressed()));
    connect(_ui->actionAbout, SIGNAL(triggered()), this, SLOT(registerAboutPressed()));
    connect(_ui->actionShortcuts, SIGNAL(triggered()), this, SLOT(registerShortcutsPressed()));
}

MainWindow::~MainWindow()
{
    delete _ui;
    delete _time;
}

void MainWindow::registerSetNewGame()
{
    QString size;
    ModelTakuzu::Difficulty difficulty;
    if (setGame(difficulty, size) == QMessageBox::Yes) {
        _ui->gridWidget->setRows(size.toInt());
        emit notifyMapChosen(difficulty, size.toInt());
        startChrono();
        _ui->submitButton->setEnabled(true);
    }
}

void MainWindow::registerNewGamePicked()
{
    QString size;
    ModelTakuzu::Difficulty difficulty;
    if (setGame(difficulty, size) == QMessageBox::Yes) {
        _ui->gridWidget->setRows(size.toInt());
        emit notifySizeMapPicked(difficulty, size.toInt());
    }
}

QMessageBox::StandardButton MainWindow::setGame(ModelTakuzu::Difficulty &difficulty, QString &size)
{
    bool ok;
    QStringList sizes;
    sizes << "6" << "8" << "10";
    size = QInputDialog::getItem(this, "Choose map size", "Size :", sizes, 0, false, &ok);
    if (ok && !size.isEmpty()) {
        QStringList levels;
        levels << "Easy" << "Hard";
        QString level = QInputDialog::getItem(this, "Choose difficulty level", "Level :", levels, 0, false, &ok);
        difficulty = level=="Easy"?ModelTakuzu::Difficulty::Easy:ModelTakuzu::Difficulty::Hard;
        if (ok && !level.isEmpty()) {
            QMessageBox::StandardButton button;
            button = QMessageBox::question(this, "Choices",
                                           "You have chosen a "+size+"x"+size+" grid with the difficulty '"+level+"'.\n"
                                           "Are you sure ?",
                                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            return button;
        }
    }
}

void MainWindow::registerPickMap(int nbMaps)
{
    bool ok;
    QStringList maps;
    for (int i = 1; i <= nbMaps; i++) {
        maps << QString::number(i);
    }
    QString mapPicked = QInputDialog::getItem(this, "Choose one grid", "Grid number :", maps, 0, false, &ok);
    if (ok && !mapPicked.isEmpty()) {
        emit notifMapPicked(mapPicked.toInt()-1);
        startChrono();
        _ui->submitButton->setEnabled(true);
    }
}

void MainWindow::startChrono()
{
    _chrono = new QTimer(this);
    _chrono->start(1000);
    _time = new QTime;
    _time->start();
    connect(_chrono, SIGNAL(timeout()), this, SLOT(registerChronoChanged()));
}

void MainWindow::registerChronoChanged()
{
    _ui->timeEdit->setTime(QTime(0,0).addMSecs(_time->elapsed()));
}

void MainWindow::registerNumberMap(ModelTakuzu::Difficulty difficulty, int sizeMap, int chosenMap, int nbMaps)
{
    _ui->mapLabel->setText("Grid : " + QString::number(sizeMap)+ "x" + QString::number(sizeMap)
                           + " " + QString(difficulty) + " "
                           + QString::number(chosenMap+1) + "/" + QString::number(nbMaps));
}

void MainWindow::registerQuitPressed()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, "You want to quit...",
                                   "Are you sure that you want to quit"
                                   " this application ?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (button == QMessageBox::Yes) {
        close();
    }
}

void MainWindow::registerRulesPressed()
{
    QMessageBox::information(this, "Rules of Takuzu", "To win the game, you must fill the grid with black and white pawns respecting three rules:\n\n"
                             "-Same number of black and white pawns on each row and each column;\n"
                             "-No more than 2 pawns of the same color in a raw;\n"
                             "-2 rows or 2 columns must not be the same.");
}

void MainWindow::registerAboutPressed()
{
    QMessageBox::information(this, "About this application", "Takuzu\n"
                             "(c) 2020 Christian Zheng and Quentin Derambure");
}

void MainWindow::registerEndGame(bool winStatus)
{
    if (winStatus) {
        _chrono->stop();
        QTime endTime = QTime(0,0).addMSecs(_time->elapsed());
        QMessageBox::information(this, "Victory !", "You won the game in "
                                 + QString::number(endTime.minute()) + " minutes and "
                                 + QString::number(endTime.second()) + " seconds !");
    } else {
        _ui->incorrectLabel->setText("The grid is not correctly filled.");
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
        _ui->incorrectLabel->setGraphicsEffect(effect);
        QPropertyAnimation *animation = new QPropertyAnimation(effect,"opacity");
        animation->setDuration(3000);
        animation->setStartValue(1.0);
        animation->setEndValue(0.0);
        animation->setEasingCurve(QEasingCurve::OutQuad);
        connect(animation, &QPropertyAnimation::finished, [=](){
            _ui->incorrectLabel->setText("");
        });
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MainWindow::registerShortcutsPressed()
{
    QMessageBox::information(this, "Shortcuts", "New Random Game : CTRL + N\n"
                             "Pick One Grid : CTRL + P\n"
                             "Quit : CTRL + Q\n"
                             "Undo : CTRL + Z\n"
                             "Redo : CTRL + SHIFT + Z\n"
                             "Submit : Space");
}

QWidget *MainWindow::getGrid() const
{
    return _ui->gridWidget;
}

QToolButton *MainWindow::getRedoButton() const
{
    return _ui->redoButton;
}

QToolButton *MainWindow::getUndoButton() const
{
    return _ui->undoButton;
}

QLabel *MainWindow::getLabelNbUndo() const
{
    return _ui->labelUndo;
}

QPushButton *MainWindow::getSubmitButton() const
{
    return _ui->submitButton;
}
