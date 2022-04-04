typedef struct
{
    char *data;
} Layer;

typedef struct
{
    char *data;
} Tile;

typedef struct
{
    int headerWidth;
    int headerHeight;
    int headerHorisontalPadding;
    char *headerText;

    char *overlay;
    int width;
    int height;
    int xOffset;
    int yOffset;
} Camera;

typedef struct
{
    Layer *layers;
    int layersCount;
    int width;
    int height;
} Scene;

typedef struct
{
    int x;
    int y;
} PointInt;