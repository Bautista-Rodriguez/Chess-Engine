#ifndef TRANSPOSITIONTABLE_H_INCLUDED
#define TRANSPOSITIONTABLE_H_INCLUDED

#include "../constsAndEnums.h"
#include "../board/bitboard.h"

void initKeys();
U64 generateHashKey(struct BoardState board);
unsigned int getRandomBits();
U64 get64Bits();
void initHashTable();
void freeHashTable();
void clearHashTable();
int hashTableRead(int alpha,int beta,int depth,U64 key);
void hashTableWrite(int score,int depth,int hashFlag,U64 key);

#endif // TRANSPOSITIONTABLE_H_INCLUDED
