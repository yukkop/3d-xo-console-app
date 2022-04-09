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

    camera.isPopup = false;
    camera.needRerender = true;

    return camera;
}

void setCameraHeaderText(Camera *camera, char *text, int lengs)
{
    camera->headerText = text;
    camera->headerWidth = lengs;

    camera->needRerender = true;
}

void setCameraPopupText(Camera *camera, char *text, int lengs)
{
    camera->popupText = text;
    camera->popupTextWidth = lengs;

    camera->needRerender = true;
}

void disableCameraPopup(Camera *camera)
{
    camera->isPopup = false;
}

void enableCameraPopup(Camera *camera)
{
    camera->isPopup = true;
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
    if (camera->needRerender)
    {
        camera->overlay = (char *)malloc(width * height * sizeof(char));
        memset(camera->overlay, ' ', width * height * sizeof(char));

        camera->width = width;
        camera->height = height;

        // Шапка

        int headerLineWidth = camera->width - camera->headerHorisontalPadding * 2;
        camera->headerHeight = camera->headerWidth / headerLineWidth;
        if (camera->headerWidth % headerLineWidth != 0)
            camera->headerHeight += 1;

        for (int y = 0; y < camera->headerHeight; y++)
        {
            for (int x = 0; x < headerLineWidth; x++)
            {
                if (x + y * headerLineWidth > camera->headerWidth - 1)
                    break;

                if (camera->headerText[x + y * headerLineWidth] == ' ')
                    camera->overlay[x + camera->headerHorisontalPadding + y * camera->width] = SPEC_SMBOL_OVERLAY_VOID;
                else
                    camera->overlay[x + camera->headerHorisontalPadding + y * camera->width] = camera->headerText[x + y * headerLineWidth];
            }
        }

        // Попап

        if (camera->isPopup == true)
        {
            int popupHorisontalPadding = 2; // отступы текста от границ

            int persent = 60; // Процент от экрана (максимальная ширина попапа)
            int maxPopupWidth = camera->width / 100. * 60;
            int popupTextHeight = camera->popupTextWidth * 1. / maxPopupWidth;
            if (camera->popupTextWidth % maxPopupWidth != 0)
                popupTextHeight += 1;

            int popupHeight = popupTextHeight + 2; // 2 из за рамок с двух сторон
            int popupWidth = 0;
            if (popupTextHeight > 1)
                popupWidth = maxPopupWidth + popupHorisontalPadding * 2;
            else
                popupWidth = camera->popupTextWidth + popupHorisontalPadding * 2;

            // Создаем попап
            char *popupText = (char *)malloc(camera->popupTextWidth * sizeof(char));
            for (int i = 0; i < camera->popupTextWidth; i++)
            {
                if (camera->popupText[i] == ' ')
                {
                    popupText[i] = SPEC_SMBOL_OVERLAY_VOID;
                }
                else
                {
                    popupText[i] = camera->popupText[i];
                }
            }

            char *popup = (char *)malloc(popupHeight * popupWidth * sizeof(char));
            memset(popup, SPEC_SMBOL_OVERLAY_VOID, popupHeight * popupWidth * sizeof(char));

            int index = 0;
            popup[index++] = '+';
            memset(&(popup[index]), '-', (popupWidth - 2) * sizeof(char)); // заполняем строку '-'; -2 из-за '+' по углам
            index += popupWidth - 2;                                       // ставим индекс за memset
            popup[index++] = '+';

            for (int y = 0; y < popupTextHeight; y++)
            {
                int x = 0;
                popup[x + (y + 1) * popupWidth] = '|';
                x += popupHorisontalPadding;
                for (x; x < popupWidth - popupHorisontalPadding; x++)
                {
                    int index = x - popupHorisontalPadding + y * maxPopupWidth;
                    if (index >= camera->popupTextWidth)
                    {
                        x = maxPopupWidth + popupHorisontalPadding;
                        break;
                    }
                    if (camera->popupText[index] == ' ')
                        popup[x + (y + 1) * popupWidth] = '_';

                    else
                        popup[x + (y + 1) * popupWidth] = camera->popupText[index];
                }
                x += popupHorisontalPadding - 1;
                popup[x + (y + 1) * popupWidth] = '|';

                index += x + 1;
            }

            popup[index++] = '+';
            memset(&(popup[index]), '-', (popupWidth - 2) * sizeof(char)); // заполняем строку '-'; -2 из-за '+' по углам
            index += popupWidth - 2;                                       // ставим индекс за memset
            popup[index++] = '+';

            // drowBox(popup, popupWidth, popupHeight);
            //  Загружаем попап в оверлей

            int freeSpaceHeight = camera->height - camera->headerHeight;

            int centeredPopupPaddingX = (camera->width - popupWidth) / 2;
            int centeredPopupPaddingY = (freeSpaceHeight - popupHeight) / 2;

            for (int y = 0; y < popupHeight; y++)
            {
                for (int x = 0; x < popupWidth; x++)
                {
                    camera->overlay[x + centeredPopupPaddingX + (y + centeredPopupPaddingY) * camera->width] = popup[x + y * popupWidth];
                }
            }
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
                    if (overlaySymbol == SPEC_SMBOL_OVERLAY_VOID)
                    {
                        printf(" ");
                        continue;
                    }
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

                    if (overlaySymbol == SPEC_SMBOL_OVERLAY_VOID)
                    {
                        printf(" ");
                        continue;
                    }

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
        }

        if (y <= camera->height - 2) // Не переходить на новую строку на последней линии
            printf("\n");
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