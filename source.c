#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/binpow.c"
#include <unistd.h>
#include "constants.h"
#include "lib/types.c"
#include "renderer.c"

#ifndef __linux
#include <conio.h>
#else
#include "lib/lgetch.c"
#endif

enum minimapSymbols
{
    emptySymbol = 0,
    oSymbol = 1,
    xSymbol = 2
};

Tile emptyTiles[FIELD_HEIGHT * FIELD_WIDTH];
Tile xTiles[FIELD_HEIGHT * FIELD_WIDTH];
Tile oTiles[FIELD_HEIGHT * FIELD_WIDTH];
Tile chosenTile[FIELD_HEIGHT * FIELD_WIDTH];
Tile transparencyTile;

void cutTileSHeetToTiles(Tile *tiles, char *tileSheet)
{
    // printf("Make tiles \n");
    for (int yTile = 0; yTile < FIELD_HEIGHT; yTile++)
    {

        for (int xTile = 0; xTile < FIELD_WIDTH; xTile++)
        {
            for (int yChar = 0; yChar < TILE_HEIGHT; yChar++)
            {
                for (int xChar = 0; xChar < TILE_WIDTH * UNICOD_COSTIL; xChar++)
                {
                    tiles[yTile * FIELD_WIDTH + xTile].data[yChar * TILE_WIDTH * UNICOD_COSTIL + xChar] =
                        tileSheet[/* x */ (xChar + xTile * TILE_WIDTH * UNICOD_COSTIL) + /* y */ (yChar + yTile * TILE_HEIGHT) * FIELD_WIDTH * TILE_WIDTH * UNICOD_COSTIL];

                    // printf("%c", tileSheet[/* x */ (xChar + xTile * TILE_WIDTH * UNICOD_COSTIL) + /* y */ (yChar + yTile * TILE_HEIGHT) * FIELD_WIDTH * TILE_WIDTH * UNICOD_COSTIL]);
                }
                // printf("\n");
            }
        }
    }
}

void fillScene(Tile *defaultTiles, Layer *layer)
{
    for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++)
    {
        int x = i % FIELD_WIDTH;
        int y = i / FIELD_WIDTH;
        // printf("%i:%i\n", x, y);
        inputTileToLayer(
            &defaultTiles[i],
            TILE_WIDTH * UNICOD_COSTIL,
            TILE_HEIGHT,
            x,
            y,
            layer,
            FIELD_WIDTH);
    }
}

void drowBox(char *arr, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            printf("%c", arr[x + y * width]);
        }
        if (y <= height - 2) // Не переходить на новую строку на последней линии
            printf("\n");
    }
}

