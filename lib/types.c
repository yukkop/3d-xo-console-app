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

    _Bool isPopup;
    char *popupText;
    int popupTextWidth;

    char *overlay;

    int width;
    int height;
    int xOffset;
    int yOffset;

    _Bool needRerender;
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