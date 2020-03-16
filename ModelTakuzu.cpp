#include "ModelTakuzu.h"

#include <QFile>
#include <QIODevice>
#include <QByteArray>

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
}

void ModelTakuzu::loadFile(const QString &name)
{
    QFile file(name);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray line;

        // read number of grids in file
        bool ok = true;
        _nbMaps = file.readLine().toInt(&ok);
        if (!ok) {
            std::cerr << "Issue when reading new line. \n"\
                      << "Make sure the file has the same path as the executable.\n";
        }

        // get size of grid
        _sizeMap = name.at(0).digitValue();
        if (_sizeMap == -1) {
            std::cerr << "Issue when reading size of map. \n";
        }

        // read each map in separate line until EOF
        // char **_grids;
        _grids = new char*[_nbMaps];
        for (int i = 0; i < _nbMaps; ++i) {
            _grids[i] = new char[_sizeMap * _sizeMap]();
        }
        {
            int i = 0;
            while (!file.atEnd()) {
                line = file.readLine();
                memcpy(_grids[i++], line.constData(), sizeof(char) * _sizeMap * _sizeMap);
            }
        }
    } else {
        std::cerr << "Error while opening new file: " << name.toStdString() << " .\n";
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
    loadFile(QString(name));
}

void ModelTakuzu::setRandomMap()
{
    std::cout << "Nb maps: " << _nbMaps <<
                 " Size map: " << _sizeMap << "\n";
    assert((_nbMaps != -1 || _sizeMap != -1) && \
           "Choose a map pool before using setRandomMap().");
    delete _currentGrid; // _currentGrid allocated by setRandomMap() or nullptr
    int randomGridIndex = (rand() % _nbMaps);
    _currentGrid = new char[_sizeMap * _sizeMap]();
    // load a fresh new grid in _current grid
    memcpy(_currentGrid, _grids[randomGridIndex], sizeof(char) * _sizeMap * _sizeMap);
}

void ModelTakuzu::playAt(int i, int j, Pawn pawn)
{
    assert((_currentGrid != nullptr) && \
           "Set a map using setRandomMap() before calling playAt().");
    // we could create a map (pawn=>letter) but for 3 elements, not necessary
    char c = 'E'; // E stands for error
    switch (pawn) {
    case Black: c = 'B'; break;
    case White: c = 'W'; break;
    case Empty: c = '.'; break;
    default: c = ' '; break;}
    _currentGrid[i * _sizeMap + j] = c;
}

namespace TakuzuUtils {
/**
 * @brief isBBBorWWWpresentIn is not meant to be used by external user \
 *
 * @param strToScan pointer to the beginning of char[] to be scanned
 * @return true or false
 */
bool isBBBorWWWpresentIn(char *strToScan);
bool isBBBorWWWpresentIn(char *strToScan) {
    std::cout << "strToScan: " << strToScan << "\n";
    char *occurencePointerWWW = strstr(strToScan, "WWW");
    char *occurrencePointerBBB = strstr(strToScan, "BBB");
    return (occurencePointerWWW != NULL || occurrencePointerBBB != NULL);
}
}

bool ModelTakuzu::positionIsValid(int i, int j, Pawn pawn)
{
    char oldPawn = _currentGrid[i * _sizeMap + j];
    playAt(i, j, pawn); // simulate the play

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
    int oneOtheerIdenticalCol = findFirstIdenticalCol(j);

    _currentGrid[i * _sizeMap + j] = oldPawn; // undo the simulation
    return (!repetitionInRow &&
            !repetitionInCol &&
            (oneOtherIdenticalRow == _sizeMap) &&
            (oneOtheerIdenticalCol == _sizeMap));
}

int ModelTakuzu::findFirstIdenticalRow(int i)
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

int ModelTakuzu::findFirstIdenticalCol(int j)
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





