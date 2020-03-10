#ifndef MODELTAKUZU_H
#define MODELTAKUZU_H

#include <QString>
#include <QFile>

class ModelTakuzu
{
public:
    ModelTakuzu();
    void loadFile(const QString &name);
private:
    uint _nbMaps;
    uint _sizeMap;

};

#endif // MODELTAKUZU_H
