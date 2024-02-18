#ifndef PERFT_H_INCLUDED
#define PERFT_H_INCLUDED

#include "../constsAndEnums.h"
#include "boardRepresentation.h"
#include "bitboard.h"
#include "moveGen.h"

U64 perftPosition(struct BoardState *board, int depth);
void perftTest();

#endif // PERFT_H_INCLUDED
