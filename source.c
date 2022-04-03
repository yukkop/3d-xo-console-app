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

typedef struct
{
    int x;
    int y;
} PointInt;

void renderOnScreen(Camera *camera, char field[], int width, int height)
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
    for (int y = 0; y < camera->height; y++)
    {
        if (y >= height)
            for (int x = 0; x < camera->width; x++)
            {
                printf("%s", VOID_SYMBOL);
            }
        else
            for (int x = 0; x < camera->width * UNICOD_COSTIL; x++)
            {
                if (x >= width)
                    continue;
                else
                    printf("%c", field[camera->xOffset + x + (camera->yOffset + y) * width]);
            }
        if (y <= camera->height - 2)
            printf("\n");
    }
}

void render(char *field, Camera *camera, struct winsize *w)
{
    system("clear");
    ioctl(STDOUT_FILENO, TIOCGWINSZ, w);
    camera->xOffset = 0;
    camera->yOffset = 0;
    camera->width = w->ws_col;
    camera->height = w->ws_row;

    renderOnScreen(camera, field, FIELD_WIDTH * TILE_WIDTH, FIELD_HEIGHT * TILE_HEIGHT);
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

void initField(Tile *defaultTiles, char *field)
{
    for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++)
    {
        int x = i % FIELD_WIDTH;
        int y = i / FIELD_WIDTH;
        // printf("%i:%i\n", x, y);
        inputTileToScreen(&defaultTiles[i], x, y, field);
    }
}

void put();

Tile emptyTiles[FIELD_HEIGHT * FIELD_WIDTH];
Tile xTiles[FIELD_HEIGHT * FIELD_WIDTH];
Tile oTiles[FIELD_HEIGHT * FIELD_WIDTH];
Tile chosenTile[FIELD_HEIGHT * FIELD_WIDTH];

int main(int argc, char *argv[])
{
    struct winsize w;

    int fieldSize = FIELD_HEIGHT * FIELD_WIDTH * TILE_HEIGHT * TILE_WIDTH;
    char field[fieldSize];

    cutTileSHeetToTiles(emptyTiles, EMPTY_TILES);
    cutTileSHeetToTiles(oTiles, O_TILES);
    cutTileSHeetToTiles(xTiles, X_TILES);
    cutTileSHeetToTiles(chosenTile, CHOSEN_TILES);

    initField(emptyTiles, field);

    Camera camera;
    PointInt selectedTilePoint;
    selectedTilePoint.x = 1;
    selectedTilePoint.y = 4;

    char command = 'q';

    render(field, &camera, &w);
    do
    {
        command = getch();

        switch (command)
        {
        case 'p':
        case 'P':
            put(&selectedTilePoint, field, &camera, &w);

            break;
        case 'r':
        case 'R':
            break;
        default:
            continue;
        }

        render(field, &camera, &w);
    } while (command != 'q');
    system("clear");
}

void chageSelectedTile(PointInt *selectedTilePoint, char *field, int newX, int newY)
{
    inputTileToScreen(
        &emptyTiles[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
        selectedTilePoint->x,
        selectedTilePoint->y,
        field);

    selectedTilePoint->x = newX;
    selectedTilePoint->y = newY;

    inputTileToScreen(
        &chosenTile[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
        selectedTilePoint->x,
        selectedTilePoint->y,
        field);
}

void put(PointInt *selectedTilePoint, char *field, Camera *camera, struct winsize *w)
{
    char command = 'q';
    inputTileToScreen(
        &chosenTile[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
        selectedTilePoint->x,
        selectedTilePoint->y,
        field);

    render(field, camera, w);
    do
    {
        command = getch();
        switch (command)
        {
        case 37: // LeftArrow
        case 'a':
            chageSelectedTile(selectedTilePoint, field, selectedTilePoint->x - 1, selectedTilePoint->y);
            break;
        case 38: // UpArrow
        case 'w':
            chageSelectedTile(selectedTilePoint, field, selectedTilePoint->x, selectedTilePoint->y - 1);
            break;
        case 39: // RightArrow
        case 'd':
            chageSelectedTile(selectedTilePoint, field, selectedTilePoint->x + 1, selectedTilePoint->y);
            break;
        case 40: // DowndArrow
        case 's':
            chageSelectedTile(selectedTilePoint, field, selectedTilePoint->x, selectedTilePoint->y + 1);
            break;
        case 'o':
        case 'O':
            inputTileToScreen(
                &oTiles[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
                selectedTilePoint->x,
                selectedTilePoint->y,
                field);
            return;
        case 'x':
        case 'X':
            inputTileToScreen(
                &xTiles[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
                selectedTilePoint->x,
                selectedTilePoint->y,
                field);
            return;
        default:
            continue;
        }
        render(field, camera, w);
    } while (command != 'q');

    inputTileToScreen(
        &emptyTiles[selectedTilePoint->x + selectedTilePoint->y * FIELD_WIDTH],
        selectedTilePoint->x,
        selectedTilePoint->y,
        field);
}