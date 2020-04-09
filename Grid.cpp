#include "Grid.h"
#include <QDebug>

Grid::Grid(QWidget *parent) : QWidget(parent)
{

}

void Grid::paintEvent(QPaintEvent *)
{
    _painter = new QPainter(this);
    _pen = new QPen(Qt::black);
    _pen->setWidth(3);
    _painter->setPen(*_pen);

    int width = (QWidget::width()<QWidget::height())?QWidget::width():QWidget::height();
    _widthRect = width/(_rows+2);
    _margin = (QWidget::width()-width+2*_widthRect)/2;

    _rects = new QRect*[_rows];
    _rowCountArea = new QRect[_rows*2];
    _columnCountArea = new QRect[_rows*2];

    // Grid drawing
    for (int i = 0; i < _rows; ++i) {
        _rects[i] = new QRect[_rows];
        for (int j = 0; j < _rows; ++j) {
            _rects[i][j] = QRect(i*_widthRect+_margin, j*_widthRect, _widthRect, _widthRect);
            _painter->drawRect(_rects[i][j]);
        }
        _rowCountArea[i*2] = QRect(_rows*_widthRect+_margin, i*_widthRect, _widthRect, _widthRect);
        _columnCountArea[i*2] = QRect(i*_widthRect+_margin, _rows*_widthRect, _widthRect, _widthRect);
        _rowCountArea[i*2+1] = QRect((_rows+1)*_widthRect+_margin, i*_widthRect, _widthRect, _widthRect);
        _columnCountArea[i*2+1] = QRect(i*_widthRect+_margin, (_rows+1)*_widthRect, _widthRect, _widthRect);
    }

    // Draws in red invalid rows and highlights common rows
    QRect rect;
    for (int i = 0; i < _rows; i++) {
        if (_invalidHorizontal[i]) {
            for (int j = 0; j < _rows; j++) {
                rect = _rects[j][i];
                _painter->fillRect(rect.x()+3, rect.y()+3, rect.width()-3, rect.height()-3, QBrush(Qt::red));
            }
        }
        if (_invalidVertical[i]) {
            for (int j = 0; j < _rows; j++) {
                rect = _rects[i][j];
                _painter->fillRect(rect.x()+3, rect.y()+3, rect.width()-3, rect.height()-3, QBrush(Qt::red));
            }
        }
        QPen penYellow(Qt::yellow);
        penYellow.setWidth(3);
        _painter->setPen(penYellow);
        if (_commonRows[i]!=_rows) {
            rect = _rects[0][i];
            _painter->drawRect(rect.x(), rect.y(), _rows*_widthRect, _widthRect);
            rect = _rects[0][_commonRows[i]];
            _painter->drawRect(rect.x(), rect.y(), _rows*_widthRect, _widthRect);
        }
        if (_commonColumns[i]!=_rows) {
            rect = _rects[i][0];
            _painter->drawRect(rect.x(), rect.y(), _widthRect, _rows*_widthRect);
            rect = _rects[_commonColumns[i]][0];
            _painter->drawRect(rect.x(), rect.y(), _widthRect, _rows*_widthRect);
        }
        _painter->setPen(*_pen);
    }

    // Pawns and counts drawing
    for (int i = 0; i < _rows; i++) {
        paintCount(true, i, _rowCounts[i*2], _rowCounts[i*2+1]);
        paintCount(false, i, _colCounts[i*2], _colCounts[i*2+1]);
        for (int j = 0; j < _rows; j++) {
            paintPawn(i, j, _pawns[i*_rows+j]);
        }
    }

    _painter->end();
}

void Grid::paintPawn(int row, int column, Pawn p) {
    QRect rect = _rects[row][column];
    int width = rect.width()/2-3;
    QPoint center(rect.center().x()+1.8, rect.center().y()+1.8);
    switch(p) {
    case Black:
        _painter->setBrush(Qt::black);
        _painter->drawEllipse(center, width, width);
        break;
    case White:
        _painter->setBrush(Qt::white);
        _painter->drawEllipse(center, width, width);
        break;
    case Empty:
        _painter->eraseRect(rect.x()+3, rect.y()+3, rect.width()-3, rect.height()-3);
        break;
    }
    if (_initPawns[row*_rows+column]) {
        QPen penInit(Qt::blue);
        penInit.setWidth(3);
        _painter->setPen(penInit);
        _painter->drawEllipse(center, width, width);
        _painter->setPen(*_pen);
    }
}

