#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "variants/fieldfill.c"
#include "constants.h"
#include <sys/ioctl.h>
#include <unistd.h>

#ifndef __linux

#include <conio.h>

#else

#include "lib/lgetch.c"

#endif

#define VOID_SYMBOL "⠀"

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

void renderScreen(Camera screen, char field[], int width, int height)
{
    // printf("height: %i, width: %i \n", height, width);
    /*
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            printf("%c", field[x + y * width]);
        }
        printf("\n");
    }
    */
    for (int y = 0; y < screen.height - 1; y++)
    {
        if (y >= height)
            for (int x = 0; x < screen.width; x++)
            {
                printf("%s", VOID_SYMBOL);
            }
        else
            for (int x = 0; x < screen.width * UNICOD_COSTIL; x++)
            {
                if (x >= width)
                    continue;
                else
                    printf("%c", field[screen.xOffset + x + (screen.yOffset + y) * width]);
            }
        if (y <= screen.height - 1)
            printf("\n");
    }
}

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

void inputTileToScreen(Tile *tile, int x, int y, char *screen)
{
    for (int yTile = 0; yTile < TILE_HEIGHT; yTile++)
    {
        for (int xTile = 0; xTile < TILE_WIDTH; xTile++)
        {
            int yOffset = y * TILE_HEIGHT * FIELD_WIDTH * TILE_WIDTH;
            int xOffset = x * TILE_WIDTH;
            int index = xOffset + yOffset + xTile + yTile * FIELD_WIDTH * TILE_WIDTH;
            screen[index] =
                tile->data[xTile + yTile * TILE_WIDTH];
        }
    }
}

int main(int argc, char *argv[])
{
    struct winsize w;

    int fieldSize = FIELD_HEIGHT * FIELD_WIDTH * TILE_HEIGHT * TILE_WIDTH;
    char field[fieldSize];

    Tile emptyTiles[FIELD_HEIGHT * FIELD_WIDTH];
    Tile xTiles[FIELD_HEIGHT * FIELD_WIDTH];
    Tile oTiles[FIELD_HEIGHT * FIELD_WIDTH];

    cutTileSHeetToTiles(emptyTiles, EMPTY_TILES);
    cutTileSHeetToTiles(oTiles, O_TILES);
    cutTileSHeetToTiles(xTiles, X_TILES);

    for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++)
    {
        int x = i % FIELD_WIDTH;
        int y = i / FIELD_WIDTH;
        // printf("%i:%i\n", x, y);
        inputTileToScreen(&emptyTiles[i], x, y, field);
    }

    inputTileToScreen(&xTiles[1 + 1 * FIELD_WIDTH], 1, 1, field);
    inputTileToScreen(&oTiles[0 + 2 * FIELD_WIDTH], 0, 2, field);

    while (1)
    {
        system("clear");
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        Camera camera;
        camera.xOffset = 0;
        camera.yOffset = 0;
        camera.width = w.ws_col;
        camera.height = w.ws_row;

        renderScreen(camera, field, FIELD_WIDTH * TILE_WIDTH, FIELD_HEIGHT * TILE_HEIGHT);

        getch();
    }
}
