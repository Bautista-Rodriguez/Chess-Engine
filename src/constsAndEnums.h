#ifndef CONSTSANDENUMS_H_INCLUDED
#define CONSTSANDENUMS_H_INCLUDED

typedef unsigned long long  U64;


enum square
{
    a1,b1,c1,d1,e1,f1,g1,h1,
    a2,b2,c2,d2,e2,f2,g2,h2,
    a3,b3,c3,d3,e3,f3,g3,h3,
    a4,b4,c4,d4,e4,f4,g4,h4,
    a5,b5,c5,d5,e5,f5,g5,h5,
    a6,b6,c6,d6,e6,f6,g6,h6,
    a7,b7,c7,d7,e7,f7,g7,h7,
    a8,b8,c8,d8,e8,f8,g8,h8, noNum, noNum2
};

enum moveType
{
    quietMove = 0b0000,
    doublePush = 0b0001,
    castleKing = 0b0010,
    castleQueen = 0b0011,
    capture = 0b0100,
    enPassant = 0b0101,
    promotion = 0b1000,
    knightProm = 0b1000,
    bishopProm = 0b1001,
    rookProm = 0b1010,
    queenProm = 0b1011,
    knightPromCap = 0b1100,
    bishopPromCap = 0b1101,
    rookPromCap = 0b1110,
    queenPromCap = 0b1111
};

static const char typeMoveChar[16][16]=
{
    "quiet",
    "double push",
    "castle king",
    "castle queen",
    "capture",
    "en passant",
    "",
    "",
    "knight prom",
    "bishop prom",
    "rook prom",
    "queen prom",
    "knight prom cap",
    "bishop prom cap",
    "rook prom cap",
    "queen prom cap"
};

static const char squareChar[66][3] =
{
    "a1","b1","c1","d1","e1","f1","g1","h1",
    "a2","b2","c2","d2","e2","f2","g2","h2",
    "a3","b3","c3","d3","e3","f3","g3","h3",
    "a4","b4","c4","d4","e4","f4","g4","h4",
    "a5","b5","c5","d5","e5","f5","g5","h5",
    "a6","b6","c6","d6","e6","f6","g6","h6",
    "a7","b7","c7","d7","e7","f7","g7","h7",
    "a8","b8","c8","d8","e8","f8","g8","h8", "no", "no"
};

enum color
{
    white,black
};

enum castle
{
    KC = 1, QC = 2, kc = 4, qc = 8
};

enum pieces
{
    P, N, B, R, Q, K, p, n, b, r, q, k
};

U64 pawnAttacks[2][64];
U64 knightAttacks[64];
U64 kingAttacks[64];
U64 rookRay[64];
U64 bishopRay[64];
U64 queenRay[64];

struct BoardState
{
    U64 bitboards[12];
    U64 occupancies[3];
    int sideToMove;
    int castle;
    int enPassant;
    int halfMoveCount;
    int fullMoveCount;
};

U64 bishopOccupancy[64][512];
U64 rookOccupancy[64][4096];

