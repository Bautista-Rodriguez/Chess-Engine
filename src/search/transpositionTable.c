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

void initHashTable()
{
    initKeys();
    hashTable = (HashT*) malloc(hashSize * sizeof(HashT));
    clearHashTable();
    return;
}

void clearHashTable()
{
    memset(hashTable,0,sizeof(HashT)*hashSize);
    /*
    for(U64 i = 0ULL;i < hashSize;i++)
    {
        hashTable[i].key = 0ULL;
        hashTable[i].depth = 0;
        hashTable[i].flags = 0;
        hashTable[i].score = 0;
    }
    */
    return;
}

void freeHashTable()
{
    free(hashTable);
    return;
}

int hashTableRead(int alpha,int beta,int depth, U64 key)
{
    HashT *hashEntry = &hashTable[key % hashSize];

    if(hashEntry->key == key)
    {
        if(hashEntry->depth >= depth)
        {
            if(hashEntry->score < -48000)
                hashEntry->score += ply;
            if(hashEntry->score > 48000)
                hashEntry->score -= ply;
            if(hashEntry->flags == exactHF)
                return hashEntry->score;
            if ((hashEntry->flags == alphaHF) && (hashEntry->score <= alpha))
                return alpha;
            if((hashEntry->flags == betaHF) && (hashEntry->score >= beta))
                return beta;
        }
    }
    return 300000;
}

void hashTableWrite(int score,int depth,int hashFlag,U64 key)
{
    HashT *hashEntry = &hashTable[key % hashSize];
    if(score < -48000)
        score -= ply;
    if(score > 48000)
        score += ply;
    hashEntry->key = key;
    hashEntry->score = score;
    hashEntry->flags = hashFlag;
    hashEntry->depth = depth;
    return;
}

int isRepetition(U64 hashKey)
{
    for(int i = 0;i < repetitionIndex;i++)
    {
        if(repetitionTable[repetitionIndex] == hashKey)
            return 1;
    }
    return 0;
}
