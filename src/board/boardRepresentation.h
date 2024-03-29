#ifndef BOARDREPRESENTATION_H_INCLUDED
#define BOARDREPRESENTATION_H_INCLUDED

#include "../constsAndEnums.h"
#include "../search/transpositionTable.h"

void mapBoard(struct BoardState board, int boardArray[8][8]);
void printBoard(struct BoardState board);
void printBitboard();
void parseFEN(char *, struct BoardState *);

#endif // BOARDREPRESENTATION_H_INCLUDED