static const char ascii[]={'P','N','B','R','Q','K','p','n','b','r','q','k'};
static const char startingPosition[]={"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

static const U64 notFileA = 0xFEFEFEFEFEFEFEFEULL;
static const U64 notFileH = 0x7F7F7F7F7F7F7F7FULL;
static const U64 notFileGH = 0x3F3F3F3F3F3F3F3FULL;
static const U64 notFileAB = 0xFCFCFCFCFCFCFCFCULL;

static const U64 fileA = 0x0101010101010101ULL;
static const U64 fileB = fileA << 1;
static const U64 fileC = fileA << 2;
static const U64 fileD = fileA << 3;
static const U64 fileE = fileA << 4;
static const U64 fileF = fileA << 5;
static const U64 fileG = fileA << 6;
static const U64 fileH = fileA << 7;

static const U64 rank1 = 0xFFULL;
static const U64 rank2 = rank1 << 8;
static const U64 rank3 = rank1 << 8*2;
static const U64 rank4 = rank1 << 8*3;
static const U64 rank5 = rank1 << 8*4;
static const U64 rank6 = rank1 << 8*5;
static const U64 rank7 = rank1 << 8*6;
static const U64 rank8 = rank1 << 8*7;

static const int castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

//SEARCH HEURISTICS CONSTANTS AND VARIABLES

static const int mvvLva[12][12] = {
 	{105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605},
	{104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604},
	{103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603},
	{102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602},
	{101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601},
	{100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600},
	{105, 205, 305, 405, 505, 605, 105, 205, 305, 405, 505, 605},
	{104, 204, 304, 404, 504, 604, 104, 204, 304, 404, 504, 604},
	{103, 203, 303, 403, 503, 603, 103, 203, 303, 403, 503, 603},
	{102, 202, 302, 402, 502, 602, 102, 202, 302, 402, 502, 602},
	{101, 201, 301, 401, 501, 601, 101, 201, 301, 401, 501, 601},
	{100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600}
};

U64 pieceKeys[12][64];
U64 enPassantKeys[8];
U64 castleKeys[16];
U64 sideKey;

//MAGIC BITBOARDS CONSTANTS

static const int rookShifts[64] = { 52, 52, 52, 52, 52, 52, 52, 52,
                                   53, 53, 53, 54, 53, 53, 54, 53,
                                    53, 54, 54, 54, 53, 53, 54, 53,
                                    53, 54, 53, 53, 54, 54, 54, 53,
                                    52, 54, 53, 53, 53, 53, 54, 53,
                                    52, 53, 54, 54, 53, 53, 54, 53,
                                    53, 54, 54, 54, 53, 53, 54, 53,
                                    52, 53, 53, 53, 53, 53, 53, 52 };

static const int bishopShifts[64] = { 58, 60, 59, 59, 59, 59, 60, 58,
                                      60, 59, 59, 59, 59, 59, 59, 60,
                                      59, 59, 57, 57, 57, 57, 59, 59,
                                      59, 59, 57, 55, 55, 57, 59, 59,
                                      59, 59, 57, 55, 55, 57, 59, 59,
                                      59, 59, 57, 57, 57, 57, 59, 59,
                                      60, 60, 59, 59, 59, 59, 60, 60,
                                      58, 60, 59, 59, 59, 59, 59, 58 };

static const U64 rookMagicNumbers[64] = {
    468374916371625120ULL,   18428729537625841661ULL,   2531023729696186408ULL,    6093370314119450896ULL,    13830552789156493815ULL,   16134110446239088507ULL,   12677615322350354425ULL,   5404321144167858432ULL,
    2111097758984580ULL,     18428720740584907710ULL,   17293734603602787839ULL,   4938760079889530922ULL,    7699325603589095390ULL,    9078693890218258431ULL,    578149610753690728ULL,     9496543503900033792ULL,
    1155209038552629657ULL,  9224076274589515780ULL,    1835781998207181184ULL,    509120063316431138ULL,     16634043024132535807ULL,   18446673631917146111ULL,   9623686630121410312ULL,    4648737361302392899ULL,
    738591182849868645ULL,   1732936432546219272ULL,    2400543327507449856ULL,    5188164365601475096ULL,    10414575345181196316ULL,   1162492212166789136ULL,    9396848738060210946ULL,    622413200109881612ULL,
    7998357718131801918ULL,  7719627227008073923ULL,    16181433497662382080ULL,   18441958655457754079ULL,   1267153596645440ULL,       18446726464209379263ULL,   1214021438038606600ULL,    4650128814733526084ULL,
    9656144899867951104ULL,  18444421868610287615ULL,   3695311799139303489ULL,    10597006226145476632ULL,   18436046904206950398ULL,   18446726472933277663ULL,   3458977943764860944ULL,    39125045590687766ULL,
    9227453435446560384ULL,  6476955465732358656ULL,    1270314852531077632ULL,    2882448553461416064ULL,    11547238928203796481ULL,   1856618300822323264ULL,    2573991788166144ULL,       4936544992551831040ULL,
    13690941749405253631ULL, 15852669863439351807ULL,   18302628748190527413ULL,   12682135449552027479ULL,   13830554446930287982ULL,   18302628782487371519ULL,   7924083509981736956ULL,    4734295326018586370ULL };

static const U64 bishopMagicNumbers[64] = {
    16509839532542417919ULL, 14391803910955204223ULL,   1848771770702627364ULL,    347925068195328958ULL,     5189277761285652493ULL,    3750937732777063343ULL,    18429848470517967340ULL,   17870072066711748607ULL,
    16715520087474960373ULL, 2459353627279607168ULL,    7061705824611107232ULL,    8089129053103260512ULL,    7414579821471224013ULL,    9520647030890121554ULL,    17142940634164625405ULL,   9187037984654475102ULL,
    4933695867036173873ULL,  3035992416931960321ULL,    15052160563071165696ULL,   5876081268917084809ULL,    1153484746652717320ULL,    6365855841584713735ULL,    2463646859659644933ULL,    1453259901463176960ULL,
    9808859429721908488ULL,  2829141021535244552ULL,    576619101540319252ULL,     5804014844877275314ULL,    4774660099383771136ULL,    328785038479458864ULL,     2360590652863023124ULL,    569550314443282ULL,
    17563974527758635567ULL, 11698101887533589556ULL,   5764964460729992192ULL,    6953579832080335136ULL,    1318441160687747328ULL,    8090717009753444376ULL,    16751172641200572929ULL,   5558033503209157252ULL,
    17100156536247493656ULL, 7899286223048400564ULL,    4845135427956654145ULL,    2368485888099072ULL,       2399033289953272320ULL,    6976678428284034058ULL,    3134241565013966284ULL,    8661609558376259840ULL,
    17275805361393991679ULL, 15391050065516657151ULL,   11529206229534274423ULL,   9876416274250600448ULL,    16432792402597134585ULL,   11975705497012863580ULL,   11457135419348969979ULL,   9763749252098620046ULL,
    16960553411078512574ULL, 15563877356819111679ULL,   14994736884583272463ULL,   9441297368950544394ULL,    14537646123432199168ULL,   9888547162215157388ULL,    18140215579194907366ULL,   18374682062228545019ULL };

#endif // CONSTSANDENUMS_H_INCLUDED
