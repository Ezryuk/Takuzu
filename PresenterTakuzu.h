#ifndef PRESENTERTAKUZU_H
#define PRESENTERTAKUZU_H

#include <QObject>
#include <QUndoStack>

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
    ModelTakuzu *_model;
    MainWindow *_view;
signals:
public slots:
    void registerCommand(int i, int j);
    void registerUndoRedoTriggered();
private:
    QUndoStack *_undoStack;
};

#endif // PRESENTERTAKUZU_H
