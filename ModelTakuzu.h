#ifndef MODELTAKUZU_H
#define MODELTAKUZU_H

#include <QObject>
#include <QString>
#include <ctime>

enum Pawn {
    Black,
    White,
    Empty
};

class ModelTakuzu : public QObject
{
    Q_OBJECT
public:
    enum Difficulty {
        Easy,
        Hard
    };
    ModelTakuzu();
    void loadFile(const QString &name); // it will be private later
    void chooseMapPool(ModelTakuzu::Difficulty difficulty, int size);
    void setRandomMap();
    void playAt(int i, int j, Pawn pawn);
    bool positionIsValid(int i, int j, Pawn pawn); // no const because we simulate the play so attributes are modified
    void updateCount();
    void updateCount(int i, int j, Pawn oldPawn, Pawn newPawn);
public slots:
    void playAt(int i, int j);
private: // private methods
    /**
     * @brief findFirstIdenticalRow
     * @param indexOfUpdatedRow
     * @return int index of the first row which is the same as the row \
     * n° indexOfUpdatedRow. \
     * _sizeOfMap if no identical row found
     */
    int findFirstIdenticalRow(int indexOfUpdatedRow);
    int findFirstIdenticalCol(int indexOfUpdatedCol);
private: // attributes
    int _nbMaps;
    int _sizeMap;

    Difficulty _difficulty;
    char **_grids;
    char *_currentGrid;
    struct {
        char *_Wrow;
        char *_Brow;
        char *_Wcol;
        char *_Bcol;
    } _countPawn;


};

#endif // MODELTAKUZU_H
