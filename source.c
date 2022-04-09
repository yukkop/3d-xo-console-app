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

#include "scenes/sandboxmod.c"
#include "scenes/menu.c"

int main(int argc, char *argv[])
{
    struct winsize w;

    Camera camera = initCamera("", 0, 3);

    Scene scene = initScene(FIELD_WIDTH * TILE_WIDTH * UNICOD_COSTIL, FIELD_HEIGHT * TILE_HEIGHT, 2);

    loadMenu(&camera, &scene, &w);

    system("clear");
}