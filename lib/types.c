typedef struct
{
    char data[TILE_HEIGHT * TILE_WIDTH];
} Tile;

typedef struct
{
    int width;
    int height;
    int xOffset;
    int yOffset;
} Camera;

typedef struct
{
    int x;
    int y;
} PointInt;

enum minimapSymbols
{
    emptySymbol = 0,
    oSymbol = 1,
    xSymbol = 2
};