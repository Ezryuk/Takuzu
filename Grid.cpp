#include "Grid.h"
#include <QDebug>

Grid::Grid(QWidget *parent) : QWidget(parent)
{

}

void Grid::paintEvent(QPaintEvent *)
{
    _painter = new QPainter(this);
    QPen pen(Qt::black);
    pen.setWidth(3);
    _painter->setPen(pen);

    int width = (QWidget::width()<QWidget::height())?QWidget::width():QWidget::height();
    _widthRect = width/(_rows+2);
    _margin = (QWidget::width()-width+2*_widthRect)/2;

    _rects = new QRect*[_rows];
    _rowCountArea = new QRect[_rows];
    _columnCountArea = new QRect[_rows];

    for (int i = 0; i < _rows; ++i) {
        _rects[i] = new QRect[_rows];
        for (int j = 0; j < _rows; ++j) {
            _rects[i][j] = QRect(i*_widthRect+_margin, j*_widthRect, _widthRect, _widthRect);
            _painter->drawRect(_rects[i][j]);
        }
        _rowCountArea[i] = QRect(_rows*_widthRect+_margin, i*_widthRect, _widthRect*2, _widthRect);
        _columnCountArea[i] = QRect(i*_widthRect+_margin, _rows*_widthRect, _widthRect, _widthRect*2);
    }
    if (!_valid) {
        _painter->fillRect(_rects[_invalidSquare.x()][_invalidSquare.y()], QBrush(Qt::red));
    }
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
    int width = rect.width()/2-2;
    QPoint center(rect.center().x()+2, rect.center().y()+2);
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
        _painter->eraseRect(rect.x()+2, rect.y()+2, rect.width()-2, rect.height()-2);
        break;
    }
}

void Grid::paintCount(bool isRow, int index, int black, int white)
{
    QRect countArea;
    if (isRow) {
        countArea = _rowCountArea[index];
    } else {
        countArea = _columnCountArea[index];
    }

    if (black == _rows/2 && white == _rows/2) {
        _painter->drawText(countArea, Qt::AlignCenter, "OK");
    } else if (isRow) {
        _painter->drawText(countArea, Qt::AlignCenter, QString::number(black) + " | " + QString::number(white));
    } else {
        _painter->drawText(countArea, Qt::AlignCenter, QString::number(black) + "\n - \n" + QString::number(white));
    }
}

void Grid::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
            int x = (event->x()-_margin) / _widthRect;
            int y = event->y() / _widthRect;
            if (x < _rows && event->x()-_margin > 0) {
                if (y < _rows) {
                    emit notifyCoordinatesClicked(x,y);
                }
            }
    }
}

void Grid::setRows(int rows)
{
    _rows = rows;
    _rowCounts = new int[_rows*2]();
    _colCounts = new int[_rows*2]();
    _pawns = new Pawn[_rows*_rows];
    for (int i = 0; i < _rows*_rows; i++) {
        _pawns[i] = Empty;
    }
    repaint();
}

void Grid::registerCount(int i, int j, int Brow, int Bcol, int Wrow, int Wcol)
{
    if (_rows != 0) {
        _rowCounts[i*2] = Brow;
        _rowCounts[i*2+1] = Wrow;
        _colCounts[j*2] = Bcol;
        _colCounts[j*2+1] = Wcol;
    }
}

void Grid::registerPositionIsValid(int i, int j, bool isValid)
{
    if (_rows != 0) {
        _valid = isValid;
        _invalidSquare.setX(i);
        _invalidSquare.setY(j);
    }
}

