#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "../board/bitboard.h"
#include "../constsAndEnums.h"
#include "../board/moveGen.h"

int negamax(struct BoardState board, int alpha, int beta, int depth, int *bestMove);
void searchMove(struct BoardState board, int depth);
#endif // SEARCH_H_INCLUDED
