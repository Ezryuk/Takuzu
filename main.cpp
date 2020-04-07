#include <QApplication>

#include "PresenterTakuzu.h"

int main(int argc, char *argv[])
{
    srand((unsigned)time(0));
    QApplication a(argc, argv);
    PresenterTakuzu *presenter = new PresenterTakuzu();

    /*presenter->_model->chooseMapPool(ModelTakuzu::Easy, 6);
    presenter->_model->setRandomMap();
    presenter->_model->playAt(1, 2, Black);
    presenter->_model->positionIsValid(3, 5, White);*/
    return a.exec();

}
