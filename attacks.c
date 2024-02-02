#include "boardRepresentation.h"
#include "attacks.h"

const U64 notAFile = 0xFEFEFEFEFEFEFEFEULL;
const U64 notHFile = 0x7F7F7F7F7F7F7F7FULL;
const U64 notHGFile = 0x3F3F3F3F3F3F3F3FULL;
const U64 notABFile = 0xFCFCFCFCFCFCFCFCULL;

const U64 borders = 0xFF818181818181FFULL;
const U64 notBorders = ~borders;

const U64 AFile = 0x0101010101010101ULL;
const U64 BFile = AFile << 1;
const U64 CFile = AFile << 2;
const U64 DFile = AFile << 3;
const U64 EFile = AFile << 4;
const U64 FFile = AFile << 5;
const U64 GFile = AFile << 6;
const U64 HFile = AFile << 7;

const U64 Rank1 = 0xFFULL;
const U64 Rank2 = Rank1 << 8;
const U64 Rank3 = Rank1 << 8*2;
const U64 Rank4 = Rank1 << 8*3;
const U64 Rank5 = Rank1 << 8*4;
const U64 Rank6 = Rank1 << 8*5;
const U64 Rank7 = Rank1 << 8*6;
const U64 Rank8 = Rank1 << 8*7;

const U64 A1Diagonal = 0x8040201008040201ULL;
const U64 H1Diagonal = 0x0102040810204080ULL;

U64 maskPawnAttacks(int side, int square)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);
    if(!side)
    {
        if((bitboard << 7) & notHFile)
            attacks |= (bitboard << 7);
        if((bitboard << 9) & notAFile)
            attacks |= (bitboard << 9);
    }
    else
    {
        if((bitboard >> 7) & notAFile)
            attacks |= (bitboard >> 7);
        if((bitboard >> 9) & notHFile)
            attacks |= (bitboard >> 9);
    }
    return attacks;
}

void initMaskPawnAttacks()
{
    for(int square=0;square<64;square++)
    {
        pawnAttacks[0][square]=maskPawnAttacks(0,square);
        pawnAttacks[1][square]=maskPawnAttacks(1,square);
    }
}

U64 maskKnightAttacks(int square)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    if((bitboard << 6) & notHGFile)
        attacks |= (bitboard << 6);
    if((bitboard << 15) & notHFile)
        attacks |= (bitboard << 15);
    if((bitboard << 10) & notABFile)
        attacks |= (bitboard << 10);
    if((bitboard << 17) & notAFile)
        attacks |= (bitboard << 17);

    if((bitboard >> 6) & notABFile)
        attacks |= (bitboard >> 6);
    if((bitboard >> 15) & notAFile)
        attacks |= (bitboard >> 15);
    if((bitboard >> 10) & notHGFile)
        attacks |= (bitboard >> 10);
    if((bitboard >> 17) & notHFile)
        attacks |= (bitboard >> 17);
    return attacks;
}

void initMaskKnightAttacks()
{
    for(int square=0;square<64;square++)
    {
        knightAttacks[square]=maskKnightAttacks(square);
    }
}

U64 maskKingAttacks(int square)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    if((bitboard << 7) & notHFile)
        attacks |= (bitboard << 7);
    if((bitboard << 9) & notAFile)
        attacks |= (bitboard << 9);
    if((bitboard >> 7) & notAFile)
        attacks |= (bitboard >> 7);
    if((bitboard >> 9) & notHFile)
        attacks |= (bitboard >> 9);
    if((bitboard << 1) & notAFile)
        attacks |= (bitboard << 1);
    if((bitboard >> 1) & notHFile)
        attacks |= (bitboard >> 1);
    attacks |= (bitboard << 8);
    attacks |= (bitboard >> 8);

    return attacks;
}

void initMaskKingAttacks()
{
    for(int square=0;square<64;square++)
    {
        kingAttacks[square]=maskKingAttacks(square);
    }
}

U64 maskRookRay(int square)
{
    U64 ray = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    int file = square%8;
    int rank = square/8;

    for(int i = 0;i < 7 - rank; i++)
    {
        ray |= (bitboard << (8 * i));
    }
    for(int i = 0;i < file; i++)
    {
        ray |= (bitboard >> (1 * i));
    }
    for(int i = 0;i < rank; i++)
    {
        ray |= (bitboard >> (8 * i));
    }
    for(int i = 0;i < 7 - file; i++)
    {
        ray |= (bitboard << (1 * i));
    }

    ray &= ~bitboard;

    return ray;
}

