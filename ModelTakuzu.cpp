#include "ModelTakuzu.h"

#include <iostream>
#include <QFile>
#include <QByteArray>
#include <cstring>

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
    if (file.open(QFile::ReadOnly)) {
        QByteArray line;

        // read number of grids in file
        bool ok = true;
        _nbMaps = file.readLine().toInt(&ok);
        if (!ok) {
            std::cerr << "Issue when reading new line. \n";
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
    }
}

void ModelTakuzu::setDifficulty(ModelTakuzu::Difficulty difficulty)
{
    _difficulty = difficulty;
}

void ModelTakuzu::setSize(int size)
{
    _sizeMap = size;
}

void ModelTakuzu::chooseMapPool(ModelTakuzu::Difficulty difficulty, int size)
{
    QString name = QString::number(size);
    if (difficulty == Easy) {
        name.append("_easy.txt");
    } else /* (difficulty == Hard) */ {
        name.append("_hard.txt");
    }
}




