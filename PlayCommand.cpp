#include "PlayCommand.h"
#include <assert.h>

PlayCommand::PlayCommand(ModelTakuzu *m, int i, int j, Pawn p)
{
    _model = m;
    assert(i >= 0 && "Row index should be >= 0");
    assert(j >= 0 && "Col index should be >= 0");
    _row = i;
    _col = j;
    _pawn = p;
}

PlayCommand::~PlayCommand()
{

}

void PlayCommand::undo()
{
    Pawn oldPawn = ModelTakuzu::permuteL(_pawn);
    _model->playAt(_row, _col, oldPawn);
}

void PlayCommand::redo()
{
    _model->playAt(_row, _col, _pawn);
}