void initMaskRookRay()
{
    for(int square=0;square<64;square++)
    {
        rookRay[square]=maskRookRay(square);
    }
}

U64 maskBishopRay(int square)
{
    U64 ray = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    int file = square%8;
    int rank = square/8;

    for(int i = 0;i < 8 - file; i++)
    {
        ray |= (bitboard << (9 * i));
    }
    for(int i = 0;i < file + 1; i++)
    {
        ray |= (bitboard << (7 * i));
    }
    for(int i = 0;i < file + 1; i++)
    {
        ray |= (bitboard >> (9 * i));
    }
    for(int i = 0;i < 8 - file; i++)
    {
        ray |= (bitboard >> (7 * i));
    }
    ray &= ~AFile;
    ray &= ~HFile;
    ray &= ~Rank1;
    ray &= ~Rank8;
    ray &= ~bitboard;

    return ray;
}

void initMaskBishopRay()
{
    for(int square=0;square<64;square++)
    {
        bishopRay[square]=maskBishopRay(square);
    }
}

U64 maskQueenRay(int square)
{
    U64 ray = 0ULL;

    ray |= maskBishopRay(square);
    ray |= maskRookRay(square);

    return ray;
}

void initMaskQueenRay()
{
    for(int square=0;square<64;square++)
    {
        queenRay[square]=maskQueenRay(square);
    }
}


U64 maskRookAttacks(int square, U64 position)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    int file = square%8;
    int rank = square/8;

    for(int i = 0;i < 8 - rank; i++)
    {
        attacks |= (bitboard << (8 * i));
        if((bitboard << (8 * i)) & position) break;
    }
    for(int i = 0;i < file + 1; i++)
    {
        attacks |= (bitboard >> (1 * i));
        if((bitboard >> (1 * i)) & position) break;
    }
    for(int i = 0;i < rank + 1; i++)
    {
        attacks |= (bitboard >> (8 * i));
        if((bitboard >> (8 * i)) & position) break;
    }
    for(int i = 0;i < 8 - file; i++)
    {
        attacks |= (bitboard << (1 * i));
        if((bitboard << (1 * i)) & position) break;
    }

    attacks &= ~bitboard;

    return attacks;
}

U64 maskBishopAttacks(int square, U64 position)
{
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;
    bitboard |= (1ULL << square);

    int file = square%8;
    int rank = square/8;
    for(int i = 0;i < 8 - file; i++)
    {
        attacks |= (bitboard << (9 * i));
        if((bitboard << (9 * i)) & position) break;
    }
    for(int i = 0;i < file + 1; i++)
    {
        attacks |= (bitboard << (7 * i));
        if((bitboard << (7 * i)) & position) break;
    }
    for(int i = 0;i < file + 1; i++)
    {
        attacks |= (bitboard >> (9 * i));
        if((bitboard >> (9 * i)) & position) break;
    }
    for(int i = 0;i < 8 - file; i++)
    {
        attacks |= (bitboard >> (7 * i));
        if((bitboard >> (7 * i)) & position) break;
    }
    attacks &= ~bitboard;

    return attacks;
}

U64 maskQueenAttacks(int square, U64 position)
{
    U64 attacks = 0ULL;
    attacks |= maskBishopAttacks(square, position);
    attacks |= maskRookAttacks(square, position);
    return attacks;
}

U64 occupancyRookAttacks(int square, int number)
{
    U64 rookRay = maskRookRay(square);
    int numberOfBits = popCount(rookRay);
    U64 occupancy = 0ULL;
    for(int i = 0;i < numberOfBits;i++)
    {
        int index = bitScan(rookRay);
        occupancy |= ((number >> i) & 1ULL) << index;
        rookRay &= rookRay - 1;
    }
    return occupancy;
}

U64 occupancyRook[64][4096];

U64 occupancyBishopAttacks(int square, int number)
{
    U64 bishopRay = maskBishopRay(square);
    int numberOfBits = popCount(bishopRay);
    U64 occupancy = 0ULL;
    for(int i = 0;i < numberOfBits;i++)
    {
        int index = bitScan(bishopRay);
        occupancy |= ((number >> i) & 1ULL) << index;
        bishopRay &= bishopRay - 1;
    }
    return occupancy;
}

