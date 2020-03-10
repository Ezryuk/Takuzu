#ifndef MODELTAKUZU_H
#define MODELTAKUZU_H

#include <QString>
#include <QFile>

class ModelTakuzu
{
public:
    ModelTakuzu();
    void loadFile(const QString &name); // it will be private later
private:
    int _nbMaps;
    int _sizeMap;
    enum Difficulty {
        Easy,
        Hard
    };
    Difficulty _difficulty;
    char **_grids;
};

#endif // MODELTAKUZU_H
