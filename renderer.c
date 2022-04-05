#include <sys/ioctl.h>
#include <math.h>

Tile initTile(int width, int height)
{
    Tile tile;
    tile.data = (char *)malloc(width * height * sizeof(char));
    return tile;
}

Camera initCamera(char *headerText, int lengs, int headerhorisontalPadding)
{
    Camera camera;
    camera.headerText = headerText;
    camera.headerWidth = lengs;
    camera.headerHorisontalPadding = headerhorisontalPadding;
    camera.xOffset = 0;
    camera.yOffset = 0;
    return camera;
}

void setCameraHeaderText(Camera *camera, char *text, int lengs)
{
    camera->headerText = text;
    camera->headerWidth = lengs;
}

void setCameraOffset(Camera *camera, int xDirrection, int yDirrection)
{
    camera->xOffset += xDirrection;
    camera->yOffset += yDirrection;

    /*
        char *value = (char *)malloc(11 * sizeof(char));
        memset(value, ' ', 11 * sizeof(char));
        snprintf(value, 11 * sizeof(char), "X:%i, Y:%i;", camera->xOffset, camera->yOffset);
        setCameraHeaderText(camera, value, 11);
    */
}

void cameraRender(Camera *camera, int width, int height)
{
    camera->overlay = (char *)malloc(width * height * sizeof(char));
    memset(camera->overlay, ' ', width * height * sizeof(char));

    camera->width = width;
    camera->height = height;

    int headerLineWidth = camera->width - camera->headerHorisontalPadding * 2;
    camera->headerHeight = ceil(camera->headerWidth / headerLineWidth);
    if (camera->headerHeight == 0)
        camera->headerHeight = 1;
    for (int y = 0; y < camera->headerHeight; y++)
    {
        for (int x = 0; x < headerLineWidth; x++)
        {
            if (x + y * headerLineWidth > camera->headerWidth - 1)
                break;
            camera->overlay[x + camera->headerHorisontalPadding + y * camera->width] = camera->headerText[x + y * headerLineWidth];
        }
    }
}

PointInt ZeroPoint()
{
    PointInt point;
    point.x = 0;
    point.y = 0;
    return point;
}

void drowOnScreen(Camera *camera, Scene *scene)
{
    int centeredPaddingX = (scene->width / UNICOD_COSTIL - camera->width) / 2;
    int centeredPaddingY = (scene->height - camera->height) / 2;
    for (int y = 0; y < camera->height; y++)
    {
        if (camera->yOffset + centeredPaddingY + y < 0 || camera->yOffset + centeredPaddingY + y >= scene->height) // Если камера выходит за пределы
        {
            for (int x = 0; x < camera->width; x++)
            {
                char overlaySymbol = camera->overlay[x + y * camera->width];
                if (overlaySymbol != ' ')
                {
                    printf("%c", overlaySymbol);
                    continue;
                }
                else
                {
                    printf(" ");
                }
            }
        }
        else
        {
            for (int x = 0; x < camera->width * UNICOD_COSTIL; x++)
            {
                char overlaySymbol = camera->overlay[(x / UNICOD_COSTIL) + y * camera->width];
                if (overlaySymbol != ' ')
                {
                    x += 2;
                    printf("%c", overlaySymbol);
                    continue;
                }
                if (camera->xOffset * UNICOD_COSTIL + centeredPaddingX * UNICOD_COSTIL + x < 0 || camera->xOffset * UNICOD_COSTIL + centeredPaddingX * UNICOD_COSTIL + x >= scene->width)
                {

                    x += 2;
                    printf(" ");
                    continue;
                }
                _Bool empty = true;
                for (int i = scene->layersCount - 1; i >= 0; i--)
                {
                    char symbol = scene->layers[i].data[camera->xOffset * UNICOD_COSTIL + x + centeredPaddingX * UNICOD_COSTIL + (camera->yOffset + y + centeredPaddingY) * scene->width];
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
            if (y <= camera->height - 2) // Не переходить на новую строку на последней линии
                printf("\n");
        }
    }
}

void render(Scene *scene, Camera *camera, struct winsize *w)
{
    sleep(0.01);
    system("clear");
    ioctl(STDOUT_FILENO, TIOCGWINSZ, w);
    cameraRender(camera, w->ws_col, w->ws_row);
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

// Очередной костыль.
// Сцена состоит из символов юникода которые занимают по 3 чара
// А оверлей Камеры состоит из обычных символов

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