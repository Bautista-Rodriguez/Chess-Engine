#ifndef TRANSPOSITIONTABLE_H_INCLUDED
#define TRANSPOSITIONTABLE_H_INCLUDED

#include "../constsAndEnums.h"
#include "../board/bitboard.h"

void initKeys();
U64 generateHashKey(struct BoardState board);
unsigned int getRandomBits();
U64 get64Bits();

#endif // TRANSPOSITIONTABLE_H_INCLUDED
