#include "ModelTakuzu.h"

#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include <iostream>
#include <cstring>
#include <cassert>



ModelTakuzu::ModelTakuzu()
{
    _nbMaps = -1;
    _sizeMap = -1;
    _difficulty = Easy;
    _grids = nullptr;
    _currentGrid = nullptr;
    _countPawn = {nullptr, nullptr, nullptr, nullptr};
    _chosenMap = -1;
}

ModelTakuzu::~ModelTakuzu()
{
    for (int i = 0; i < _sizeMap; ++i) {
        delete _grids[i];
    }
    delete _grids;
    delete _currentGrid;
    delete _countPawn._Wrow;
    delete _countPawn._Brow;
    delete _countPawn._Wcol;
    delete _countPawn._Bcol;
}

void ModelTakuzu::loadFile(const QString &name)
{
    QFile file(":/resources/"+name);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString line;
        QTextStream in(&file);
        // read number of grids in file
        bool ok = true;
        _nbMaps = in.readLine().toInt(&ok);
        if (!ok) {
            std::cerr << "Issue when reading new line. \n"\
                      << "Make sure the file has the same path as the executable.\n";
        }

//        // get size of grid
//        _sizeMap = name.at(0).digitValue();
//        if (_sizeMap == -1) {
//            std::cerr << "Issue when reading size of map. \n";
//        }

        // read each map in separate line until EOF
        // char **_grids;
        _grids = new char*[_nbMaps];
        for (int i = 0; i < _nbMaps; ++i) {
            _grids[i] = new char[_sizeMap * _sizeMap]();
        }
        {
            int i = 0;
            while (!in.atEnd()) {
                line = in.readLine();
                memcpy(_grids[i++], line.constData(), sizeof(char) * _sizeMap * _sizeMap);
            }
        }
    } else {
        std::cerr << "Error while opening new file: " << name.toStdString() << " .\n";
    }
    setRandomMap();
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
}

int ModelTakuzu::setMap(int chosenMap)
{
    std::cout << "Nb maps: " << _nbMaps <<
                 " Size map: " << _sizeMap << "\n";
    assert((_nbMaps != -1 || _sizeMap != -1) && \
           "Choose a map pool before using setRandomMap().");
    delete _currentGrid; // _currentGrid allocated by setRandomMap() or nullptr

    _currentGrid = new char[_sizeMap * _sizeMap]();
    // load a fresh new grid in _current grid
    memcpy(_currentGrid, _grids[chosenMap], sizeof(char) * _sizeMap * _sizeMap);


    delete _countPawn._Wrow;
    delete _countPawn._Brow;
    delete _countPawn._Wcol;
    delete _countPawn._Bcol;
    _countPawn = {
        new char[_sizeMap](),
        new char[_sizeMap](),
        new char[_sizeMap](),
        new char[_sizeMap]()
    };
    for(int i = 0; i < _sizeMap; ++i) {
        for (int j = 0; j < _sizeMap; ++j) {
            emit notifyInitialPawn(i, j, TakuzuUtils::toPawn(_grids[chosenMap][i * _sizeMap + j]));
        }
    }
    return chosenMap;
}

int ModelTakuzu::setRandomMap()
{
    int randomGridIndex = (rand() % _nbMaps);
    _chosenMap = setMap(randomGridIndex);
    return randomGridIndex;
}

void ModelTakuzu::playAt(int i, int j, Pawn pawn)
{
        std::cout << "call playAt with params " << i << " and " << j << "\n";
    assert((_currentGrid != nullptr) && \
           "Set a map using setRandomMap() before calling playAt().");
    // we could create a map (pawn=>letter) but for 3 elements, not necessary
    char c = 'E'; // E stands for error
    switch (pawn) {
    case Black:
        c = 'B';
        break;
    case White:
        c = 'W';
        break;
    case Empty:
        c = '.';
        break;
    default:
        c = ' ';
        break;
    }
    Pawn oldPawn = TakuzuUtils::toPawn(_currentGrid[i * _sizeMap + j]);
    Pawn newPawn = pawn;
    updateCount(i, j, oldPawn, newPawn);
    _currentGrid[i * _sizeMap + j] = c;
    emit notifyNewPawn(i, j, pawn);
}


