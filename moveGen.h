#ifndef MOVEGEN_H_INCLUDED
#define MOVEGEN_H_INCLUDED

#include "constsAndEnums.h"

int isSquareAttacked(struct BoardState board, int square, int side);
void moveGenerator(struct BoardState board, int side);
void decodeMove(int move);

#endif // MOVEGEN_H_INCLUDED
