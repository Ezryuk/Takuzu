#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QInputDialog>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <QGraphicsEffect>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    connect(_ui->newGame, SIGNAL(triggered()), this, SLOT(registerSetNewGame()));
    connect(_ui->actionQuit, SIGNAL(triggered()), this, SLOT(registerQuitPressed()));
    connect(_ui->actionRules, SIGNAL(triggered()), this, SLOT(registerRulesPressed()));
    connect(_ui->actionAbout, SIGNAL(triggered()), this, SLOT(registerAboutPressed()));
}

MainWindow::~MainWindow()
{
    delete _ui;
    delete _time;
}

void MainWindow::registerSetNewGame()
{
    bool ok;
    QStringList sizes;
    sizes << "6" << "8" << "10";
    QString size = QInputDialog::getItem(this, "Choose map size", "Size :", sizes, 0, false, &ok);
    if (ok && !size.isEmpty()) {
        QStringList levels;
        levels << "Easy" << "Hard";
        QString level = QInputDialog::getItem(this, "Choose difficulty level", "Level :", levels, 0, false, &ok);
        ModelTakuzu::Difficulty difficulty = level=="Easy"?ModelTakuzu::Difficulty::Easy:ModelTakuzu::Difficulty::Hard;
        if (ok && !level.isEmpty()) {
            QMessageBox::StandardButton button;
            button = QMessageBox::question(this, "Choices",
                                           "You have chosen a "+size+"x"+size+" map with the difficulty '"+level+"'.\n"
                                           "Are you sure ?",
                                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (button == QMessageBox::Yes) {
                _ui->gridWidget->setRows(size.toInt());
                emit notifyMapChosen(difficulty, size.toInt());
                startChrono();
            }
        }
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
        QMessageBox::information(this, "Victory !", "You won the game in "
                                 + QString::number(_time->minute()) + " minutes and "
                                 + QString::number(_time->second()) + " seconds !");
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
