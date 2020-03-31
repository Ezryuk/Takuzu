#ifndef PRESENTERTAKUZU_H
#define PRESENTERTAKUZU_H

#include <QObject>

#include "ModelTakuzu.h"
#include "MainWindow.h"
class PresenterTakuzu : public QObject
{
    Q_OBJECT
public:
    explicit PresenterTakuzu(QObject *parent = nullptr);
    ~PresenterTakuzu();
    void startNewGame();
public:
    ModelTakuzu *model;
    MainWindow *view;
signals:
public slots:
private:
};

#endif // PRESENTERTAKUZU_H
