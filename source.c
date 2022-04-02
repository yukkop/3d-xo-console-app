#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "variants/fieldfill.c"
#include "constants.h"

#define TILE struct Tile

struct Tile
{
    char data[TILE_HEIGHT * TILE_WIDTH];
};

void renderField(char field[], int height, int width)
{
    printf("height: %i, width: %i \n", height, width);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            printf("%c", field[x + y * width]);
        }
        printf("\n");
    }
}

void catTileSHeetToTiles(TILE *tiles, char *tileSheet)
{
    printf("Make tiles \n");
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
                printf("\n");
            }
        }
    }
}

int main(int argc, char *argv[])
{
    char *field;

    TILE x_Tiles[FIELD_HEIGHT * FIELD_WIDTH];

    int fieldCharHeight = FIELD_HEIGHT * TILE_HEIGHT;
    int fieldCharWidth = FIELD_WIDTH * TILE_WIDTH;

    catTileSHeetToTiles(x_Tiles, X_TILES);
    renderField(x_Tiles[13].data, TILE_HEIGHT, TILE_WIDTH);
}