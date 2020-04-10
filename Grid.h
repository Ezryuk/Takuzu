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
    void setRows(int rows);

protected:
    void paintEvent(QPaintEvent *) override;
    void paintPawn(int row, int column, Pawn p);
    void paintCount(bool isRow, int index, int black, int white);
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void notifyCoordinatesClicked(int x,int y);

public slots:
    void registerCount(int i, int j, int Brow, int Bcol, int Wrow, int Wcol);
    void registerInitialPawn(int i, int j, Pawn p);
    void registerNewPawn(int i, int j, Pawn p);
    void registerOverThreeAdjacentPawns(int index, bool isVertical, bool isOk);
    void registerCommonPatterns(int first, int second, bool isVertical, bool isOK);

private:
    int _rows = 0;
    int _widthRect;
    int _margin;
    QPen* _pen;
    QPainter* _painter;
    QRect** _rects;
    QRect* _rowCountArea;
    QRect* _columnCountArea;
    int* _rowCounts;
    int* _colCounts;
    bool* _initPawns;
    Pawn* _pawns;
    bool* _invalidVertical;
    bool* _invalidHorizontal;
    bool* _commonRows;
    bool* _commonColumns;
};

#endif // GRID_H
