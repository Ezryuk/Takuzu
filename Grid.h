#ifndef GRID_H
#define GRID_H

#include <QWidget>
#include <QPainter>

class QPaintEvent;

class Grid : public QWidget
{
    Q_OBJECT
public:
    explicit Grid(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void paintPawn(int row, int column);

signals:

public slots:

private:
    QPainter * _painter;
};

#endif // GRID_H
