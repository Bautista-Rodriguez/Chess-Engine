#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "constsAndEnums.h"

int negamax(struct BoardState board, int alpha, int beta, int depth, int *bestMove);
int nodes;
int searchMove();
#endif // SEARCH_H_INCLUDED
