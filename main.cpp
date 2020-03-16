#include "MainWindow.h"
#include "ModelTakuzu.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    srand((unsigned)time(0));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ModelTakuzu *model = new ModelTakuzu();
    model->chooseMapPool(ModelTakuzu::Easy, 6);
    model->setRandomMap();
    model->playAt(1, 2, Black);
    model->positionIsValid(3, 5, White);
    return a.exec();

}
