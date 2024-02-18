#include "boardRepresentation.h"

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

void mapBoard(struct BoardState board, int boardArray[8][8])
{
    for(int rank=0;rank<8;rank++)
    {
        for(int file=0;file<8;file++)
        {
            int square = rank*8 + file;
            for(int bitBoardPiece=0;bitBoardPiece<12;bitBoardPiece++)
            {
                if(board.bitboards[bitBoardPiece] & (1ULL << square))
                    boardArray[rank][file]=bitBoardPiece;
            }
        }
    }
    return;
}

void printBoard(struct BoardState board, int boardArray[8][8])
{
    printf("\n   |---|---|---|---|---|---|---|---|\n");
    for(int rank=7;rank>=0;rank--)
    {
        for(int file=0;file<8;file++)
        {
            if(!file)
                printf(" %d", rank + 1);
            int piece=boardArray[rank][file];
            printf(" | %c", (piece == 32) ? ' ' : ascii[piece]);
        }
        printf(" |\n   |---|---|---|---|---|---|---|---|\n");
    }
    printf("     A   B   C   D   E   F   G   H\n\n");
    char castleRights[5];
    castleRights[0] = (board.castle & (1ULL)) ? 'K' : '.';
    castleRights[1] = (board.castle & (1ULL << 1)) ? 'Q' : '.';
    castleRights[2] = (board.castle & (1ULL << 2)) ? 'k' : '.';
    castleRights[3] = (board.castle & (1ULL << 3)) ? 'q' : '.';
    castleRights[4]='\0';
    printf("Castle Rights: %s\n", castleRights);
    printf("Side To Move: %s\n", (!board.sideToMove) ? "White" : "Black");
    printf("En Passant: %i\n", board.enPassant);
    printf("Half Moves: %i\n", board.halfMoveCount);
    printf("Full Moves: %i\n", board.fullMoveCount);
    return;
}

void parseFEN(char *stringFEN, struct BoardState *board)
{
    int currentRank = 7;
    int currentFile = 0;
    int outOfPosition = 0;
    board->bitboards[0]=0ULL;
    board->bitboards[1]=0ULL;
    board->bitboards[2]=0ULL;
    board->bitboards[3]=0ULL;
    board->bitboards[4]=0ULL;
    board->bitboards[5]=0ULL;
    board->bitboards[6]=0ULL;
    board->bitboards[7]=0ULL;
    board->bitboards[8]=0ULL;
    board->bitboards[9]=0ULL;
    board->bitboards[10]=0ULL;
    board->bitboards[11]=0ULL;
    board->occupancies[0]=0ULL;
    board->occupancies[1]=0ULL;
    board->occupancies[2]=0ULL;
    board->enPassant = 65;
    board->fullMoveCount = -1;
    board->halfMoveCount = -1;
    board->sideToMove = -1;
    board->castle = 0;
    for(int i = 0;stringFEN[i] != '\0';i++)
    {
        char c = stringFEN[i];
        switch(c)
        {
        case 'P':
            board->bitboards[0] |= (1ULL << (currentRank * 8 + currentFile));
            currentFile++;
            break;
        case 'N':
            board->bitboards[1] |= (1ULL << (currentRank * 8 + currentFile));
            currentFile++;
            break;
        case 'B':
            board->bitboards[2] |= (1ULL << (currentRank * 8 + currentFile));
            currentFile++;
            break;
        case 'R':
            board->bitboards[3] |= (1ULL << (currentRank * 8 + currentFile));
            currentFile++;
            break;
        case 'Q':
            if(outOfPosition)
            {
                board->castle += 2;
            }
            else
            {
                board->bitboards[4] |= (1ULL << (currentRank * 8 + currentFile));
                currentFile++;
            }
            break;
        case 'K':
            if(outOfPosition)
            {
                board->castle += 1;
            }
            else
            {
                board->bitboards[5] |= (1ULL << (currentRank * 8 + currentFile));
                currentFile++;
            }
            break;
        case 'p':
            board->bitboards[6] |= (1ULL << (currentRank * 8 + currentFile));
            currentFile++;
            break;
        case 'n':
            board->bitboards[7] |= (1ULL << (currentRank * 8 + currentFile));
            currentFile++;
            break;
        case 'b':
            if(outOfPosition == 3)
                board->enPassant = (stringFEN[i+1] - 49) * 8 + (c - 97);
            else if(outOfPosition == 1)
                board -> sideToMove = 1;
            else
            {
                board->bitboards[8] |= (1ULL << (currentRank * 8 + currentFile));
                currentFile++;
            }
            break;
        case 'r':
            board->bitboards[9] |= (1ULL << (currentRank * 8 + currentFile));
            currentFile++;
            break;
        case 'q':
            if(outOfPosition)
            {
                board->castle += 8;
            }
            else
            {
                board->bitboards[10] |= (1ULL << (currentRank * 8 + currentFile));
                currentFile++;
            }
            break;
        case 'k':
            if(outOfPosition)
            {
                board->castle += 4;
            }
            else
            {
                board->bitboards[11] |= (1ULL << (currentRank * 8 + currentFile));
                currentFile++;
            }
            break;
        case 'w':
            board->sideToMove = 0;
            break;
        case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
            if(outOfPosition == 5 && board->fullMoveCount == -1)
            {
                int moveNum = 0;
                for(int j = 0;stringFEN[i+j] != '\0';j++)
                {
                    moveNum = moveNum * 10;
                    moveNum += stringFEN[i+j] - 48;
                }
                board->fullMoveCount = moveNum;
            }
            else if(outOfPosition == 4 && board->halfMoveCount == -1)
            {
                int moveNum = 0;
                for(int j = 0;stringFEN[i+j] != ' ';j++)
                {
                    moveNum *= 10;
                    moveNum += stringFEN[i+j] - 48;
                }
                board->halfMoveCount = moveNum;
            }
            else
                currentFile += c - 48;
            break;
        case '/':
            currentFile = 0;
            currentRank--;
            break;
        case 'a':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':
            board->enPassant = (stringFEN[i+1] - 49) * 8 + (c - 97);
            break;
        case ' ':
            outOfPosition += 1;
        default:
            break;
        }
    }
    board->occupancies[0] =
        board->bitboards[0] | board->bitboards[1] | board->bitboards[2] |
        board->bitboards[3] | board->bitboards[4] | board->bitboards[5];
    board->occupancies[1] =
        board->bitboards[6] | board->bitboards[7] | board->bitboards[8] |
        board->bitboards[9] | board->bitboards[10] | board->bitboards[11];
    board->occupancies[2] = board->occupancies[0] | board->occupancies[1];
    return;
}
