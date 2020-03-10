#include "MainWindow.h"
#include <QApplication>

#include "ModelTakuzu.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();

    ModelTakuzu *model = new ModelTakuzu();
    model->loadFile(QString("6_easy.txt"));

}
