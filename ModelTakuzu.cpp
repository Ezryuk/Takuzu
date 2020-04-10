#include "ModelTakuzu.h"
#include <cassert>
#include <iostream>
#include <QtAlgorithms>
#include <QFile>
#include <QTextStream>

ModelTakuzu::ModelTakuzu()
{
    _nbMaps  = -1;
    _sizeMap = -1;
    _grids = nullptr;
}

ModelTakuzu::~ModelTakuzu()
{
    delete[] _grids;
}

Pawn ModelTakuzu::permuteR(Pawn p)
{
    if (p == Black) {
        return White;
    } else if (p == White) {
        return Empty;
    } else /* if (p == Empty) */ {
        return Black;
    }
}

Pawn ModelTakuzu::permuteL(Pawn p)
{
    if (p == Black) {
        return Empty;
    } else if (p == Empty) {
        return White;
    } else /* if (p == White) */ {
        return Black;
    }
}

void ModelTakuzu::loadFile(const QString &name)
{
    QFile file(":/resources/"+name);
    if (!(file.open(QIODevice::ReadOnly | QIODevice::Text))) {
        std::cerr << "Error while opening new file: " << name.toStdString() << " .\n";
    } else {
        QString line;
        QTextStream in(&file);

        // read number of grids in file
        bool ok = true;
        _nbMaps = in.readLine().toInt(&ok);
        if (!ok) {
            std::cerr << "Issue when reading new line. \n"\
                      << "Make sure the file has the same path as the executable.\n";
        }
        // allocate the grids
        delete[] _grids;
        _grids = new Grid_[_nbMaps];
        for (int i = 0; i < _nbMaps; ++i) {
            _grids[i] = Grid_(_sizeMap * _sizeMap);
        }

        // fill the grids
        int i = 0;
        while (!in.atEnd()) {
            line = in.readLine();
            int letterIndex = 0;
            QChar letter;
            foreach(letter, line) {
                if (letterIndex < _sizeMap * _sizeMap)
                    _grids[i][letterIndex++] = ModelTakuzu::toPawn(letter);
            }
            i++;
        }
    }
}

void ModelTakuzu::chooseMapPool(ModelTakuzu::Difficulty difficulty, int size)
{
    QString name = QString::number(size);
    if (difficulty == Easy) {
        name.append("_easy.txt");
    } else /* (difficulty == Hard) */ {
        name.append("_hard.txt");
    }
    _difficulty = difficulty;
    _sizeMap = size;
    loadFile(QString(name));
    setRandomMap();
    emit notifyNumberMap(_difficulty, _sizeMap, _chosenMap, _nbMaps);
}

int ModelTakuzu::setMap(int chosenMap)
{
    assert((_nbMaps != -1 || _sizeMap != -1) && \
           "Choose a map pool before using setRandomMap().");
    _chosenMap = chosenMap;
    _currentGrid = _grids[chosenMap];
    _countPawn = {
        std::vector<int>(_sizeMap),
        std::vector<int>(_sizeMap),
        std::vector<int>(_sizeMap),
        std::vector<int>(_sizeMap)
    };
    for(int i = 0; i < _sizeMap; ++i) {
        for (int j = 0; j < _sizeMap; ++j) {
            emit notifyInitialPawn(i, j, _grids[chosenMap][i * _sizeMap + j]); // do not use pawnAt(i, j) here
        }
    }
    initCount();
    return _chosenMap;
}

int ModelTakuzu::setRandomMap()
{
    int randomGridIndex = (rand() % _nbMaps);
    _chosenMap = setMap(randomGridIndex);
    return randomGridIndex;
}

void ModelTakuzu::playAt(int i, int j, Pawn pawn)
{
    assert((!_currentGrid.empty()) && \
           "Set a map using setRandomMap() before calling playAt().");
    Pawn oldPawn = pawnAt(i, j);
    Pawn newPawn = pawn;
    updateCount(i, j, oldPawn, newPawn);
    pawnAt(i, j) = newPawn;
    positionIsValid(i, j);
    emit notifyNewPawn(i, j, pawn);
}

