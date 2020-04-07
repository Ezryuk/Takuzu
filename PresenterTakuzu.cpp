#include "PresenterTakuzu.h"
#include "PlayCommand.h"

PresenterTakuzu::PresenterTakuzu(QObject *parent) : QObject(parent)
{
    _model = new ModelTakuzu();
    _view = new MainWindow();
    _view->show();
    connect(_view->getGrid(), SIGNAL(notifyCoordinatesClicked(int,int)), _model, SLOT(registerPlayAt(int,int)));
    connect(_model, SIGNAL(notifyCount(int,int,int,int,int,int)), _view->getGrid(), SLOT(registerCount(int,int,int,int,int,int)));
    connect(_model, SIGNAL(notifyPositionIsValid(int,int,bool)), _view->getGrid(), SLOT(registerPositionIsValid(int,int,bool)));
    _undoStack = new QUndoStack(this);
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

