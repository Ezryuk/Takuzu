#ifndef IOBSERVER_H
#define IOBSERVER_H

struct Datas {
    int data1;
    char data2;
    bool data3;
};

class IObserver
{
public:

    virtual void update(Datas *datas) = 0;
};

#endif // IOBSERVER_H
