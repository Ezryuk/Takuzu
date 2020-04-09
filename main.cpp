#include <QApplication>

#include "PresenterTakuzu.h"

int main(int argc, char *argv[])
{
    srand((unsigned)time(0));
    QApplication a(argc, argv);
    PresenterTakuzu *presenter = new PresenterTakuzu();

    return a.exec();

}
