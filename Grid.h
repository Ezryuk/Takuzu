#ifndef GRID_H
#define GRID_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "ModelTakuzu.h"

class QPaintEvent;

class Grid : public QWidget
{
    Q_OBJECT
public:
    explicit Grid(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void paintPawn(int row, int column, Pawn p);
    /**
     * @param isRow true if row / false if column
     * @param index row or column index
     * @param black number of black pawns
     * @param white number of white pawns
     */
    void paintCount(bool isRow, int index, int black, int white);
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void notifyCoordinatesClicked(int x,int y);

public slots:

private:
    int _rows = 8;
    int _widthRect;
    QPainter * _painter;
    QRect** _rects;
    QRect* _rowCount;
    QRect* _columnCount;
};

#endif // GRID_H