void Grid::paintCount(bool isRow, int index, int black, int white)
{
    QRect countAreaBlack;
    QRect countAreaWhite;
    if (isRow) {
        countAreaBlack = _rowCountArea[index*2];
        countAreaWhite = _rowCountArea[index*2+1];
    } else {
        countAreaBlack = _columnCountArea[index*2];
        countAreaWhite = _columnCountArea[index*2+1];
    }

    if (black == _rows/2 && white == _rows/2) {
        _painter->fillRect(countAreaBlack, Qt::green);
        _painter->fillRect(countAreaWhite, Qt::green);
        _painter->drawText(countAreaBlack, Qt::AlignCenter, "O");
        _painter->drawText(countAreaWhite, Qt::AlignCenter, "K");
    } else {
        _painter->fillRect(countAreaBlack, Qt::black);
        _painter->fillRect(countAreaWhite, Qt::white);
        QPen penWhite(Qt::white);
        penWhite.setWidth(3);
        _painter->setPen(penWhite);
        _painter->drawText(countAreaBlack, Qt::AlignCenter, QString::number(black));
        _painter->setPen(*_pen);
        _painter->drawText(countAreaWhite, Qt::AlignCenter, QString::number(white));
    }
}

void Grid::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
            int x = (event->x()-_margin) / _widthRect;
            int y = event->y() / _widthRect;
            if (x < _rows && event->x()-_margin > 0) {
                if (y < _rows) {
                    if (!_initPawns[x*_rows+y]) {
                        emit notifyCoordinatesClicked(y,x);
                    }
                }
            }
    }
}

void Grid::setRows(int rows)
{
    _rows = rows;
    _rowCounts = new int[_rows*2]();
    _colCounts = new int[_rows*2]();
    _initPawns = new bool[_rows*_rows];
    _pawns = new Pawn[_rows*_rows];
    _invalidHorizontal = new bool[_rows];
    _invalidVertical = new bool[_rows];
    _commonRows = new int[_rows];
    _commonColumns = new int[_rows];

    for (int i = 0; i < _rows; i++) {
        _invalidHorizontal[i] = false;
        _invalidVertical[i] = false;
        _commonRows[i] = _rows;
        _commonColumns[i] = _rows;
        for (int j = 0; j < _rows; j++) {
            _initPawns[i*_rows+j] = false;
            _pawns[i*_rows+j] = Empty;
        }
    }

    repaint();
}

void Grid::registerCount(int i, int j, int Brow, int Bcol, int Wrow, int Wcol)
{
    _rowCounts[i*2] = Brow;
    _rowCounts[i*2+1] = Wrow;
    _colCounts[j*2] = Bcol;
    _colCounts[j*2+1] = Wcol;
    repaint();
}

void Grid::registerInitialPawn(int i, int j, Pawn p)
{
    if (p != Empty) {
        _initPawns[j*_rows+i] = true;
    }
    _pawns[j*_rows+i] = p;
    repaint();
}

void Grid::registerNewPawn(int i, int j, Pawn p)
{
    _pawns[j*_rows+i] = p;
    repaint();
}

void Grid::registerOverThreeAdjacentPawns(int index, bool isVertical, bool isOk)
{
    if (isVertical) {
        _invalidVertical[index] = !isOk;
    } else {
        _invalidHorizontal[index] = !isOk;
    }
}

void Grid::registerCommonPatterns(int first, int second, bool isVertical, bool isOK)
{
    qDebug() << first << " and " << second << " " << isOK << " vertical " << isVertical;
    if (isVertical) {
        _commonColumns[first] = second;
        if (!isOK) {
            _commonColumns[second] = first;
        } else {
            _commonColumns[_commonColumns[first]] = _rows;
        }
    } else {
        _commonRows[first] = second;
        if (!isOK) {
            _commonRows[second] = first;
        } else {
            _commonRows[_commonRows[first]] = _rows;
        }
    }
}

