#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->newGame, SIGNAL(triggered()), this, SLOT(setNewGame()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setNewGame() {
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
        }
    }
}
