#ifndef PLAYCOMMAND_H
#define PLAYCOMMAND_H

#include <QUndoCommand>
#include "ModelTakuzu.h"

class PlayCommand : public QUndoCommand
{
public:
    PlayCommand(ModelTakuzu *m, int i, int j, Pawn p);
    ~PlayCommand();
    void undo() override;
    void redo() override;

private:
    ModelTakuzu *_model;
    int _row;
    int _col;
    Pawn _pawn;
    int _nbUndo;
};

#endif // PLAYCOMMAND_H