bool ModelTakuzu::positionIsValid(int i, int j)
{
    const bool isVertical = true;
    const bool isOK = true;

    bool repetitionInRow = isBBBorWWWpresentIn(getRow(i));
    bool repetitionInCol = isBBBorWWWpresentIn(getCol(j));
    if (repetitionInRow) {
        emit notifyOverThreeAdjacentPawns(i, !isVertical, !isOK);
    } else /* !repetitionInRow */ {
        emit notifyOverThreeAdjacentPawns(i, !isVertical, isOK);
    }
    if (repetitionInCol) {
        emit notifyOverThreeAdjacentPawns(j, isVertical, !isOK);
    } else /* !repetitionInCol */ {
        emit notifyOverThreeAdjacentPawns(j, isVertical, isOK);
    }

    int oneOtherIdenticalRow = findFirstIdenticalRowTo(i);
    int oneOtherIdenticalCol = findFirstIdenticalColTo(j);
    static auto oneOtherIdenticalRowColIsFound = [this](int index) -> bool {
        return (index != _sizeMap);
    };
    if (oneOtherIdenticalRowColIsFound(oneOtherIdenticalRow)) {
        emit notifyCommonPatterns(i, oneOtherIdenticalRow, !isVertical, isOK);
    } else {
        emit notifyCommonPatterns(i, oneOtherIdenticalRow, !isVertical, !isOK);
    }
    if (oneOtherIdenticalRowColIsFound(oneOtherIdenticalCol)) {
        emit notifyCommonPatterns(j, oneOtherIdenticalCol, isVertical, isOK);
    } else {
        emit notifyCommonPatterns(j, oneOtherIdenticalCol, isVertical, !isOK);
    }

    return (!repetitionInRow &&
            !repetitionInCol &&
            (oneOtherIdenticalRow == _sizeMap) &&
            (oneOtherIdenticalCol == _sizeMap));
}

bool ModelTakuzu::rowIsValid(int i)
{
    static auto forAll = [](bool tab[], int length) -> bool {
        for (int i = 0; i < length; ++i) {
            if (!tab[i]) return false;
        }
        return true;
    };
    bool tab[_sizeMap];
    for (int j = 0; j < _sizeMap; ++j) {
        tab[j] = positionIsValid(i, j);
    }
    return forAll(tab, _sizeMap) &&
            (_countPawn._Brow[i] == _countPawn._Wrow[i]);
}

bool ModelTakuzu::colIsValid(int j)
{
    static auto forAll = [](bool tab[], int length) -> bool {
        for (int i = 0; i < length; ++i) {
            if (!tab[i]) return false;
        }
        return true;
    };
    bool tab[_sizeMap];
    for (int i = 0; i < _sizeMap; ++i) {
        tab[i] = positionIsValid(i, j);
    }
    return forAll(tab, _sizeMap) &&
            (_countPawn._Bcol[j] == _countPawn._Wcol[j]);
}

void ModelTakuzu::initCount()
{
    for (int i = 0; i < _sizeMap; ++i) {
        for (int j = 0; j < _sizeMap; ++j) {
            updateCount(i, j, Empty, _grids[_chosenMap][i * _sizeMap + j]);
        }
    }
}

void ModelTakuzu::updateCount(int i, int j, Pawn oldPawn, Pawn newPawn)
{
    if (oldPawn != newPawn) {
        switch (oldPawn) {
        case Black:
            _countPawn._Brow[i]--;
            _countPawn._Bcol[j]--;
            break;
        case White:
            _countPawn._Wrow[i]--;
            _countPawn._Wcol[j]--;
            break;
        case Empty:
            break;
        }
        switch (newPawn) {
        case Black:
            _countPawn._Brow[i]++;
            _countPawn._Bcol[j]++;
            break;
        case White:
            _countPawn._Wrow[i]++;
            _countPawn._Wcol[j]++;
            break;
        case Empty:
            break;
        }
    }
    emit notifyCount(i, j,
                     _countPawn._Brow[i],
                     _countPawn._Bcol[j],
                     _countPawn._Wrow[i],
                     _countPawn._Wcol[j]);
}

Pawn ModelTakuzu::getPawn(int i, int j) const
{
    return _currentGrid[i * _sizeMap + j];
}

bool ModelTakuzu::doFinalCheck()
{
    std::vector<bool> isValid;
    for(int i = 0; i < _sizeMap; ++i) {
        for(int j = 0; j < _sizeMap; ++j) {
            isValid.push_back(positionIsValid(i, j));
        }
    }

    std::vector<std::vector<Pawn>> rowsAndCols;
    for (int i = 0; i < _sizeMap; ++i) {
        rowsAndCols.push_back(getRow(i));
        rowsAndCols.push_back(getCol(i));
    }

    std::vector<int> counterOcc;
    std::for_each(rowsAndCols.begin(), rowsAndCols.end(), [&counterOcc](std::vector<Pawn> &vec) -> void {
       counterOcc.push_back(std::count(vec.begin(), vec.end(), Black));
       counterOcc.push_back(std::count(vec.begin(), vec.end(), White));
    });

    return (std::all_of(_currentGrid.begin(), _currentGrid.end(),
                       [](Pawn p)->bool { return (p != Empty);}) &&
            std::all_of(isValid.begin(), isValid.end(), [](bool b)-> bool {return b;}) &&
            std::all_of(counterOcc.begin(), counterOcc.end(), [this](int v) -> bool {return (_sizeMap == 2 * v);}));
}