void ModelTakuzu::updateCount()
{
    memset(_countPawn._Brow, 0, _sizeMap * sizeof(char));
    memset(_countPawn._Bcol, 0, _sizeMap * sizeof(char));
    memset(_countPawn._Wrow, 0, _sizeMap * sizeof(char));
    memset(_countPawn._Wcol, 0, _sizeMap * sizeof(char));
    for (int i = 0; i < _sizeMap * _sizeMap; ++i) {
        switch (_currentGrid[i]) {
        case 'B':
            _countPawn._Brow[i / _sizeMap]++;
            _countPawn._Bcol[i % _sizeMap]++;
            break;
        case 'W':
            _countPawn._Wrow[i / _sizeMap]++;
            _countPawn._Wcol[i % _sizeMap]++;
            break;
        default:
            break;
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
    return TakuzuUtils::toPawn(_currentGrid[i * _sizeMap + j]);
}

bool ModelTakuzu::positionIsValid(int i, int j) const
{
    char rowToScan[_sizeMap];
    memcpy(rowToScan, _currentGrid + _sizeMap * i, sizeof(char) * _sizeMap);

    char colToScan[_sizeMap];
    colToScan[_sizeMap] = '\0'; // I don't know why i need to do this,
    // it should be a segmentation fault, and std::cout has troubles to
    // find the end of word without it
    for (int rowIndex = 0; rowIndex < _sizeMap;++rowIndex) {
        colToScan[rowIndex] = _currentGrid[rowIndex * _sizeMap + j];
        std::cout << "test " << rowIndex << ": "<< colToScan << "\t";
        std::cout << "size :" << sizeof(colToScan) << "\n";
    }

    bool repetitionInRow = TakuzuUtils::isBBBorWWWpresentIn(rowToScan);
    bool repetitionInCol = TakuzuUtils::isBBBorWWWpresentIn(colToScan);
    int oneOtherIdenticalRow = findFirstIdenticalRow(i);
    int oneOtherIdenticalCol = findFirstIdenticalCol(j);

    bool isValid = (!repetitionInRow &&
                    !repetitionInCol &&
                    (oneOtherIdenticalRow == _sizeMap) &&
                    (oneOtherIdenticalCol == _sizeMap));
    emit notifyPositionIsValid(i, j, isValid);
    return isValid;
}

bool ModelTakuzu::positionIsValid(int i, int j, Pawn pawn)
{
    char oldPawn = _currentGrid[i * _sizeMap + j];
    playAt(i, j, pawn); // simulate the play
    bool result = positionIsValid(i, j);
    _currentGrid[i * _sizeMap + j] = oldPawn; // undo the simulation
    return result;
}

bool ModelTakuzu::rowIsValid(int i) const
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

bool ModelTakuzu::colIsValid(int j) const
{
    static auto forAll = [](bool tab[], int length) -> bool {
        for (int i = 0; i < length; ++i) {
            if (!tab[i]) return false;
        }
        return true;
    }; // better allocate lambda twice in total rather than create a
    // private method ?
    bool tab[_sizeMap];
    for (int i = 0; i < _sizeMap; ++i) {
        tab[i] = positionIsValid(i, j);
    }
    return forAll(tab, _sizeMap) &&
            (_countPawn._Bcol[j] == _countPawn._Wcol[j]);
}

int ModelTakuzu::findFirstIdenticalRow(int i) const
{
    char rowToScan[_sizeMap];
    memcpy(rowToScan, _currentGrid + _sizeMap * i, sizeof(char) * _sizeMap);
    for (int rowIndex = 0; rowIndex < _sizeMap;++rowIndex) {
        if (rowIndex != i) {
            if (strncmp(rowToScan,
                        _currentGrid + _sizeMap * rowIndex,
                        sizeof(char) * _sizeMap)
                    == 0) {
                // we have two identical rows
                return rowIndex;
            }
        }
    }
    return _sizeMap; // we reached the end of rows list, no similarities found
}

int ModelTakuzu::findFirstIdenticalCol(int j) const
{
    char colToScan[_sizeMap];
    for (int rowIndex = 0; rowIndex < _sizeMap;++rowIndex) {
        colToScan[rowIndex] = _currentGrid[rowIndex * _sizeMap + j];
    }
    for (int colIndex = 0; colIndex < _sizeMap; ++colIndex) {
        // I have no idea about how to implement it without double for loop
        // which is inneficient (following lines) (while is the same as for loop)
        if (colIndex != j) {
            // let's compare our column with each other column one by one
            int commomThreshold = 0;
            // compare from left to right char by char
            while ((colToScan[commomThreshold] ==
                    _currentGrid[commomThreshold * _sizeMap + colIndex]) &&
                   commomThreshold < _sizeMap) {
                commomThreshold++;
            }
            // if we reached at the end, it means our two cols are the same
            if (commomThreshold == _sizeMap) {
                // we found two identical columns
                return colIndex;
            }
        }
    }
    return _sizeMap;
}


void ModelTakuzu::registerPlayAt(int i, int j)
{
    Pawn nextPawn = TakuzuUtils::
            permuteR(TakuzuUtils::
                     toPawn(_currentGrid[i * _sizeMap + j]));
    playAt(i, j, nextPawn);
}

void ModelTakuzu::registerChooseMapPool(ModelTakuzu::Difficulty difficulty, int size)
{
    chooseMapPool(difficulty, size);
}

namespace TakuzuUtils {

bool isBBBorWWWpresentIn(char *strToScan) {
    std::cout << "strToScan: " << strToScan << "\n";
    char *occurrencePointerWWW = strstr(strToScan, "WWW");
    char *occurrencePointerBBB = strstr(strToScan, "BBB");
    return (occurrencePointerWWW != NULL || occurrencePointerBBB != NULL);
}

Pawn permuteR(Pawn p) {
    if (p == Black) {
        return White;
    } else if (p == White) {
        return Empty;
    } else /* if (p == Empty) */ {
        return Black;
    }
}

Pawn toPawn(char c) {
    switch (c) {
    case 'B': return Black;
    case 'W': return White;
    case '.': return Empty;
    default: return Empty;
    }
}

char toChar(Pawn p) {
    switch (p) {
    case Black: return 'B';
    case White: return 'W';
    case Empty: return '.';
    default: return '.';
    }
}

Pawn permuteL(Pawn p) {
    if (p == Black) {
        return Empty;
    } else if (p == Empty) {
        return White;
    } else /* if (p == White) */ {
        return Black;
    }
}

}



