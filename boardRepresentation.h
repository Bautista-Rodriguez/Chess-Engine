#ifndef BOARDREPRESENTATION_H_INCLUDED
#define BOARDREPRESENTATION_H_INCLUDED


typedef unsigned long long  U64;

typedef unsigned long long  Bitboard;

#define whitePawns bitboard[0]
#define whiteKnights bitboard[1]
#define whiteBishops bitboard[2]
#define whiteRooks bitboard[3]
#define whiteQueens bitboard[4]
#define whiteKing bitboard[5]

#define blackPawns bitboard[6]
#define blackKnights bitboard[7]
#define blackBishops bitboard[8]
#define blackRooks bitboard[9]
#define blackQueens bitboard[10]
#define blackKing bitboard[11]

U64 bitboard[12]=
{
    //WHITE
    0xff00ULL,
    0x42ULL,
    0x24ULL,
    0x81ULL,
    0x8ULL,
    0x10ULL,
    //BLACK
    0xff000000000000ULL,
    0x4200000000000000ULL,
    0x2400000000000000ULL,
    0x8100000000000000ULL,
    0x800000000000000ULL,
    0x1000000000000000ULL
};

short int board[8][8]=
{
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32,
    32,32,32,32,32,32,32,32
};

char ascii[]={'P','N','B','R','Q','K','p','n','b','r','q','k'};

void printBoard();

void printBoard()
{
    for(int rank=0;rank<8;rank++)
    {
        for(int file=0;file<8;file++)
        {
            int square = rank*8 + file;
            for(int bitBoardPiece=0;bitBoardPiece<12;bitBoardPiece++)
            {
                if(bitboard[bitBoardPiece] & (1ULL << square))
                   board[rank][file]=bitBoardPiece;
            }
        }
    }
    printf("\n   |---|---|---|---|---|---|---|---|\n");
    for(int rank=7;rank>=0;rank--)
    {
        for(int file=0;file<8;file++)
        {
            if(!file)
                printf(" %d", rank + 1);
            int piece=board[rank][file];
            printf(" | %c", (piece == 32) ? ' ' : ascii[piece]);
        }
        printf(" |\n   |---|---|---|---|---|---|---|---|\n");
    }
    printf("     A   B   C   D   E   F   G   H\n");
    return;
}

void printBitboard(U64 bitboard)
{
    for(int rank=7;rank>=0;rank--)
    {
        for(int file=0;file<8;file++)
        {
            if(bitboard & 1ULL<<(file+rank*8))
                printf("o");
            else
                printf(".");
        }
        printf("\n");
    }
    return;
}


#endif // BOARDREPRESENTATION_H_INCLUDED
