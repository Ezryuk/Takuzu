#ifndef MODELTAKUZU_H
#define MODELTAKUZU_H

#include <QString>


enum Pawn {
    Black,
    White,
    Empty
};

class ModelTakuzu
{
public:
    enum Difficulty {
        Easy,
        Hard
    };
    ModelTakuzu();
    void loadFile(const QString &name); // it will be private later
    void setDifficulty(ModelTakuzu::Difficulty difficulty); // it will be private later
    void setSize(int size); // it will be private later

    void chooseMapPool(ModelTakuzu::Difficulty difficulty, int size);
    void setRandomMap();
    bool positionIsValid(int i, int j, Pawn pawn) const;
    void playAt(int i, int j, Pawn pawn);

private:
    int _nbMaps;
    int _sizeMap;

    Difficulty _difficulty;
    char **_grids;
    char *_currentGrid;
};

#endif // MODELTAKUZU_H
