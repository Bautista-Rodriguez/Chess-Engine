#include "transpositionTable.h"

U64 pieceKeys[12][64];
U64 enPassantKeys[8];
U64 castleKeys[16];
U64 sideKey;

void initKeys()
{

    return;
}

generateHashKey(struct BoardState board)
{
    U64 bitboard, finalKey = 0ULL;
    for(int i = 0;i < 12;i++)
    {
        bitboard = board.bitboards[i];
        while(bitboard)
        {
            int square = bitScan(bitboard);
            finalKey ^= pieceKeys[i][square];
            bitboard = popLSB(bitboard);
        }
    }
    if(board.enPassant != 65)
    {
        int enPassantFile = (board.enPassant % 8);
        finalKey ^= enPassantKeys[enPassantFile];
    }
    if(board.sideToMove == 1)
        finalKey ^= sideKey;
    return;
}
