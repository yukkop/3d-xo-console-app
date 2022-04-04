#include <sys/ioctl.h>

void drowOnScreen(Camera *camera, Scene *scene)
{
    for (int y = 0; y < camera->height; y++)
    {
        if (y < scene->height)
            for (int x = 0; x < camera->width * UNICOD_COSTIL; x++)
            {
                if (x >= scene->width)
                {
                    continue;
                }
                else
                {
                    _Bool empty = true;
                    for (int i = scene->layersCount - 1; i >= 0; i--)
                    {
                        char symbol = scene->layers[i].data[camera->xOffset + x + (camera->yOffset + y) * scene->width];
                        if (symbol != VOID_SYMBOL[x % UNICOD_COSTIL])
                        {
                            printf("%c", symbol);
                            empty = false;
                            break;
                        }
                    }
                    if (empty)
                    {
                        printf("%c", VOID_SYMBOL[x % UNICOD_COSTIL]);
                    }
                }
            }
        if (y <= camera->height - 2) // Не переходить на новую строку на последней линии
            printf("\n");
    }
}

void render(Scene *scene, Camera *camera, struct winsize *w)
{
    system("clear");
    ioctl(STDOUT_FILENO, TIOCGWINSZ, w);
    camera->xOffset = 0;
    camera->yOffset = 0;
    camera->width = w->ws_col;
    camera->height = w->ws_row;

    drowOnScreen(camera, scene);
}

void inputTileToLayer(Tile *tile, int tileWidth, int tileHeight, int x, int y, Layer *layer, int layerWidth)
{
    for (int yTile = 0; yTile < tileHeight; yTile++)
    {
        for (int xTile = 0; xTile < tileWidth; xTile++)
        {
            int yOffset = y * tileHeight * layerWidth * tileWidth;
            int xOffset = x * tileWidth;
            int index = xOffset + yOffset + xTile + yTile * layerWidth * tileWidth;
            layer->data[index] =
                tile->data[xTile + yTile * tileWidth];
        }
    }
}

Scene initScene(int width, int height, int layersCount)
{
    Scene scene;
    scene.width = width;
    scene.height = height;
    scene.layersCount = layersCount;
    scene.layers = (Layer *)malloc(width * height * sizeof(Layer));
    for (int i = 0; i < layersCount; i++)
    {
        scene.layers[i].data = (char *)malloc(width * height * sizeof(char));
        for (int j = 0; j < width * height; j++)
        {
            scene.layers[i].data[j] = VOID_SYMBOL[j % UNICOD_COSTIL];
        }
    }
    return scene;
}

Tile initTile(int width, int height)
{
    Tile tile;
    tile.data = (char *)malloc(width * height * sizeof(char));
    return tile;
}