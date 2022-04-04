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
void cutTileSHeetToTiles(Tile *tiles, char *tileSheet)
{
    // printf("Make tiles \n");
    for (int yTile = 0; yTile < FIELD_HEIGHT; yTile++)
    {

        for (int xTile = 0; xTile < FIELD_WIDTH; xTile++)
        {
            for (int yChar = 0; yChar < TILE_HEIGHT; yChar++)
            {
                for (int xChar = 0; xChar < TILE_WIDTH; xChar++)
                {
                    tiles[yTile * FIELD_WIDTH + xTile].data[yChar * TILE_WIDTH + xChar] =
                        tileSheet[/* x */ (xChar + xTile * TILE_WIDTH) + /* y */ (yChar + yTile * TILE_HEIGHT) * FIELD_WIDTH * TILE_WIDTH];

                    // printf("%c", tileSheet[/* x */ (xChar + xTile * TILE_WIDTH) + /* y */ (yChar + yTile * TILE_HEIGHT) * FIELD_WIDTH * TILE_WIDTH]);
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
            TILE_WIDTH,
            TILE_HEIGHT,
            x,
            y,
            layer,
            FIELD_WIDTH);
    }
}

void put();
void findEmptyTilePoint(enum minimapSymbols *minimap, PointInt *selectedTilePoint);

Tile emptyTiles[FIELD_HEIGHT * FIELD_WIDTH];
Tile xTiles[FIELD_HEIGHT * FIELD_WIDTH];
Tile oTiles[FIELD_HEIGHT * FIELD_WIDTH];
Tile chosenTile[FIELD_HEIGHT * FIELD_WIDTH];
Tile transparencyTile;

int main(int argc, char *argv[])
{
    struct winsize w;

    for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; i++)
    {
        emptyTiles[i] = initTile(TILE_WIDTH, TILE_HEIGHT);
        xTiles[i] = initTile(TILE_WIDTH, TILE_HEIGHT);
        oTiles[i] = initTile(TILE_WIDTH, TILE_HEIGHT);
        chosenTile[i] = initTile(TILE_WIDTH, TILE_HEIGHT);
    }

    cutTileSHeetToTiles(emptyTiles, EMPTY_TILES);
    cutTileSHeetToTiles(oTiles, O_TILES);
    cutTileSHeetToTiles(xTiles, X_TILES);
    cutTileSHeetToTiles(chosenTile, CHOSEN_TILES);
    transparencyTile = initTile(TILE_WIDTH, TILE_HEIGHT);
    transparencyTile.data = TRANSPARENCY_TILE;

    Scene scene = initScene(FIELD_WIDTH * TILE_WIDTH, FIELD_HEIGHT * TILE_HEIGHT, 2);
    fillScene(emptyTiles, &scene.layers[0]);

    enum minimapSymbols minimap[FIELD_HEIGHT * FIELD_WIDTH];
    memset(minimap, emptySymbol, sizeof(enum minimapSymbols) * FIELD_HEIGHT * FIELD_WIDTH);
    printf("%s\n", minimap);

    Camera camera;
    PointInt selectedTilePoint;
    selectedTilePoint.x = 0;
    selectedTilePoint.y = 0;

    char command = 'q';
    render(&scene, &camera, &w);
    do
    {
        command = getch();

        switch (command)
        {
        case 'p':
        case 'P':
            findEmptyTilePoint(minimap, &selectedTilePoint);
            put(&selectedTilePoint, minimap, &scene, &camera, &w);
            selectedTilePoint.x = 0;
            selectedTilePoint.y = 0;
            break;
        case 'r':
        case 'R':
            break;
        default:
            continue;
        }

        render(&scene, &camera, &w);
    } while (command != 'q');
    system("clear");
}

void chageSelectedTile(PointInt *selectedTilePoint, Scene *scene, PointInt newPoint)
{
    inputTileToLayer(
        &transparencyTile,
        TILE_WIDTH,
        TILE_HEIGHT,
        selectedTilePoint->x,
        selectedTilePoint->y,
        &scene->layers[1],
        FIELD_WIDTH);

    selectedTilePoint->x = newPoint.x;
    selectedTilePoint->y = newPoint.y;

    inputTileToLayer(
        &chosenTile[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
        TILE_WIDTH,
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
    /*
    while (minimap[x + y * FIELD_WIDTH] != emptySymbol)
    {

        x += xDirrection;
        if (x >= FIELD_WIDTH)
            x = 0;
        else if (x < 0)
            x = FIELD_WIDTH - 1;

        y += yDirrection;
        if (y >= FIELD_HEIGHT)
            y = 0;
        else if (y < 0)
            y = FIELD_HEIGHT - 1;
    }
    */
    PointInt answer;
    answer.x = x;
    answer.y = y;
    return answer;
}

void findEmptyTilePoint(enum minimapSymbols *minimap, PointInt *selectedTilePoint)
{
    int i = 0;
    while (minimap[i] != emptySymbol)
    {
        i += 1;
    }
    selectedTilePoint->x = i % FIELD_WIDTH;
    selectedTilePoint->y = i / FIELD_WIDTH;
}

void put(PointInt *selectedTilePoint, enum minimapSymbols *minimap, Scene *scene, Camera *camera, struct winsize *w)
{
    char command = 'q';
    inputTileToLayer(
        &chosenTile[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
        TILE_WIDTH,
        TILE_HEIGHT,
        selectedTilePoint->x,
        selectedTilePoint->y,
        &scene->layers[1],
        FIELD_WIDTH);

    render(scene, camera, w);
    do
    {
        command = getch();
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
            inputTileToLayer(
                &oTiles[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
                TILE_WIDTH,
                TILE_HEIGHT,
                selectedTilePoint->x,
                selectedTilePoint->y,
                &scene->layers[0],
                FIELD_WIDTH);

            inputTileToLayer(
                &transparencyTile,
                TILE_WIDTH,
                TILE_HEIGHT,
                selectedTilePoint->x,
                selectedTilePoint->y,
                &scene->layers[1],
                FIELD_WIDTH);
            minimap[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH] = oSymbol;
            return;
        case 'x':
        case 'X':
            inputTileToLayer(
                &xTiles[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
                TILE_WIDTH,
                TILE_HEIGHT,
                selectedTilePoint->x,
                selectedTilePoint->y,
                &scene->layers[0],
                FIELD_WIDTH);

            inputTileToLayer(
                &transparencyTile,
                TILE_WIDTH,
                TILE_HEIGHT,
                selectedTilePoint->x,
                selectedTilePoint->y,
                &scene->layers[1],
                FIELD_WIDTH);
            minimap[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH] = xSymbol;
            return;
        default:
            continue;
        }
        render(scene, camera, w);
    } while (command != 'q');

    inputTileToLayer(
        &transparencyTile,
        TILE_WIDTH,
        TILE_HEIGHT,
        selectedTilePoint->x,
        selectedTilePoint->y,
        &scene->layers[1],
        FIELD_WIDTH);
}