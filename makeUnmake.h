#ifndef MAKEUNMAKE_H_INCLUDED
#define MAKEUNMAKE_H_INCLUDED

#include "constsAndEnums.h"
#include "attacks.h"
#include "moveGen.h"
#include "string.h"

void copyBoardState(struct BoardState board,struct BoardState *boardCopy);
int makeMove(struct BoardState *boardPtr, int chessMove);
U64 getLegalMovesCount(struct BoardState *board, int moveList[]);

#endif // MAKEUNMAKE_H_INCLUDED
