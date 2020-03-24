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
    int widthRect = width/(_rows+2);

    _rects = new QRect*[_rows];
    _rowCount = new QRect[_rows];
    _columnCount = new QRect[_rows];

    for (int i = 0; i < _rows; ++i) {
        _rects[i] = new QRect[_rows];
        for (int j = 0; j < _rows; ++j) {
            _rects[i][j] = QRect(i*widthRect, j*widthRect, widthRect, widthRect);
            _painter->drawRect(_rects[i][j]);
        }
        _rowCount[i] = QRect(_rows*widthRect, i*widthRect, widthRect*2, widthRect);
        _columnCount[i] = QRect(i*widthRect, _rows*widthRect, widthRect, widthRect*2);
        _painter->drawRect(_rowCount[i]);
        _painter->drawRect(_columnCount[i]);
    }
    _painter->end();
}

void Grid::paintPawn(int row, int column, Pawn p) {
    QRect rect = _rects[row][column];
    int width = rect.width()/2-2;
    QPoint center(rect.center().x()+2, rect.center().y()+2);
    switch(p) {
    case Pawn::Black:
        _painter->setBrush(Qt::black);
        _painter->drawEllipse(center, width, width);
        break;
    case Pawn::White:
        _painter->setBrush(Qt::white);
        _painter->drawEllipse(center, width, width);
        break;
    case Pawn::Empty:
        _painter->eraseRect(rect.x()+2, rect.y()+2, rect.width()-2, rect.height()-2);
        break;
    }
}

void Grid::paintCount(bool isRow, int index, int black, int white)
{
    QRect countArea;
    if (isRow) {
        countArea = _rowCount[index];
    } else {
        countArea = _columnCount[index];
    }

    if (black == _rows/2 && white == _rows/2) {
        _painter->drawText(countArea, Qt::AlignCenter, "OK");
    } else if (isRow) {
        _painter->drawText(countArea, Qt::AlignCenter, QString::number(black) + " | " + QString::number(white));
    } else {
        _painter->drawText(countArea, Qt::AlignCenter, QString::number(black) + "\n | \n" + QString::number(white));
    }
}

void Grid::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
            int x = event->x();
            qDebug() << QString::number(x);
    }
}
