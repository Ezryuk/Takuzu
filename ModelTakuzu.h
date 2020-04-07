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

namespace TakuzuUtils {
/**
 * @brief isBBBorWWWpresentIn is not meant to be used by external user \
 *
 * @param strToScan pointer to the beginning of char[] to be scanned
 * @return true or false
 */
bool isBBBorWWWpresentIn(char *strToScan);
Pawn permuteR(Pawn p);
Pawn permuteL(Pawn p);
Pawn toPawn(char c);
char toChar(Pawn p);
}

class ModelTakuzu : public QObject
{
    Q_OBJECT
public:
    enum Difficulty {
        Easy,
        Hard
    };
    ModelTakuzu();
    ~ModelTakuzu();
    void loadFile(const QString &name); // it will be private later
    void chooseMapPool(ModelTakuzu::Difficulty difficulty, int size);
    int setMap(int chosenMap);
    int setRandomMap();
    void playAt(int i, int j, Pawn pawn);
    bool positionIsValid(int i, int j) const;
    /**
     * @brief positionIsValid \
     * no const because we simulate the play of pawn at (i,j)
     * @param i
     * @param j
     * @param pawn
     * @return
     */
    bool positionIsValid(int i, int j, Pawn pawn); // no const because we simulate the play so attributes are modified
    bool rowIsValid(int i) const;
    bool colIsValid(int j) const;
    void initCount();
    void updateCount();
    void updateCount(int i, int j, Pawn oldPawn, Pawn newPawn);
    Pawn getPawn(int i, int j) const;
signals:
    void notifyNewPawn(int i, int j, Pawn newPawn);
    void notifyCount(int i, int j, int Brow, int Bcol, int Wrow, int Wcol);
    void notifyPositionIsValid(int i, int j, bool isValid) const;
    void notifyInitialPawn(int i, int j, Pawn readOnlyPawn);
public slots:
    void registerPlayAt(int i, int j);
    void registerChooseMapPool(ModelTakuzu::Difficulty difficulty, int size);

public: // public "attributes". Replace getter + setter
    char *_currentGrid;
    struct {
        char *_Wrow;
        char *_Brow;
        char *_Wcol;
        char *_Bcol;
    } _countPawn;

private: // private methods
    /**
     * @brief findFirstIdenticalRow
     * @param indexOfUpdatedRow
     * @return int index of the first row which is the same as the row \
     * n° indexOfUpdatedRow. \
     * _sizeOfMap if no identical row found
     */
    int findFirstIdenticalRow(int indexOfUpdatedRow) const;
    int findFirstIdenticalCol(int indexOfUpdatedCol) const;

private: // attributes
    int _nbMaps;
    int _sizeMap;
    Difficulty _difficulty;
    char **_grids;
    int _chosenMap;

};

#endif // MODELTAKUZU_H
