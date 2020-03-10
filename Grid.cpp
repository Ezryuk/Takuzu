#include "Grid.h"

Grid::Grid(QWidget *parent) : QWidget(parent)
{

}

void Grid::paintEvent(QPaintEvent *)
{
    _painter = new QPainter(this);
    QPen pen(Qt::black);
    pen.setWidth(3);
    _painter->setPen(pen);

    int rows = 8;
    int width = (QWidget::width()<QWidget::height())?QWidget::width():QWidget::height();

    QRect rects[rows][rows];

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < rows; ++j) {
            rects[i][j] = QRect(i*width/rows, j*width/rows, width/rows, width/rows);
            _painter->drawRect(rects[i][j]);
        }
    }
    _painter->end();
}

void Grid::paintPawn(int row, int column) {

}
