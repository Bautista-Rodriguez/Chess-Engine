#include "transpositionTable.h"

unsigned int randomState = 1804289383;

void initKeys()
{
    for (int j = 0;j < 64;j++)
    {
        for (int i = 0;i < 12;i++)
            pieceKeys[i][j] = get64Bits();
    }
    for (int i = 0;i < 8;i++)
        enPassantKeys[i] = get64Bits();
    for (int i = 0;i < 16;i++)
        castleKeys[i] = get64Bits();
    sideKey = get64Bits();
    return;
}

U64 generateHashKey(struct BoardState board)
{
    U64 bitboard =0ULL, finalKey = 0ULL;
    for(int i = 0;i < 12;i++)
    {
        bitboard = board.bitboards[i];
        while(bitboard)
        {
            int square = bitScan(bitboard);
            finalKey ^= pieceKeys[i][square];
            bitboard = (U64) popLSB(bitboard);
        }
    }
    if(board.enPassant < 64)
    {
        int enPassantFile = (board.enPassant % 8);
        finalKey ^= enPassantKeys[enPassantFile];
    }
    finalKey ^= castleKeys[board.castle];
    if(board.sideToMove == 1)
        finalKey ^= sideKey;
    return finalKey;
}

unsigned int getRandomBits()
{
    unsigned int number = randomState;

    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    randomState = number;
    return number;
}


U64 get64Bits()
{
    U64 n1 = 0ULL, n2 = 0ULL, n3 = 0ULL, n4 = 0ULL;

    n1 = ((U64)(getRandomBits()) & 0xFFFF);
    n2 = ((U64)(getRandomBits()) & 0xFFFF);
    n3 = ((U64)(getRandomBits()) & 0xFFFF);
    n4 = ((U64)(getRandomBits()) & 0xFFFF);

    return (n1 | (n2 << 16) | (n3 << 32) | (n4 << 48));
}
