#ifndef MODELTAKUZU_H
#define MODELTAKUZU_H

#if 0
{
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
    bool positionIsValid(int i, int j);
    /**
     * @brief positionIsValid \
     * no const because we simulate the play of pawn at (i,j)
     * @param i
     * @param j
     * @param pawn
     * @return
     */
    bool positionIsValid(int i, int j, Pawn pawn); // no const because we simulate the play so attributes are modified
    bool rowIsValid(int i);
    bool colIsValid(int j);
    void initCount();
    void updateCount();
    void updateCount(int i, int j, Pawn oldPawn, Pawn newPawn);
    Pawn getPawn(int i, int j) const;
signals:
    void notifyNewPawn(int i, int j, Pawn newPawn);
    void notifyCount(int i, int j, int Brow, int Bcol, int Wrow, int Wcol);
    void notifyPositionIsValid(int i, int j, bool isValid) const;
    void notifyInitialPawn(int i, int j, Pawn readOnlyPawn);
    void notifyOverThreeAdjacentPawns(int index, bool isVertical, bool isOK); // "!isVertical = isHorizontal"
    void notifyCommonPatterns(int first, int second, bool isVertical, bool isOK);
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
}
#endif

#include <algorithm>
#include <set>
#include <string>
#include <vector>
#include <QObject>
#include <QString>


enum Pawn : unsigned char { Empty, Black, White};
using Grid_ = std::vector<Pawn>;

class ModelTakuzu : public QObject {
    Q_OBJECT
public: // types
    enum Difficulty : unsigned char {
        Easy,
        Hard
    };
public: // methods
    ModelTakuzu();
    ~ModelTakuzu();
    static Pawn permuteR(Pawn pawn);
    static Pawn permuteL(Pawn pawn);
    void chooseMapPool(Difficulty difficulty, int size);
    int setMap(int chosenMap);
    int setRandomMap();
    void playAt(int i, int j, Pawn pawn);
    bool positionIsValid(int i, int j); // no const due to emit signal
    bool rowIsValid(int i);
    bool colIsValid(int j);
    void initCount();
    void updateCount(int i, int j, Pawn oldPawn, Pawn newPawn);
    Pawn getPawn(int i, int j) const;
    bool doFinalCheck(); // no const because positionIsValid(int,int) no const

signals:
    void notifyNewPawn(int i, int j, Pawn newPawn);
    void notifyCount(int i, int j, int Brow, int Bcol, int Wrow, int Wcol);
    void notifyPositionIsValid(int i, int j, bool isValid);
    void notifyInitialPawn(int i, int j, Pawn readOnlyPawn);
    void notifyOverThreeAdjacentPawns(int index, bool isVertical, bool isOK); // "!isVertical = isHorizontal"
    void notifyCommonPatterns(int first, int second, bool isVertical, bool isOK);
    void notifyEndGame(bool win);

public slots:
    void registerPlayAt(int i, int j);
    void registerChooseMapPool(ModelTakuzu::Difficulty difficulty, int size);
    void registerAttemptToEndGame();

private: // methods
    void loadFile(const QString &name);
    static QChar toQChar(Pawn pawn);
    static Pawn toPawn(QChar letter);
    Pawn &pawnAt(int i, int j);
    Pawn pawnAt(int i, int j) const;
    std::vector<Pawn> getRow(int i) const;
    std::vector<Pawn> getCol(int j) const;
    static bool isBBBorWWWpresentIn(std::vector<Pawn> vec);
    int findFirstIdenticalRowTo(int i) const;
    int findFirstIdenticalColTo(int j) const;

private: // attributes
    int _nbMaps;
    int _sizeMap;
    Difficulty _difficulty;
    Grid_ *_grids;
    int _chosenMap;
    Grid_ _currentGrid;
    struct {
        std::vector<int> _Wrow;
        std::vector<int> _Brow;
        std::vector<int> _Wcol;
        std::vector<int> _Bcol;
    } _countPawn;
};

#endif // MODELTAKUZU_H
