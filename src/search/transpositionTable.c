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

unsigned int randomState = 1804289383;

unsigned int getRandom32bits()
{
    unsigned int number = randomState;

    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    randomState = number;

    return number;
}

U64 getRandom64bits()
{
    U64 n1, n2, n3, n4;

    n1 = (U64)(getRandom32bits()) & 0xFFFF;
    n2 = (U64)(getRandom32bits()) & 0xFFFF;
    n3 = (U64)(getRandom32bits()) & 0xFFFF;
    n4 = (U64)(getRandom32bits()) & 0xFFFF;

    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}