void chageSelectedTile(PointInt *selectedTilePoint, Scene *scene, PointInt newPoint)
{
    inputTileToLayer(
        &transparencyTile,
        TILE_WIDTH * UNICOD_COSTIL,
        TILE_HEIGHT,
        selectedTilePoint->x,
        selectedTilePoint->y,
        &scene->layers[1],
        FIELD_WIDTH);

    selectedTilePoint->x = newPoint.x;
    selectedTilePoint->y = newPoint.y;

    inputTileToLayer(
        &chosenTile[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
        TILE_WIDTH * UNICOD_COSTIL,
        TILE_HEIGHT,
        selectedTilePoint->x,
        selectedTilePoint->y,
        &scene->layers[1],
        FIELD_WIDTH);
}

PointInt returnEmptyTilePointTowards(enum minimapSymbols *minimap, PointInt *selectedTilePoint, int xDirrection, int yDirrection)
{
    // int xOffset = xDirrection;
    // int yOffset = yDirrection;
    int x = selectedTilePoint->x + xDirrection;
    if (x >= FIELD_WIDTH)
        x = 0;
    else if (x < 0)
        x = FIELD_WIDTH - 1;

    int y = selectedTilePoint->y + yDirrection;
    if (y >= FIELD_HEIGHT)
        y = 0;
    else if (y < 0)
        y = FIELD_HEIGHT - 1;
    PointInt answer;
    answer.x = x;
    answer.y = y;
    return answer;
}

void findEmptyTilePoint(enum minimapSymbols *minimap, PointInt *selectedTilePoint)
{
    int lengs = FIELD_WIDTH * FIELD_HEIGHT;
    for (int i = lengs / 2; i < lengs; i++)
    {
        if (minimap[i] == emptySymbol)
        {
            selectedTilePoint->x = i % FIELD_WIDTH;
            selectedTilePoint->y = i / FIELD_WIDTH;
            // printf("%i, %i", selectedTilePoint->x, selectedTilePoint->y);
            return;
        }
        else if (minimap[lengs / 2 - (i - lengs / 2)] == emptySymbol)
        {
            selectedTilePoint->x = (lengs / 2 - (i - lengs / 2)) % FIELD_WIDTH;
            selectedTilePoint->y = (lengs / 2 - (i - lengs / 2)) / FIELD_WIDTH;
            return;
        }
    }
    selectedTilePoint->x = FIELD_WIDTH / 2;
    selectedTilePoint->y = FIELD_HEIGHT / 2;
}

void put(PointInt *selectedTilePoint, enum minimapSymbols *minimap, Scene *scene, Camera *camera, struct winsize *w)
{
    char command = 'q';
    inputTileToLayer(
        &chosenTile[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
        TILE_WIDTH * UNICOD_COSTIL,
        TILE_HEIGHT,
        selectedTilePoint->x,
        selectedTilePoint->y,
        &scene->layers[1],
        FIELD_WIDTH);

    render(scene, camera, w);
    do
    {
        command = getch();
        disableCameraPopup(camera);
        switch (command)
        {
        case 37: // LeftArrow
        case 'a':
        case 'A':
            chageSelectedTile(selectedTilePoint, scene,
                              returnEmptyTilePointTowards(minimap, selectedTilePoint, -1, 0));
            break;
        case 38: // UpArrow
        case 'w':
        case 'W':
            chageSelectedTile(selectedTilePoint, scene,
                              returnEmptyTilePointTowards(minimap, selectedTilePoint, 0, -1));
            break;
        case 39: // RightArrow
        case 'd':
        case 'D':
            chageSelectedTile(selectedTilePoint, scene,
                              returnEmptyTilePointTowards(minimap, selectedTilePoint, +1, 0));
            break;
        case 40: // DowndArrow
        case 's':
        case 'S':
            chageSelectedTile(selectedTilePoint, scene,
                              returnEmptyTilePointTowards(minimap, selectedTilePoint, 0, +1));
            break;
        case 'o':
        case 'O':
            if (minimap[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH] == emptySymbol)
            {

                inputTileToLayer(
                    &oTiles[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
                    TILE_WIDTH * UNICOD_COSTIL,
                    TILE_HEIGHT,
                    selectedTilePoint->x,
                    selectedTilePoint->y,
                    &scene->layers[0],
                    FIELD_WIDTH);
                minimap[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH] = oSymbol;
                command = 'q';
            }
            else
            {
                setCameraPopupText(camera, "this place already busy", 23);
                enableCameraPopup(camera);
            }
            break;
        case 'x':
        case 'X':
            if (minimap[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH] == emptySymbol)
            {
                inputTileToLayer(
                    &xTiles[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
                    TILE_WIDTH * UNICOD_COSTIL,
                    TILE_HEIGHT,
                    selectedTilePoint->x,
                    selectedTilePoint->y,
                    &scene->layers[0],
                    FIELD_WIDTH);

                minimap[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH] = xSymbol;
                command = 'q';
            }
            else
            {
                setCameraPopupText(camera, "this place already busy", 23);
                enableCameraPopup(camera);
            }
            break;
        default:
            continue;
        }
        setCameraHeaderText(camera, "#3d-xo v1.0# [q - back] [o - put o] [x - put x] [w,a,s,d - mark movement]", 73);
        render(scene, camera, w);
    } while (command != 'q');

    inputTileToLayer(
        &transparencyTile,
        TILE_WIDTH * UNICOD_COSTIL,
        TILE_HEIGHT,
        selectedTilePoint->x,
        selectedTilePoint->y,
        &scene->layers[1],
        FIELD_WIDTH);
}

void loadGameScene(Camera *camera, Scene *scene, struct winsize *w)
{
    disableCameraPopup(camera);

    for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; i++)
    {
        emptyTiles[i] = initTile(TILE_WIDTH * UNICOD_COSTIL, TILE_HEIGHT);
        xTiles[i] = initTile(TILE_WIDTH * UNICOD_COSTIL, TILE_HEIGHT);
        oTiles[i] = initTile(TILE_WIDTH * UNICOD_COSTIL, TILE_HEIGHT);
        chosenTile[i] = initTile(TILE_WIDTH * UNICOD_COSTIL, TILE_HEIGHT);
    }

    cutTileSHeetToTiles(emptyTiles, EMPTY_TILES);
    cutTileSHeetToTiles(oTiles, O_TILES);
    cutTileSHeetToTiles(xTiles, X_TILES);
    cutTileSHeetToTiles(chosenTile, CHOSEN_TILES);
    transparencyTile = initTile(TILE_WIDTH * UNICOD_COSTIL, TILE_HEIGHT);
    transparencyTile.data = TRANSPARENCY_TILE;

    fillScene(emptyTiles, &scene->layers[0]);

    enum minimapSymbols minimap[FIELD_HEIGHT * FIELD_WIDTH];
    memset(minimap, emptySymbol, sizeof(enum minimapSymbols) * FIELD_HEIGHT * FIELD_WIDTH);

    PointInt selectedTilePoint;
    selectedTilePoint.x = FIELD_WIDTH / 2;
    selectedTilePoint.y = FIELD_HEIGHT / 2;

    char command = 'q';
    setCameraHeaderText(camera, "#3d-xo v1.0# [q - back] [p - put] [w,a,s,d - camera movement]", 61);

    render(scene, camera, w);
    do
    {
        command = getch();

        switch (command)
        {
        case 'p':
        case 'P':
            setCameraHeaderText(camera, "#3d-xo v1.0# [q - back] [o - put o] [x - put x] [w,a,s,d - mark movement]", 73);
            findEmptyTilePoint(minimap, &selectedTilePoint);
            put(&selectedTilePoint, minimap, scene, camera, w);
            break;
        case 'w':
        case 'W':
            setCameraOffset(camera, 0, -1);
            break;
        case 'a':
        case 'A':
            setCameraOffset(camera, -1, 0);
            break;
        case 's':
        case 'S':
            setCameraOffset(camera, 0, +1);
            break;
        case 'd':
        case 'D':
            setCameraOffset(camera, +1, 0);
            break;
        case 'r':
        case 'R':
            break;
        default:
            continue;
        }

        setCameraHeaderText(camera, "#3d-xo v1.0# [q - back] [p - put] [w,a,s,d - camera movement]", 61);
        render(scene, camera, w);
    } while (command != 'q');
}

void loadMenu(Camera *camera, Scene *scene, struct winsize *w)
{

    setCameraHeaderText(camera, "#3d-xo v1.0# Menu", 17);
    setCameraPopupText(camera, "[S - singlplayer]", 17);
    enableCameraPopup(camera);

    render(scene, camera, w);

    char command = 'q';
    do
    {
        command = getch();

        switch (command)
        {
        case 's':
        case 'S':
            loadGameScene(camera, scene, w);
            break;
        default:
            continue;
        }

        setCameraHeaderText(camera, "#3d-xo v1.0# Menu", 17);
        setCameraPopupText(camera, "[S - singlplayer]", 17);
        enableCameraPopup(camera);

        render(scene, camera, w);
    } while (command != 'q');
}

int main(int argc, char *argv[])
{
    struct winsize w;

    Camera camera = initCamera("", 0, 3);

    Scene scene = initScene(FIELD_WIDTH * TILE_WIDTH * UNICOD_COSTIL, FIELD_HEIGHT * TILE_HEIGHT, 2);

    loadMenu(&camera, &scene, &w);

    system("clear");
}