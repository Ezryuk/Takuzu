#include <QApplication>

#include "ModelTakuzu.h"

int main(int argc, char *argv[])
{
    srand((unsigned)time(0));
    QApplication a(argc, argv);
//    PresenterTakuzu *presenter = new PresenterTakuzu();
    ModelTakuzu *model = new ModelTakuzu();
    model->loadFile("6_easy.txt");
    return a.exec();

}
