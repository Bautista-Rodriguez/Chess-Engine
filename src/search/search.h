#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include <string.h>
#include "../constsAndEnums.h"
#include "transpositionTable.h"
#include "../board/bitboard.h"
#include "../board/moveGen.h"

void searchMove(struct BoardState board, int depth);
int negamax(struct BoardState board, int alpha, int beta, int depth);
int quietSearch(struct BoardState board, int alpha, int beta);

int sortMoves(struct BoardState board, int moveList[]);
int scoreMove(struct BoardState board, int chessMove);

int evaluate(struct BoardState board);
#endif // SEARCH_H_INCLUDED
