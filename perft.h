#ifndef PERFT_H_INCLUDED
#define PERFT_H_INCLUDED

#include "constsAndEnums.h"
#include "boardRepresentation.h"
#include "attacks.h"
#include "bitboard.h"
#include "moveGen.h"
#include "perft.h"
#include "makeUnmake.h"

U64 perftPosition(struct BoardState *board, int depth);
void perftTest();
void perft_driver(int depth);

#endif // PERFT_H_INCLUDED
