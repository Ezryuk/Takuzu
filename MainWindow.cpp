#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QInputDialog>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    connect(_ui->newGame, SIGNAL(triggered()), this, SLOT(setNewGame()));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

QWidget *MainWindow::getGrid() const
{
    return _ui->gridWidget;
}

void MainWindow::setNewGame()
{
    int sizeMap;
    QString levelMap;

    bool ok;
    QStringList sizes;
    sizes << "6" << "8" << "10";
    QString size = QInputDialog::getItem(this, "Choose map size", "Size :", sizes, 0, false, &ok);
    if (ok && !size.isEmpty()) {
        sizeMap = size.toInt();
        QStringList levels;
        levels << "easy" << "hard";
        QString level = QInputDialog::getItem(this, "Choose difficulty level", "Level :", levels, 0, false, &ok);
        if (ok && !level.isEmpty()) {
            levelMap = level;
            startChrono();
        }
    }
}

void MainWindow::startChrono()
{
    QTimer * chrono = new QTimer(this);
    chrono->start(1000);
    _time = new QTime;
    _time->start();
    connect(chrono, SIGNAL(timeout()), this, SLOT(chronoChanged()));
}

void MainWindow::chronoChanged()
{
    _ui->timeEdit->setTime(QTime(0,0).addMSecs(_time->elapsed()));
}