U64 occupancyBishop[64][512];

rookShifts[64] = { 52, 52, 52, 52, 52, 52, 52, 52,
                53, 53, 53, 54, 53, 53, 54, 53,
                53, 54, 54, 54, 53, 53, 54, 53,
                53, 54, 53, 53, 54, 54, 54, 53,
                52, 54, 53, 53, 53, 53, 54, 53,
                52, 53, 54, 54, 53, 53, 54, 53,
                53, 54, 54, 54, 53, 53, 54, 53,
                52, 53, 53, 53, 53, 53, 53, 52 };

bishopShifts[64] = { 58, 60, 59, 59, 59, 59, 60, 58,
                   60, 59, 59, 59, 59, 59, 59, 60,
                    59, 59, 57, 57, 57, 57, 59, 59,
                    59, 59, 57, 55, 55, 57, 59, 59,
                    59, 59, 57, 55, 55, 57, 59, 59,
                    59, 59, 57, 57, 57, 57, 59, 59,
                    60, 60, 59, 59, 59, 59, 60, 60,
                    58, 60, 59, 59, 59, 59, 59, 58 };

U64 rookMagics[64] = {
    468374916371625120,   18428729537625841661,   2531023729696186408,    6093370314119450896,    13830552789156493815,   16134110446239088507,   12677615322350354425,   5404321144167858432,
    2111097758984580,     18428720740584907710,   17293734603602787839,   4938760079889530922,    7699325603589095390,    9078693890218258431,    578149610753690728,     9496543503900033792,
    1155209038552629657,  9224076274589515780,    1835781998207181184,    509120063316431138,     16634043024132535807,   18446673631917146111,   9623686630121410312,    4648737361302392899,
    738591182849868645,   1732936432546219272,    2400543327507449856,    5188164365601475096,    10414575345181196316,   1162492212166789136,    9396848738060210946,    622413200109881612,
    7998357718131801918,  7719627227008073923,    16181433497662382080,   18441958655457754079,   1267153596645440,       18446726464209379263,   1214021438038606600,    4650128814733526084,
    9656144899867951104,  18444421868610287615,   3695311799139303489,    10597006226145476632,   18436046904206950398,   18446726472933277663,   3458977943764860944,    39125045590687766,
    9227453435446560384,  6476955465732358656,    1270314852531077632,    2882448553461416064,    11547238928203796481,   1856618300822323264,    2573991788166144,       4936544992551831040,
    13690941749405253631, 15852669863439351807,   18302628748190527413,   12682135449552027479,   13830554446930287982,   18302628782487371519,   7924083509981736956,    4734295326018586370 };

bishopMagics[64] = {
    16509839532542417919, 14391803910955204223,   1848771770702627364,    347925068195328958,     5189277761285652493,    3750937732777063343,    18429848470517967340,   17870072066711748607,
    16715520087474960373, 2459353627279607168,    7061705824611107232,    8089129053103260512,    7414579821471224013,    9520647030890121554,    17142940634164625405,   9187037984654475102,
    4933695867036173873,  3035992416931960321,    15052160563071165696,   5876081268917084809,    1153484746652717320,    6365855841584713735,    2463646859659644933,    1453259901463176960,
    9808859429721908488,  2829141021535244552,    576619101540319252,     5804014844877275314,    4774660099383771136,    328785038479458864,     2360590652863023124,    569550314443282,
    17563974527758635567, 11698101887533589556,   5764964460729992192,    6953579832080335136,    1318441160687747328,    8090717009753444376,    16751172641200572929,   5558033503209157252,
    17100156536247493656, 7899286223048400564,    4845135427956654145,    2368485888099072,       2399033289953272320,    6976678428284034058,    3134241565013966284,    8661609558376259840,
    17275805361393991679, 15391050065516657151,   11529206229534274423,   9876416274250600448,    16432792402597134585,   11975705497012863580,   11457135419348969979,   9763749252098620046,
    16960553411078512574, 15563877356819111679,   14994736884583272463,   9441297368950544394,    14537646123432199168,   9888547162215157388,    18140215579194907366,   18374682062228545019 };

