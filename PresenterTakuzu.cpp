#include "PresenterTakuzu.h"
#include "PlayCommand.h"

PresenterTakuzu::PresenterTakuzu(QObject *parent) : QObject(parent)
{
    _model = new ModelTakuzu();
    _view = new MainWindow();
    _view->show();
    connect(_view, SIGNAL(notifyMapChosen(ModelTakuzu::Difficulty,int)), _model, SLOT(registerChooseMapPool(ModelTakuzu::Difficulty,int)));
    connect(_view->getGrid(), SIGNAL(notifyCoordinatesClicked(int,int)), _model, SLOT(registerPlayAt(int,int)));
    connect(_model, SIGNAL(notifyCount(int,int,int,int,int,int)), _view->getGrid(), SLOT(registerCount(int,int,int,int,int,int)));
    connect(_model, SIGNAL(notifyPositionIsValid(int,int,bool)), _view->getGrid(), SLOT(registerPositionIsValid(int,int,bool)));
    connect(_model, SIGNAL(notifyInitialPawn(int,int,Pawn)), _view->getGrid(), SLOT(registerInitialPawn(int,int,Pawn)));
    connect(_model, SIGNAL(notifyNewPawn(int,int,Pawn)), _view->getGrid(), SLOT(registerNewPawn(int,int,Pawn)));
    _undoStack = new QUndoStack(this);
    _view->getUndoButton()->setDefaultAction(_undoStack->createUndoAction(this, tr("&Undo")));
    _view->getRedoButton()->setDefaultAction(_undoStack->createRedoAction(this, tr("&Redo")));
    connect(_view->getGrid(), SIGNAL(notifyCoordinatesClicked(int, int)), this, SLOT(registerCommand(int, int)));
}

PresenterTakuzu::~PresenterTakuzu()
{
    delete _model;
}

void PresenterTakuzu::registerCommand(int i, int j)
{
    _undoStack->push(new PlayCommand(_model, i, j, _model->getPawn(i, j)));
}