void ModelTakuzu::registerPlayAt(int i, int j)
{
    Pawn nextPawn = permuteR(pawnAt(i, j));
    playAt(i, j, nextPawn);
}

void ModelTakuzu::registerChooseMapPool(ModelTakuzu::Difficulty difficulty, int size)
{
    chooseMapPool(difficulty, size);
}

void ModelTakuzu::registerSizeMapPicked(ModelTakuzu::Difficulty difficulty, int size)
{
    QString name = QString::number(size);
    if (difficulty == Easy) {
        name.append("_easy.txt");
    } else /* (difficulty == Hard) */ {
        name.append("_hard.txt");
    }
    _difficulty = difficulty;
    _sizeMap = size;
    loadFile(QString(name));
    emit notifyNbMaps(_nbMaps);
}

void ModelTakuzu::registerChooseMapPicked(int mapPicked)
{
    _chosenMap = mapPicked;
    setMap(_chosenMap);
    emit notifyNumberMap(_difficulty, _sizeMap, _chosenMap, _nbMaps);
}

void ModelTakuzu::registerAttemptToEndGame()
{
    bool win = doFinalCheck();
    emit notifyEndGame(win);
}



QChar ModelTakuzu::toQChar(Pawn pawn)
{
    switch (pawn) {
    case Black: return QChar('B');
    case White: return QChar('W');
    case Empty: return QChar('.');
    default: return QChar('.');
    }
}

Pawn ModelTakuzu::toPawn(QChar letter)
{
    switch (letter.unicode()) {
    case 'B': return Black;
    case 'W': return White;
    case '.': return Empty;
    default: return Empty;
    }
}

Pawn &ModelTakuzu::pawnAt(int i, int j)
{
    assert(_chosenMap != -1 &&
            "A map should be chosen before trying to access to _currentGrid.");
    return _currentGrid[i * _sizeMap + j];
}

Pawn ModelTakuzu::pawnAt(int i, int j) const
{
    assert(_chosenMap != -1 &&
            "A map should be chosen before trying to access to _currentGrid.");
    return _currentGrid[i * _sizeMap + j];
}

std::vector<Pawn> ModelTakuzu::getRow(int i) const
{
    return std::vector<Pawn>(_currentGrid.begin() + i * _sizeMap,
                             _currentGrid.begin() + (i + 1) * _sizeMap);
}

std::vector<Pawn> ModelTakuzu::getCol(int j) const
{
    std::vector<Pawn> column;
    for (int row = 0; row < _sizeMap; ++row) {
        column.push_back(pawnAt(row, j));
    }
    return column;
}

bool ModelTakuzu::isBBBorWWWpresentIn(std::vector<Pawn> vec)
{
    static std::vector<Pawn> vecBBB = {Black, Black, Black};
    static std::vector<Pawn> vecWWW = {White, White, White};
    auto itB = std::search(vec.cbegin(), vec.cend(), vecBBB.cbegin(), vecBBB.cend());
    auto itW = std::search(vec.cbegin(), vec.cend(), vecWWW.cbegin(), vecWWW.cend());
    return (itB != vec.cend() || itW != vec.cend());
}

int ModelTakuzu::findFirstIdenticalRowTo(int i) const
{
    std::vector<Pawn> rowToScan = getRow(i);
    for (int rowIndex = 0; rowIndex < _sizeMap;++rowIndex) {
        if (rowIndex != i) {
            if (std::equal(_currentGrid.cbegin() + rowIndex * _sizeMap,
                       _currentGrid.cbegin() + (rowIndex + 1) * _sizeMap,
                           rowToScan.cbegin())) {
                return rowIndex; // we have found two identical rows
            }
        }
    }
    return _sizeMap; // we reached the end of rows list, no similarities found
}

int ModelTakuzu::findFirstIdenticalColTo(int j) const
{
    std::vector<Pawn> colToScan = getCol(j);
    for (int colIndex = 0; colIndex < _sizeMap; ++colIndex) {
        if (colIndex != j) {
            std::vector<Pawn> otherCol = getCol(colIndex);
            if (std::equal(colToScan.begin(), colToScan.end(),
                           otherCol.begin(), otherCol.end())) {
                return colIndex;
            }
        }
    }
    return _sizeMap;
}


