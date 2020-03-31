#include <QApplication>

#include "PresenterTakuzu.h"

int main(int argc, char *argv[])
{
    srand((unsigned)time(0));
    QApplication a(argc, argv);
    PresenterTakuzu *presenter = new PresenterTakuzu();

    presenter->model->chooseMapPool(ModelTakuzu::Easy, 6);
    presenter->model->setRandomMap();
    presenter->model->registerPlayAt(1, 2, Black);
    presenter->model->positionIsValid(3, 5, White);
    return a.exec();

}
