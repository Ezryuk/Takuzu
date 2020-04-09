#include "PresenterTakuzu.h"
#include "PlayCommand.h"
#include <QDebug>

PresenterTakuzu::PresenterTakuzu(QObject *parent) : QObject(parent)
{
    _model = new ModelTakuzu();
    _view = new MainWindow();
    _view->show();
    connect(_view, SIGNAL(notifyMapChosen(ModelTakuzu::Difficulty,int)), _model, SLOT(registerChooseMapPool(ModelTakuzu::Difficulty,int)));
    connect(_view->getGrid(), SIGNAL(notifyCoordinatesClicked(int,int)), _model, SLOT(registerPlayAt(int,int)));
    connect(_model, SIGNAL(notifyCount(int,int,int,int,int,int)), _view->getGrid(), SLOT(registerCount(int,int,int,int,int,int)));
    connect(_model, SIGNAL(notifyInitialPawn(int,int,Pawn)), _view->getGrid(), SLOT(registerInitialPawn(int,int,Pawn)));
    connect(_model, SIGNAL(notifyNewPawn(int,int,Pawn)), _view->getGrid(), SLOT(registerNewPawn(int,int,Pawn)));
    _undoStack = new QUndoStack(this);
    _undoStack->setUndoLimit(10);
    _view->getUndoButton()->setDefaultAction(_undoStack->createUndoAction(this, tr("&Undo")));
    _view->getRedoButton()->setDefaultAction(_undoStack->createRedoAction(this, tr("&Redo")));
    connect(_view->getGrid(), SIGNAL(notifyCoordinatesClicked(int, int)), this, SLOT(registerCommand(int, int)));
    connect(_model, SIGNAL(notifyOverThreeAdjacentPawns(int,bool,bool)), _view->getGrid(), SLOT(registerOverThreeAdjacentPawns(int,bool,bool)));
    connect(_model, SIGNAL(notifyCommonPatterns(int,int,bool,bool)), _view->getGrid(), SLOT(registerCommonPatterns(int,int,bool,bool)));
    connect(_view->getUndoButton(), SIGNAL(triggered(QAction*)), this, SLOT(registerUndoRedoTriggered()));
    connect(_view->getRedoButton(), SIGNAL(triggered(QAction*)), this, SLOT(registerUndoRedoTriggered()));
    //connect(_model, SIGNAL(notifyEndGame()), _view, SLOT(registerEndGame()));
}

PresenterTakuzu::~PresenterTakuzu()
{
    delete _model;
}

void PresenterTakuzu::registerCommand(int i, int j)
{
    _undoStack->push(new PlayCommand(_model, i, j, _model->getPawn(i, j)));
    _view->getLabelNbUndo()->setText("Number of undo done : "+QString::number(0));
}

void PresenterTakuzu::registerUndoRedoTriggered()
{
    _view->getLabelNbUndo()->setText("Number of undo done : "+QString::number(_undoStack->count()-_undoStack->index()));
}

