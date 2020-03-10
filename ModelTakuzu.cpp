#include "ModelTakuzu.h"

#include <iostream>


ModelTakuzu::ModelTakuzu()
{
    _nbMaps = -1;
    _sizeMap = -1;
    _difficulty = Easy;
    _grids = nullptr;
}

void ModelTakuzu::loadFile(const QString &name)
{
    QFile file(name);
    if (file.open(QFile::ReadOnly)) {
        char buffer[64];
        qint64 lineLenghth = file.readLine(buffer, sizeof(buffer));
        // read number of grids in file
        if (lineLenghth != -1) {
            _nbMaps = atoi(buffer);
        } else {
            std::cerr << "Issue when reading new line. \n";
        }
        // get size of grid
        _sizeMap = name.at(0).digitValue();
        if (_sizeMap == -1) {
            std::cerr << "Issue when reading size of map. \n";
        }

        // each map in separate line until EOF
        // char **_grids;
        _grids = new char*[_nbMaps];
        for (int i = 0; i < _nbMaps; ++i) {
            _grids[i] = new char[_sizeMap]();
            for (int j = 0; j < _sizeMap; ++j) {
                _grids[i][j] = ((_nbMaps * i + j) % 2 == 0 )? 'B' : '.';
                std::cerr << _grids[i][j] << " | ";
            }
            std::cerr << "\n";
        }
        while (lineLenghth != -1) {
            lineLenghth = file.readLine(buffer, sizeof(buffer));

        }
    }
}
