#ifndef MOVEGEN_H_INCLUDED
#define MOVEGEN_H_INCLUDED

#include <string.h>
#include "../constsAndEnums.h"
#include "bitboard.h"


int isSquareAttacked(struct BoardState board, int square, int side);
int inCheck(struct BoardState board);
void moveGenerator(struct BoardState board, int moveArray[]);
void decodeMove(int chessMove);
int encodeMove(int type, int side, int piece, int fromSquare, int toSquare);
void copyBoardState(struct BoardState board,struct BoardState *boardCopy);
int makeMove(struct BoardState *boardPtr, int chessMove);
U64 getLegalMovesCount(struct BoardState *board, int moveList[]);

#endif // MOVEGEN_H_INCLUDED
