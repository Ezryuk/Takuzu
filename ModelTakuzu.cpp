#include "ModelTakuzu.h"

#include <iostream>

ModelTakuzu::ModelTakuzu()
{

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
            std::cerr << "Issur when reading size of map. \n";
        }
    }
}
